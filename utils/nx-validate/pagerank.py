import sys
import networkx as nx

INPUT_GRAPH = sys.argv[1]

if __name__ == '__main__':
    g = nx.read_edgelist(INPUT_GRAPH,
                         create_using=nx.DiGraph(),
                         nodetype=int)
    res = nx.pagerank(g, tol=0.0001)

    res_with_int_ids = {}

    for vertex, state in res.items():
        res_with_int_ids[vertex] = state

    with open('nx-pr.txt', 'w') as out:
        for vertex, state in res_with_int_ids.items():
            print(vertex, state, file=out)
