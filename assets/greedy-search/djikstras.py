import sys

def dijkstra_shortest_path(graph_matrix, start_node, num_vertices):
    # The output array. dist[i] will hold the shortest distance from start_node to i
    dist = [sys.maxsize] * num_vertices
    dist[start_node] = 0
    
    # shortest_path_set[i] will be True if vertex i is included in shortest path tree
    shortest_path_set = [False] * num_vertices

    for _ in range(num_vertices):
        # 1. GREEDY CHOICE: Pick the minimum distance vertex from the set of vertices not yet processed.
        min_dist = sys.maxsize
        u = -1
        
        for v in range(num_vertices):
            if dist[v] < min_dist and shortest_path_set[v] == False:
                min_dist = dist[v]
                u = v

        # 2. Put the minimum distance vertex in the shortest path tree
        shortest_path_set[u] = True

        # 3. Update dist value of the adjacent vertices of the picked vertex.
        for v in range(num_vertices):
            # Update dist[v] only if:
            # - It is not in the shortest path set
            # - There is an edge from u to v
            # - Total weight of path from src to v through u is smaller than current value of dist[v]
            if (graph_matrix[u][v] > 0 and 
                shortest_path_set[v] == False and 
                dist[v] > dist[u] + graph_matrix[u][v]):
                
                dist[v] = dist[u] + graph_matrix[u][v]

    # Print the calculated shortest distances
    print(f"Shortest distances from Node {start_node}:")
    print("Vertex \t Distance from Source")
    for node in range(num_vertices):
        print(f"{node} \t {dist[node]}")

if __name__ == '__main__':
    print("--- Dijkstra's Algorithm (Single-Source Shortest Path) ---")
    # 9 vertices. 0 means no edge.
    graph = [
        [0, 4, 0, 0, 0, 0, 0, 8, 0],
        [4, 0, 8, 0, 0, 0, 0, 11, 0],
        [0, 8, 0, 7, 0, 4, 0, 0, 2],
        [0, 0, 7, 0, 9, 14, 0, 0, 0],
        [0, 0, 0, 9, 0, 10, 0, 0, 0],
        [0, 0, 4, 14, 10, 0, 2, 0, 0],
        [0, 0, 0, 0, 0, 2, 0, 1, 6],
        [8, 11, 0, 0, 0, 0, 1, 0, 7],
        [0, 0, 2, 0, 0, 0, 6, 7, 0]
    ]
    # Find shortest paths from Node 0
    dijkstra_shortest_path(graph, 0, 9)