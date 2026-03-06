#include <bits/stdc++.h>
using namespace std;

#define ll long long
const ll INF = 1e18;

vector<vector<pair<ll, ll>>> adj;

/* -------- Display Graph -------- */
void displayGraph() {
    ll n = adj.size();
    cout << "\nAdjacency List:\n";

    for (ll i = 1; i < n; i++) {
        cout << i << " -> ";
        for (auto [v, w] : adj[i]) {
            cout << "(" << v << ", " << w << ") ";
        }
        cout << endl;
    }
}

/* -------- Input Graph -------- */
void inputGraph() {
    ll n, m;
    cout << "Enter number of vertices: ";
    cin >> n;

    cout << "Enter number of edges: ";
    cin >> m;

    adj.clear();
    adj.resize(n + 1);

    cout << "Enter edges (u v weight):\n";
    for (ll i = 0; i < m; i++) {
        ll u, v, w;
        cin >> u >> v >> w;

        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    cout << "Graph input successful!\n";
}

/* -------- Prim’s MST -------- */
void primMST(ll src) {
    ll n = adj.size() - 1;

    vector<ll> key(n + 1, INF);
    vector<ll> par(n + 1, -1);
    vector<bool> vis(n + 1, false);

    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;

    key[src] = 0;
    pq.push({0, src});

    ll total = 0;

    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        if (vis[u]) continue;

        vis[u] = true;
        total += w;

        for (auto [v, wt] : adj[u]) {
            if (!vis[v] && wt < key[v]) {
                key[v] = wt;
                par[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    cout << "\nEdges in MST (Prim's Algorithm):\n";
    for (ll i = 1; i <= n; i++) {
        if (par[i] != -1) {
            cout << par[i] << " - " << i
                 << "  Weight = " << key[i] << endl;
        }
    }

    cout << "\nTotal MST Weight = " << total << endl;
}

/* -------- Main Menu -------- */
int main() {
    int choice;

    while (true) {
        cout << "\n====== MENU ======\n";
        cout << "1. Input Graph\n";
        cout << "2. Display Graph\n";
        cout << "3. Find MST (Prim)\n";
        cout << "4. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            inputGraph();
        }
        else if (choice == 2) {
            if (adj.empty())
                cout << "Graph is empty. Input graph first.\n";
            else
                displayGraph();
        }
        else if (choice == 3) {
            if (adj.empty()) {
                cout << "Graph is empty. Input graph first.\n";
            }
            else {
                ll src;
                cout << "Enter starting vertex: ";
                cin >> src;
                primMST(src);
            }
        }
        else if (choice == 4) {
            cout << "Exiting program...\n";
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}