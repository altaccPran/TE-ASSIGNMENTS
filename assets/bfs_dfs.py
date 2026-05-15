# 1. Function to build the graph from user input
def build_graph():
    graph = {}
    
    # Get the total number of edges
    try:
        num_edges = int(input("Enter the number of edges: "))
    except ValueError:
        print("Invalid input. Please enter an integer.")
        return graph

    print("Enter each edge separated by a space (e.g., 'A B'):")
    
    for i in range(num_edges):
        edge = input(f"Edge {i+1}: ").strip().split()
        
        # Ensure the user entered exactly two nodes
        if len(edge) != 2:
            print("Invalid format. Please enter two nodes.")
            continue
            
        u, v = edge[0], edge[1]
        
        # Initialize nodes in the dictionary if they don't exist yet
        if u not in graph:
            graph[u] = []
        if v not in graph:
            graph[v] = []
            
        # Since it's an undirected graph, add the connection both ways
        graph[u].append(v)
        graph[v].append(u)
        
    return graph

# 2. Recursive Depth-First Search (DFS)
def dfs_recursive(graph, node, visited=None):
    if visited is None:
        visited = set()
    
    if node not in visited:
        print(node, end=" ")
        visited.add(node)
        
        # Recursively visit all unvisited neighbors
        for neighbor in graph.get(node, []): # Use .get() safely in case of disconnected nodes
            dfs_recursive(graph, neighbor, visited)

# 3. Breadth-First Search (BFS)
def bfs(graph, start_node):
    if start_node not in graph:
        return
        
    visited = set()
    queue = [start_node]
    visited.add(start_node)
    
    while queue:
        node = queue.pop(0) 
        print(node, end=" ")
        
        for neighbor in graph.get(node, []):
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append(neighbor)

# --- Main Program Execution ---
print("--- Graph Input ---")
my_graph = build_graph()

if my_graph:
    print("\nGenerated Adjacency List:")
    for node, neighbors in my_graph.items():
        print(f"{node}: {neighbors}")

    # Ask the user where to start the search
    start_node = input("\nEnter the starting node for traversal: ").strip()

    if start_node in my_graph:
        print("\nRecursive DFS Traversal:")
        dfs_recursive(my_graph, start_node) 

        print("\n\nBFS Traversal:")
        bfs(my_graph, start_node)
        print() # Print a final newline
    else:
        print(f"\nNode '{start_node}' does not exist in the graph.")
else:
    print("Graph is empty. Exiting.")
