#include <bits/stdc++.h>
using namespace std;

#define ll long long
const ll INF = 1e18;

vector<ll> dist;
vector<vector<pair<ll, ll>>> adj;

void displayGraph(){
    ll n = adj.size();
    cout << "\nAdjacency List:" << endl;
    for (ll i = 1; i < n; i++) {
        cout << i << " -> ";
        for (auto [v, wt] : adj[i]){
            cout << "(" << v << ", " << wt << ") ";
        }
        cout << endl;
    }
}

void dijkstra(ll src) {
    ll n = adj.size();
    dist.assign(n, INF);
    dist[src] = 0;

    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto [v, wt] : adj[u]) {
            if (dist[v] > d + wt) {
                dist[v] = d + wt;
                pq.push({dist[v], v});
            }
        }
    }

    cout << "\nSingle Source Shortest Paths (Source = " << src << "):" << endl;
    for (ll i = 1; i < n; i++){
        if (dist[i] == INF) cout << i << " : INF" << endl;
        else cout << i << " : " << dist[i] << endl;
    }
}

void inputGraph(){
    ll n, m;
    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> m;
    adj.clear();
    adj.resize(n+1);
    cout << "Enter edges (u v weight):" << endl;
    for (ll i = 0; i < m; i++) {
        ll u, v, wt;
        cin >> u >> v >> wt;
        adj[u].push_back({v, wt});
        adj[v].push_back({u, wt});
    }

    cout << "Graph input successful!" << endl;
}

int main() {
    int choice;
    while (true) {
        cout << "\n====== MENU ======" << endl;
        cout << "1. Input Graph" << endl;
        cout << "2. Display Graph" << endl;
        cout << "3. Run Dijkstra" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            inputGraph();
        }
        else if (choice == 2) {
            if (adj.empty())
                cout << "Graph is empty. Input graph first." << endl;
            else displayGraph();
        }
        else if (choice == 3) {
            if (adj.empty()){
                cout << "Graph is empty. Input graph first." << endl;
            }
            else{
                ll src;
                cout << "Enter source vertex: ";
                cin >> src;
                dijkstra(src);
            }
        }
        else if (choice == 4){
            cout << "Exiting program..." << endl;
            break;
        }
        else{
            cout << "Invalid choice. Try again." << endl;
        }
    }
    return 0;
}
