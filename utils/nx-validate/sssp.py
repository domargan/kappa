import sys
import networkx as nx

INPUT_GRAPH = sys.argv[1]

if __name__ == '__main__':
    g = nx.read_edgelist(INPUT_GRAPH,
                         create_using=nx.DiGraph(),
                         nodetype=int)
    res = nx.single_source_shortest_path(g, 1)

    res_with_int_ids = {}

    for i in g:
        if i in res:
            res_with_int_ids[i] = len(res[i]) - 1
        else:
            res_with_int_ids[i] = 'inf'

    with open('nx-sssp.txt', 'w') as out:
        for vertex, state in res_with_int_ids.items():
            print(vertex, state, file=out)
