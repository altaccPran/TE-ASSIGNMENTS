#include <iostream>
#include <cstring>

using namespace std;

#define MAX 50

struct Service {
    int id;
    char name[32];
};

class DistributedSystem {
private:
    Service services[MAX];
    int adj[MAX][MAX];
    int serviceCount;

public:
    DistributedSystem() {
        serviceCount = 0;
        memset(adj, 0, sizeof(adj));
    }

    // -----------------------------
    // Add Service Node
    // -----------------------------
    void addService() {
        if (serviceCount >= MAX) {
            cout << "Service limit reached\n";
            return;
        }

        services[serviceCount].id = serviceCount;
        cout << "Enter service name: ";
        cin >> services[serviceCount].name;

        serviceCount++;
        cout << "Service added successfully\n";
    }

    // -----------------------------
    // Add Dependency (Directed Edge)
    // -----------------------------
    void addDependency() {
        int from, to;
        cout << "Enter FROM service ID: ";
        cin >> from;
        cout << "Enter TO service ID: ";
        cin >> to;

        if (from >= serviceCount || to >= serviceCount) {
            cout << "Invalid service ID\n";
            return;
        }

        adj[from][to] = 1;
        cout << "Dependency added: "
             << services[from].name << " -> "
             << services[to].name << "\n";
    }

    // -----------------------------
    // BFS Service Discovery
    // -----------------------------
    void bfs(int start) {
        if (start >= serviceCount) {
            cout << "Invalid start service\n";
            return;
        }

        bool visited[MAX] = {false};
        int queue[MAX];
        int front = 0, rear = 0;

        visited[start] = true;
        queue[rear++] = start;

        cout << "\n[BFS] Service Discovery:\n";

        while (front < rear) {
            int curr = queue[front++];
            cout << "Discovered: " << services[curr].name << "\n";

            for (int i = 0; i < serviceCount; i++) {
                if (adj[curr][i] && !visited[i]) {
                    visited[i] = true;
                    queue[rear++] = i;
                }
            }
        }
    }

    // -----------------------------
    // DFS Dependency Analysis
    // -----------------------------
    void dfsUtil(int node, bool visited[]) {
        visited[node] = true;
        cout << "Analyzing: " << services[node].name << "\n";

        for (int i = 0; i < serviceCount; i++) {
            if (adj[node][i] && !visited[i]) {
                dfsUtil(i, visited);
            }
        }
    }

    void dfs(int start) {
        if (start >= serviceCount) {
            cout << "Invalid start service\n";
            return;
        }

        bool visited[MAX] = {false};

        cout << "\n[DFS] Dependency Analysis:\n";
        dfsUtil(start, visited);
    }

    // -----------------------------
    // Cycle Detection (DFS + Stack)
    // -----------------------------
    bool cycleUtil(int node, bool visited[], bool recStack[]) {
        visited[node] = true;
        recStack[node] = true;

        for (int i = 0; i < serviceCount; i++) {
            if (adj[node][i]) {
                if (!visited[i] && cycleUtil(i, visited, recStack))
                    return true;
                else if (recStack[i])
                    return true;
            }
        }

        recStack[node] = false;
        return false;
    }

    void detectCycle() {
        bool visited[MAX] = {false};
        bool recStack[MAX] = {false};

        for (int i = 0; i < serviceCount; i++) {
            if (!visited[i]) {
                if (cycleUtil(i, visited, recStack)) {
                    cout << "\nCycle detected in dependencies!\n";
                    return;
                }
            }
        }

        cout << "\nNo dependency cycles detected\n";
    }

    // -----------------------------
    // Show Adjacency Matrix
    // -----------------------------
    void showGraph() {
        cout << "\nAdjacency Matrix:\n  ";
        for (int i = 0; i < serviceCount; i++)
            cout << i << " ";
        cout << "\n";

        for (int i = 0; i < serviceCount; i++) {
            cout << i << " ";
            for (int j = 0; j < serviceCount; j++) {
                cout << adj[i][j] << " ";
            }
            cout << "\n";
        }
    }

    // -----------------------------
    // Menu
    // -----------------------------
    void menu() {
        int choice, start;

        while (true) {
            cout << "\n========== Distributed System Graph ==========\n";
            cout << "1. Add Service\n";
            cout << "2. Add Dependency\n";
            cout << "3. BFS Service Discovery\n";
            cout << "4. DFS Dependency Analysis\n";
            cout << "5. Detect Dependency Cycles\n";
            cout << "6. Show Adjacency Matrix\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addService();
                    break;
                case 2:
                    addDependency();
                    break;
                case 3:
                    cout << "Enter start service ID: ";
                    cin >> start;
                    bfs(start);
                    break;
                case 4:
                    cout << "Enter start service ID: ";
                    cin >> start;
                    dfs(start);
                    break;
                case 5:
                    detectCycle();
                    break;
                case 6:
                    showGraph();
                    break;
                case 0:
                    cout << "Exiting...\n";
                    return;
                default:
                    cout << "Invalid choice\n";
            }
        }
    }
};

// -----------------------------
// Main
// -----------------------------
int main() {
    DistributedSystem system;
    system.menu();
    return 0;
}
