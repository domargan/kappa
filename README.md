# Kappa
 
Kappa is an in-memory, update-centric stream graph processing system. It maintains a dynamic directed graph in memory, applies batches of edge updates, and schedules user-defined algorithm-specific computation over the parts of the graph affected by those updates.

The central idea is that graph computation should be driven by **changes in topology**, not by repeatedly treating the whole graph as a static input. An edge insertion or deletion enters the system as an update. Kappa's update-centric programming model allows graph analytics algorithms to control whether that update changes vertex state. If it does, the runtime schedules further vertex computations until the change stops propagating.

Kappa is an experimental systems research prototype. It is not packaged as a general-purpose graph-processing library, but it contains a complete end-to-end runtime, implementations of several dynamic and static graph algorithms, data-ingestion utilities, and experiment support code.

## What Kappa does

Kappa's design and implementation explore dynamic graph processing on a single large-memory machine. It focuses on questions such as:

* how to express graph algorithms over a changing graph;
* how to trigger computation from graph updates rather than from full graph iterations;
* how to propagate only the work caused by an edge insertion or deletion;
* how to batch update streams;
* how to schedule graph updates and computation tasks across worker threads in a NUMA environment;
* how to use weakly connected components to safely restrict recomputation after edge deletions;
* how to evaluate dynamic graph workloads using preprocessed edge-list datasets.

In short:

> Kappa is an update-centric dynamic graph processing system whose runtime maintains weakly connected-component labels to support affected-region recomputation, especially after edge deletions.

## Execution model

The current executable is defined in:

```text
src/init.cpp
```

At a high level, the current execution path is:

```text
initialise Scheduler and NUMA-pinned workers
        |
        v
load core graph from edge-list file
        |
        v
preload initial vertex states
        |
        v
ingest update stream from edge-list file
        |
        v
split updates into batches
        |
        v
run hard-barrier runtime
        |
        v
write measurements and vertex-state dumps
```

The executable expects two command-line arguments:

```bash
./kappa <number_of_cores> <updates_batch_size>
```

For example:

```bash
./kappa 16 100000
```

The first argument controls the number of worker threads. The second controls the maximum number of update records processed per batch.

TODO Fix: The current executable uses hard-coded dataset paths and graph-size constants inside `src/init.cpp`.

## Current runtime dynamic graph analytics algorithms

The current `src/init.cpp` constructs three dynamic computation definitions:

* `computation_pr` for PageRank;
* `computation_sssp` for Single-Source Shortest Paths;
* `computation_wcc` for Weakly Connected Components.

The current active path constructs the graph with `computation_sssp` and then runs SSSP:

```cpp
Digraph g(total_dataset_order, updating, computation_sssp);
run(computation_sssp, updating, &g, updates_array, split_updates);
```

To run PageRank or WCC instead, change both the `Digraph` construction and the `run(...)` call to use the desired computation object consistently.

## Core architecture

Kappa is organised around a small number of core abstractions:

```text
Digraph      in-memory graph topology, vertex state, metadata, and callbacks
Computation  user-defined algorithm callbacks
Updating     graph mutation callbacks
Update       parsed edge update record
Task         executable unit of work
Scheduler    worker threads and task queue
MPMCQueue    bounded multi-producer, multi-consumer task queue
```

The runtime connects these pieces as follows:

```text
Update stream
    |
    v
Updating callbacks mutate Digraph
    |
    v
Connected-component labels are recomputed
    |
    v
Algorithm callbacks are wrapped as Tasks
    |
    v
Scheduler executes Tasks on worker threads
    |
    v
Tasks activate further vertices until propagation stops
```

## Data model

### Basic types

The main graph and runtime types are defined in `src/core/datatypes.h`.

The current implementation uses:

```cpp
typedef uint32_t vertex_id_t;
typedef uint32_t graph_size_t;
typedef uint32_t components_number_t;
typedef double state_t;
```

This means:

* vertex IDs are unsigned 32-bit integers;
* graph sizes and component labels are unsigned 32-bit integers;
* per-vertex algorithm state is a `double`, used to track the status of vertices in graph algorithms to manage traversal, shortest paths, PageRank value, etc.

Update records are stored as triples:

```cpp
typedef std::vector<std::array<vertex_id_t, 3>> raw_update_array_t;
```

The third value in each update triple is interpreted as:

* `1` for edge insertion;
* `0` for edge deletion.

### Vertex layout

Each vertex slot is represented by a `Dvertex` structure in `src/core/vertex.h`.

A `Dvertex` stores:

* incoming neighbours;
* outgoing neighbours;
* in-degree;
* out-degree;
* pointer to the vertex state;
* temporary state pointer placeholder;
* weakly connected-component label;
* logical timestamp fields;
* a `scheduled` flag used for task compaction.

The graph is directed, so incoming and outgoing neighbourhoods are stored separately. Some operations, such as weakly connected-component discovery, deliberately combine both directions and treat the directed graph as undirected for that specific purpose.

### Graph layout

The main graph structure is `Digraph`, defined in `src/core/digraph.h` and implemented in `src/core/digraph.cpp`.

A `Digraph` owns:

* `topology`, a vector of `Dvertex` entries;
* `vertex_index`, a bitmap indicating whether a vertex currently exists;
* `states`, the per-vertex state array;
* `visited_verts`, a bitmap used by traversal code;
* `cc_count`, the current number of weakly connected components;
* `order`, the current number of vertices present in the graph;
* `size`, the current number of edges;
* `updating`, the graph-mutation callback table;
* `computation`, the selected algorithm callback table;
* a global logical timestamp counter.

The graph is preallocated from a maximum vertex count supplied to the constructor. The constructor creates one vertex slot per possible vertex ID, plus one extra slot.

This design makes vertex access direct by ID:

```text
topology[v]  -> graph metadata for vertex v
states[v]    -> algorithm state for vertex v
```

This is simple and fast for datasets whose vertex IDs fit within a known range. It also means the graph assumes vertex IDs are compatible with the preallocated maximum order.

## Graph mutation

Kappa supports two graph mutation operations in the current update stream:

* edge insertion;
* edge deletion.

These are represented by the `Updating` interface:

```cpp
typedef struct Updating {
    void (*edge_insertion)(Digraph*, vertex_id_t, vertex_id_t);
    void (*edge_deletion)(Digraph*, vertex_id_t, vertex_id_t);
} Updating;
```

The default implementations are intentionally thin:

```cpp
Insertions::edge_insertion(g, src, dst) -> g->add_edge(src, dst)
Deletions::edge_deletion(g, src, dst)   -> g->remove_edge(src, dst)
```

Note: A reachability analysis is planned around insertion and deletion handling, but the current behaviour delegates directly to the graph.

### `add_edge`

`Digraph::add_edge(src, dst)` checks whether the edge already exists. If it does not, it:

1. appends `dst` to `src`'s outgoing-neighbour list;
2. appends `src` to `dst`'s incoming-neighbour list;
3. increments the relevant degrees;
4. increments the graph edge count;
5. marks newly seen vertices as present;
6. increments the graph order for newly seen vertices;
7. calls the selected algorithm's `init_state` for newly added vertices.

### `add_edge_populate`

`Digraph::add_edge_populate(src, dst)` is used when loading the initial core graph from disk. It is a faster population path and does not call the algorithm's `init_state` callback.

This matters because the current experiment flow preloads initial vertex states from a separate file after loading the core graph.

### `remove_edge`

`Digraph::remove_edge(src, dst)` checks whether the edge exists. If it does, it:

1. removes `dst` from `src`'s outgoing-neighbour list;
2. removes `src` from `dst`'s incoming-neighbour list;
3. decrements the relevant degrees;
4. decrements the graph edge count;
5. removes a vertex from the presence bitmap if it no longer has any incoming or outgoing edges.

Edge existence checks and neighbour removal currently scan vectors, so these operations are degree-dependent.

## Programming model

Kappa exposes an update-centric programming model through the `Computation` structure:

```cpp
typedef struct Computation {
    void (*init_state)(Digraph*, vertex_id_t);
    void (*on_activate)(Digraph*, vertex_id_t);
    void (*on_add_edge)(Digraph*, vertex_id_t, vertex_id_t);
    void (*on_remove_edge)(Digraph*, vertex_id_t, vertex_id_t);
} Computation;
```

A dynamic graph algorithm is written by implementing these four callbacks.

| Callback                      | Meaning                                                                  |
| ----------------------------- | ------------------------------------------------------------------------ |
| `init_state(g, v)`            | Initialise vertex `v` when it is introduced or explicitly reinitialised. |
| `on_add_edge(g, src, dst)`    | React to an inserted edge after the graph topology has already changed.  |
| `on_remove_edge(g, src, dst)` | React to a deleted edge after the graph topology has already changed.    |
| `on_activate(g, v)`           | Recompute vertex `v` and optionally activate additional vertices.        |

The runtime does not hard-code PageRank, SSSP, or WCC behaviour. It only knows how to invoke these callback slots. Algorithm-specific logic is kept in `src/applications/dynamic/`.

## Task model

Work in Kappa is represented as a `Task`.

The task types are:

```cpp
typedef enum { ON_ACTIVATE, ON_UPDATE, UPDATE } task_type_t;
```

They mean:

| Task type     | Meaning                                                       |
| ------------- | ------------------------------------------------------------- |
| `UPDATE`      | Apply a graph mutation, such as edge insertion or deletion.   |
| `ON_UPDATE`   | Run an algorithm callback for an already-applied edge update. |
| `ON_ACTIVATE` | Run an algorithm callback for an activated vertex.            |

In the current build configuration, graph updates are applied directly by the runtime (`UPDATES_NO_TASKS` defined in `CMakeLists.txt`). This means `UPDATE` tasks are part of the design, but the current compiled path applies updates without wrapping each mutation in a task.

`ON_UPDATE` and `ON_ACTIVATE` tasks are central to the computation phase.

Tasks are allocated from a Boost singleton pool:

```cpp
typedef boost::singleton_pool<Task, sizeof(Task)> task_pool;
```

This avoids repeatedly allocating and freeing small short-lived task objects through the general-purpose allocator.

## Task execution

`Task::execute()` dispatches work based on task type:

* `ON_ACTIVATE` clears the vertex's scheduled flag and calls the stored vertex function;
* `ON_UPDATE` calls the stored edge function;
* `UPDATE` calls the stored graph-update function.

In practice:

```text
ON_ACTIVATE -> computation.on_activate(g, v)
ON_UPDATE   -> computation.on_add_edge(g, src, dst)
            or computation.on_remove_edge(g, src, dst)
UPDATE      -> updating.edge_insertion(g, src, dst)
            or updating.edge_deletion(g, src, dst)
```

After execution, the worker releases the task back to the task pool.

## Task compaction

`Digraph::activate_vertex(v)` implements a simple but important optimisation.

Before submitting an `ON_ACTIVATE` task, it checks whether vertex `v` is already scheduled:

```text
if v is already scheduled:
    do not submit another task
else:
    mark v as scheduled
    submit ON_ACTIVATE task for v
```

This acts as task compaction. If multiple neighbouring vertices try to activate the same vertex before it has run, Kappa keeps only one pending activation task for that vertex.

When the activation task executes, `Task::execute()` clears the scheduled flag before calling the algorithm's `on_activate` callback. The vertex can then be scheduled again later if new work reaches it.

This is particularly relevant for skewed graphs, where high-degree vertices may otherwise receive many redundant activation requests.

## Scheduler and worker threads

The `Scheduler` owns:

* a vector of worker threads;
* a bounded multi-producer, multi-consumer task queue;
* a paused/running flag;
* an active-worker counter;
* worker initialisation logic for regular and NUMA-aware placement.

The scheduler's queue is an `MPMCQueue<Task*>` with a large fixed capacity. The queue implementation uses atomic head/tail counters and cache-line-aligned slots to reduce false sharing.

Worker threads run a loop:

```text
while scheduler is not done:
    if workers are not paused:
        try to pop a task
        if a task is available:
            execute it
            release it
            decrement active-worker count
```

The runtime starts workers for the computation phase and halts them after the barrier completes.

### NUMA-aware placement

The executable initialises workers according to the requested number of cores. For up to 64 cores, it spreads worker initialisation across four NUMA nodes using `init_numa_node(...)`.

`Scheduler::init_numa_node(node_no, no_of_threads)` creates worker threads and pins them to CPU IDs using the pattern:

```text
node_no + (i * 4)
```

This reflects the target evaluation machine layout. It may need adjustment on machines with different CPU/NUMA topology.

### Barrier

The scheduler barrier waits until:

1. the task queue is empty; and
2. no worker is currently executing a task.

This second condition matters because a running task can submit additional work even after the queue has briefly become empty.

## Hard-barrier runtime

The current runtime lives in:

```text
src/core/runtimes/runtime_hard_barrier.cpp
```

It implements a hard barrier between update application and computation.

For each batch, the runtime does the following:

1. Determine the update-line range for the current batch.
2. Apply each edge insertion or deletion to the `Digraph`.
3. Record the updates applied in this batch as `Update` objects.
4. Measure update/ingestion time.
5. Recompute weakly connected-component labels over the updated graph.
6. Dump vertex states after updates.
7. Start scheduler workers.
8. Submit algorithm work caused by each update.
9. Wait for the scheduler barrier.
10. Halt workers.
11. Measure computation time.
12. Append results to `measurements.csv`.

The barrier makes the execution easier to reason about: the graph topology is fully updated before algorithmic computation begins for that batch. The trade-off is that ingestion, graph mutation, and computation do not overlap in the current runtime.

## Connected-component-aware recomputation

Kappa maintains weakly connected-component metadata in the runtime.

The connected-component pass is implemented in:

```text
src/core/connected_components.cpp
```

It uses DFS over `get_in_out_neighborhood(v)`, which combines incoming and outgoing neighbours. This means the component pass treats the directed graph as undirected and computes weakly connected components.

For each component, Kappa records:

* the component label stored on each vertex;
* a global `cc_map` from component label to the list of vertices in that component;
* the current component count.

The runtime recomputes these labels after each batch of graph updates.

This information is especially important for deletions. With the current compile-time strategy `RECOMPUTE_ONLY_BREAKAGES_DEL`, when an edge deletion causes `src` and `dst` to end up in different weakly connected components, Kappa schedules recomputation over the affected resulting components. If the deleted edge does not split the component, the runtime delegates to the algorithm's `on_remove_edge` callback instead.

This should not be confused with the user-level dynamic WCC algorithm. The runtime-level component pass is infrastructure used to guide recomputation. The WCC algorithm under `src/applications/dynamic/wcc.cpp` is one possible user computation implemented on top of the update-centric callback interface.

## Deletion strategies

`CMakeLists.txt` defines:

```cmake
-DUPDATES_NO_TASKS
-DRECOMPUTE_ONLY_BREAKAGES_DEL
```

The relevant deletion strategies in the runtime are:

| Strategy                       | Behaviour                                                                                                                   |
| ------------------------------ | --------------------------------------------------------------------------------------------------------------------------- |
| `RECOMPUTE_ALL_DEL`            | Reinitialise and activate vertices in the source and destination components for every deletion case handled by that branch. |
| `RECOMPUTE_ONLY_BREAKAGES_DEL` | Recompute affected components only when deletion separates `src` and `dst` into different weakly connected components.      |
| `RECOMPUTE_NOTHING_DEL`        | Do not perform component-level recomputation; call the algorithm's remove-edge callback directly.                           |

The default compiled strategy is `RECOMPUTE_ONLY_BREAKAGES_DEL`.

## Implemented dynamic algorithms

Dynamic algorithms live under:

```text
src/applications/dynamic/
```

Each algorithm implements the `Computation` callbacks.

### PageRank

Files:

```text
src/applications/dynamic/pagerank.cpp
src/applications/dynamic/pagerank.h
```

PageRank uses:

```cpp
DAMPING_FACTOR = 0.85
EPSILON = 0.0001
```

The algorithm works as follows:

* `init_state` sets the vertex PageRank to `1.0 / g->get_max_order()`.
* `on_add_edge(src, dst)` activates all current out-neighbours of `src` because the contribution distributed by `src` has changed.
* `on_remove_edge(src, dst)` activates all current out-neighbours of `src` and also activates `dst`.
* `on_activate(v)` recomputes `v` by reading the states of its in-neighbours and dividing each neighbour's state by that neighbour's out-degree.
* If the PageRank value changes by at least `EPSILON`, `v` activates its out-neighbours.

This is a pull-style implementation: a vertex reads neighbour state directly from the graph rather than receiving messages.

### Single-Source Shortest Paths

Files:

```text
src/applications/dynamic/sssp.cpp
src/applications/dynamic/sssp.h
```

The current SSSP implementation is directed and unweighted. The source vertex is currently a compile-time constant:

```cpp
SOURCE = 1
```

The algorithm works as follows:

* vertex state represents distance from the source;
* `get_min_distance(v)` scans the in-neighbours of `v` and returns the smallest stored neighbour distance;
* `init_state(v)` sets the source to distance `0` and other vertices to `min_in_neighbour_distance + 1`;
* `on_add_edge(src, dst)` checks whether the new edge creates a shorter path to `dst`;
* if it does, `dst` is updated and its out-neighbours are activated;
* `on_activate(v)` recomputes whether `v` can be improved from its in-neighbours and propagates improvements to out-neighbours;
* `on_remove_edge(src, dst)` activates `dst` if the removed edge appears to have been on the current shortest path to `dst`.

Because deletions can invalidate paths in a way that is harder than insertions, the runtime's connected-component-aware recomputation strategy is important for deletion-heavy experiments.

### Weakly Connected Components

Files:

```text
src/applications/dynamic/wcc.cpp
src/applications/dynamic/wcc.h
```

The WCC algorithm uses label propagation over the combined incoming and outgoing neighbourhood.

The algorithm works as follows:

* each vertex state acts as a component label;
* `get_max_label(v)` scans both in-neighbours and out-neighbours and returns the maximum neighbouring label;
* `init_state(v)` sets the vertex state to the maximum label in its neighbourhood;
* `on_add_edge(src, dst)` checks whether the endpoints currently have different labels;
* if they do, it assigns the larger label to the endpoint with the smaller label and activates its neighbours;
* `on_remove_edge(src, dst)` pessimistically gives both endpoints fresh labels and activates their neighbours;
* `on_activate(v)` pulls the maximum neighbour label, updates `v`, and propagates if the label changed.

The runtime-level connected-component pass and the user-level WCC algorithm are related but distinct: one is infrastructure for affected-region recomputation; the other is an algorithm expressed through Kappa's programming model.

## Static algorithms and utilities

Static algorithms live under:

```text
src/applications/static/
```

The repository includes static or baseline implementations for:

* Breadth-First Search;
* Depth-First Search;
* Connected Components;
* PageRank;
* Single-Source Shortest Paths;
* No-op computation.

These are useful for comparison, testing, and benchmarking, but the main runtime path is focused on dynamic algorithms.

## Input format

Kappa reads edge-list style files.

### Core graph

The core graph is read as two-column edge-list data:

```text
src dst
src dst
src dst
```

The loader memory-maps the file, reads each line, splits on the first space, and calls `add_edge_populate(src, dst)`.

### Update stream

The update stream is read as three-column edge-list data:

```text
src dst update_type
src dst update_type
src dst update_type
```

Where:

```text
1 = edge insertion
0 = edge deletion
```

The update loader memory-maps the file, splits each line on spaces, and stores updates in a `raw_update_array_t`.

### Preloaded vertex states

The executable preloads vertex states from a separate file. The loader reads a configurable line range and separator, then parses:

```text
vertex_id state
```

This is especially relevant because the initial core graph is loaded through `add_edge_populate`, which does not call algorithm initialisation.

## Batching

The update stream is split into batches by `dataset_to_batches(...)`.

The function receives:

* beginning line;
* end line;
* total number of lines;
* chunk size.

It returns a vector of line markers. Consecutive markers define the range of update records processed by each runtime batch.

The runtime then processes:

```text
updates[batch_start - 1] ... updates[batch_end - 2]
```

This indexing reflects the line-marker convention used by the batching utility.

## Output files

The executable/runtime may produce:

```text
measurements.csv
vertex-states-dump-core.txt
vertex-states-dump-after-updates.txt
vertex-states-dump-end.txt
```

`measurements.csv` records graph order, graph size, update/ingestion time, and computation time for each batch.

The vertex-state dumps are useful for validating algorithm output and comparing state before updates, after graph mutation, and after computation.

## Repository layout

```text
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── data/
│   └── datasets_constants.h
├── doc/
│   └── campesel-msc-report.pdf
├── src/
│   ├── init.cpp
│   ├── applications/
│   │   ├── dynamic/
│   │   │   ├── pagerank.cpp / pagerank.h
│   │   │   ├── sssp.cpp / sssp.h
│   │   │   └── wcc.cpp / wcc.h
│   │   └── static/
│   ├── core/
│   │   ├── connected_components.cpp / connected_components.h
│   │   ├── datatypes.h
│   │   ├── digraph.cpp / digraph.h
│   │   ├── global_scheduler.h
│   │   ├── insertions.cpp / insertions.h
│   │   ├── deletions.cpp / deletions.h
│   │   ├── mpmc_queue.h
│   │   ├── scheduler.cpp / scheduler.h
│   │   ├── task.cpp / task.h
│   │   ├── update.cpp / update.h
│   │   ├── vertex.h
│   │   ├── runtimes/
│   │   └── utils/
│   ├── experiments/
│   └── preprocessor/
├── test/
│   └── tests_main.cpp
└── utils/
    ├── plot.py
    └── run_experiments.sh
```

## Requirements

Kappa is written in C++17 and built with CMake.

You will need:

* a C++17-compatible compiler;
* CMake 3.8 or newer;
* Boost, including `system`, `filesystem`, and `iostreams`;
* POSIX threads;
* GoogleTest, for the test target.

The current CMake configuration contains machine-specific Boost paths:

```cmake
set(Boost_INCLUDE_DIR /opt/boost/include)
set(Boost_LIBRARY_DIR /opt/boost/lib)
```

These may need to be changed for another environment.

## Building

From the repository root:

```bash
git clone https://github.com/domargan/kappa.git
cd kappa
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j
```

This builds the main executable:

```text
kappa
```

The CMake file also defines a GoogleTest target:

```bash
cmake --build . --target tests
```

Depending on the branch state, some tests may need updating to match the current `Digraph` API.

## Running an experiment

A typical experiment is:

1. Prepare a core graph edge-list file.
2. Prepare an update stream file with insertions and deletions.
3. Prepare precomputed vertex states if required by the selected algorithm.
4. Update dataset paths in `src/init.cpp`.
5. Update graph-size constants or selected dataset constants.
6. Select the desired dynamic computation in `src/init.cpp`.
7. Build in release mode.
8. Run `kappa` with a chosen worker count and batch size.

Example:

```bash
./kappa 16 100000
```

## Implementation notes

This codebase reflects an experimental systems prototype. Some implementation details are deliberate research trade-offs.

Important considerations:

* the main executable is configured by editing `src/init.cpp`;
* dataset paths are currently hard-coded;
* Boost paths are hard-coded in `CMakeLists.txt`;
* the active runtime strategy is controlled by compile-time flags;
* the current graph is directed and unweighted;
* update records currently represent edge insertions and deletions only;
* vertex state is currently fixed as `double`;
* the graph is preallocated against a known maximum vertex ID range;
* neighbour lists are stored as vectors, so edge existence and deletion are degree-dependent;
* weakly connected components are recomputed after every update batch rather than maintained incrementally;
* the task-based graph-update path exists in the runtime but is not the active path when `UPDATES_NO_TASKS` is defined;
* NUMA-aware worker placement is implemented, but NUMA-aware graph data placement is not a first-class feature.

## Possible future work

TODO includes:

* replacing hard-coded experiment configuration with command-line or config-file options;
* modernising the CMake dependency setup;
* making the selected algorithm configurable at runtime;
* supporting user-defined vertex state types;
* adding weighted-edge support;
* adding first-class vertex update operations;
* adding first-class undirected graph support;
* maintaining connected components incrementally rather than recomputing them after every batch;
* softening the hard barrier between update application and computation;
* overlapping ingestion, mutation, and computation;
* revisiting logical timestamps and dependency checks for more asynchronous execution;
* improving test coverage around graph mutation, scheduling, and deletion-heavy workloads;
* adapting NUMA placement logic to arbitrary machine topologies.

## Licence

Kappa is released under the MIT Licence. See [`LICENSE`](LICENSE) for details.

**Authors:** Dom Margan, Luca Campese
