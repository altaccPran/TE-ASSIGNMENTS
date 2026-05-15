import sys

def prim_mst(graph_matrix, num_vertices):
    # Array to store the constructed MST (stores the parent of each node)
    parent = [None] * num_vertices
    # Key values used to pick minimum weight edge
    key = [sys.maxsize] * num_vertices
    # To represent set of vertices included in MST
    mst_set = [False] * num_vertices

    # Always include first 1st vertex in MST.
    key[0] = 0        # Make key 0 so that this vertex is picked as first vertex
    parent[0] = -1    # First node is always root of MST

    for _ in range(num_vertices):
        # 1. GREEDY CHOICE: Pick the minimum key vertex from the set of vertices not yet in MST
        min_val = sys.maxsize
        u = -1
        
        for v in range(num_vertices):
            if key[v] < min_val and mst_set[v] == False:
                min_val = key[v]
                u = v

        # 2. Put the minimum distance vertex in the MST
        mst_set[u] = True

        # 3. Update key value and parent index of the adjacent vertices
        for v in range(num_vertices):
            # graph_matrix[u][v] is non-zero for adjacent vertices
            # mst_set[v] is false for vertices not yet included in MST
            # Update the key only if graph_matrix[u][v] is smaller than current key
            if graph_matrix[u][v] > 0 and mst_set[v] == False and key[v] > graph_matrix[u][v]:
                key[v] = graph_matrix[u][v]
                parent[v] = u

    # Print the constructed MST
    print("Edge \tWeight")
    total_cost = 0
    for i in range(1, num_vertices):
        weight = graph_matrix[i][parent[i]]
        total_cost += weight
        print(f"{parent[i]} - {i} \t  {weight}")
    print(f"Total MST Cost: {total_cost}")

if __name__ == '__main__':
    print("--- Prim's Algorithm (MST) ---")
    # 5 vertices. 0 means no edge.
    graph = [
        [0, 2, 0, 6, 0],
        [2, 0, 3, 8, 5],
        [0, 3, 0, 0, 7],
        [6, 8, 0, 0, 9],
        [0, 5, 7, 9, 0]
    ]
    prim_mst(graph, 5)