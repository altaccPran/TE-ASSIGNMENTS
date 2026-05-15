class Graph:
    def __init__(self, vertices):
        self.V = vertices 
        self.edges = [] 
        
    def add_edge(self, u, v, weight):
        self.edges.append([u, v, weight])

    # A utility function to find the set of an element i (Union-Find)
    def find(self, parent, i):
        if parent[i] == i:
            return i
        return self.find(parent, parent[i])

    # A utility function to do union of two sets
    def union(self, parent, rank, x, y):
        xroot = self.find(parent, x)
        yroot = self.find(parent, y)

        # Attach smaller rank tree under root of high rank tree
        if rank[xroot] < rank[yroot]:
            parent[xroot] = yroot
        elif rank[xroot] > rank[yroot]:
            parent[yroot] = xroot
        else:
            parent[yroot] = xroot
            rank[xroot] += 1

    def kruskal_mst(self):
        result = [] # This will store the resultant MST
        i = 0 # Index variable used for sorted edges
        e = 0 # Index variable used for result[]
        
        # 1. Sort all the edges in non-decreasing order of their weight (Greedy)
        self.edges = sorted(self.edges, key=lambda item: item[2])
        
        parent = []
        rank = []
        
        # Create V subsets with single elements
        for node in range(self.V):
            parent.append(node)
            rank.append(0)
            
        # Number of edges to be taken is equal to V-1
        while e < self.V - 1 and i < len(self.edges):
            # 2. Pick the smallest edge. 
            u, v, w = self.edges[i]
            i = i + 1
            
            x = self.find(parent, u)
            y = self.find(parent, v)
            
            # If including this edge does't cause cycle, include it
            if x != y:
                e = e + 1
                result.append([u, v, w])
                self.union(parent, rank, x, y)
            # Else discard the edge
            
        return result

if __name__ == '__main__':
    print("--- Kruskal's MST ---")
    # Create a graph with 4 vertices (0, 1, 2, 3)
    g = Graph(4)
    g.add_edge(0, 1, 10)
    g.add_edge(0, 2, 6)
    g.add_edge(0, 3, 5)
    g.add_edge(1, 3, 15)
    g.add_edge(2, 3, 4)

    mst = g.kruskal_mst()
    
    print("Edges in the constructed MST:")
    total_cost = 0
    for u, v, weight in mst:
        total_cost += weight
        print(f"{u} -- {v} == {weight}")
    print(f"Minimum Spanning Tree Cost: {total_cost}")