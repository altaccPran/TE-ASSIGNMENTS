#include <bits/stdc++.h>
using namespace std;

#define ll long long

vector<vector<pair<ll, ll>>> adj;

/* -------- DSU (Union By Rank) -------- */
class DSU {
    vector<ll> par, rnk;

public:
    DSU(ll n) {
        par.resize(n + 1);
        rnk.resize(n + 1, 0);

        for (ll i = 0; i <= n; i++)
            par[i] = i;
    }

    ll find(ll x) {
        if (par[x] == x) return x;
        return par[x] = find(par[x]);
    }

    void unite(ll a, ll b) {
        a = find(a);
        b = find(b);

        if (a == b) return;

        if (rnk[a] < rnk[b])
            par[a] = b;
        else if (rnk[b] < rnk[a])
            par[b] = a;
        else {
            par[b] = a;
            rnk[a]++;
        }
    }
};

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

/* -------- Kruskal MST -------- */
void kruskalMST() {
    ll n = adj.size() - 1;

    vector<pair<ll, pair<ll, ll>>> edges;

    // Collect edges
    for (ll u = 1; u <= n; u++) {
        for (auto [v, w] : adj[u]) {
            if (u < v) { // avoid duplicates
                edges.push_back({w, {u, v}});
            }
        }
    }

    sort(edges.begin(), edges.end());

    DSU dsu(n);

    ll total = 0;
    vector<pair<ll, pair<ll, ll>>> mst;

    // Build MST
    for (auto e : edges) {
        ll w = e.first;
        ll u = e.second.first;
        ll v = e.second.second;

        if (dsu.find(u) != dsu.find(v)) {
            total += w;
            mst.push_back(e);
            dsu.unite(u, v);
        }
    }

    // Print MST
    cout << "\nEdges in MST:\n";
    for (auto e : mst) {
        cout << e.second.first << " - " << e.second.second
             << "  Weight = " << e.first << endl;
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
        cout << "3. Find MST (Kruskal)\n";
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
            if (adj.empty())
                cout << "Graph is empty. Input graph first.\n";
            else
                kruskalMST();
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
