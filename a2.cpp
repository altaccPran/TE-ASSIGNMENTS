#include<bits/stdc++.h>
using namespace std;

int heuristic(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int diff = 0;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[0].size(); j++) {
            if (a[i][j] != b[i][j]) diff++;
        }
    }
    return diff;
}

vector<vector<int>> makemove(vector<vector<int>> a, pair<int,int> move, int x, int y) {
    int nx = x + move.first;
    int ny = y + move.second;

    if (nx >= 0 && nx < a.size() && ny >= 0 && ny < a[0].size()) {
        a[x][y] = a[nx][ny];
        a[nx][ny] = -1;
    }
    return a;
}

void printBoard(const vector<vector<int>>& a) {
    for (auto &row : a) {
        for (auto v : row) {
            if (v == -1) cout << "_ ";
            else cout << v << " ";
        }
        cout << "\n";
    }
}

void printStateInfo(int g, int h) {
    cout << "g(n) = " << g << "  h(n) = " << h << "  f(n) = " << g + h << "\n\n";
}

struct Node {
    vector<vector<int>> board;
    int g, h;
};

struct cmp {
    bool operator()(Node a, Node b) {
        return (a.g + a.h) > (b.g + b.h);
    }
};

void A_star(vector<vector<int>> start, vector<vector<int>> goal) {

    priority_queue<Node, vector<Node>, cmp> pq;
    set<vector<vector<int>>> vis;

    Node s;
    s.board = start;
    s.g = 0;
    s.h = heuristic(start, goal);

    pq.push(s);

    pair<int,int> dir[4] = {{-1,0},{1,0},{0,-1},{0,1}};

    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();

        if (vis.count(cur.board)) continue;
        vis.insert(cur.board);

        printBoard(cur.board);
        printStateInfo(cur.g, cur.h);

        if (cur.board == goal) {
            cout << "Goal reached\n";
            return;
        }

        int x = 0, y = 0;
        for (int i = 0; i < cur.board.size(); i++) {
            for (int j = 0; j < cur.board[0].size(); j++) {
                if (cur.board[i][j] == -1) {
                    x = i; y = j;
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            vector<vector<int>> nxt = makemove(cur.board, dir[i], x, y);

            if (nxt == cur.board) continue;
            if (vis.count(nxt)) continue;

            Node temp;
            temp.board = nxt;
            temp.g = cur.g + 1;
            temp.h = heuristic(nxt, goal);

            pq.push(temp);
        }
    }

    cout << "No solution found\n";
}

int main() {
    while (true) {
        int n;
        cout << "Enter N (0 to exit): ";
        cin >> n;

        if (n == 0) break;

        vector<vector<int>> start(n, vector<int>(n));
        vector<vector<int>> goal(n, vector<int>(n));

        cout << "Enter initial state (-1 for blank):\n";
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++) cin >> start[i][j];
        }

        cout << "Enter goal state (-1 for blank):\n";
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++) cin >> goal[i][j];
        }

        A_star(start, goal);
    }

    return 0;
}
