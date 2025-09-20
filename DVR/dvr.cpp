#include <iostream>
#include <iomanip>   // for std::setw
using namespace std;

struct node {
    unsigned dist[6];
    unsigned from[6];
} DVR[10];

int main() {
    cout << "\n================ Distance Vector Routing ================\n";
    int costmat[6][6];
    int nodes, i, j, k;

    cout << "Enter the number of nodes: ";
    cin >> nodes;

    cout << "\nEnter the cost matrix:\n";
    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            cout << "Cost from " << i + 1 << " to " << j + 1 << " : ";
            cin >> costmat[i][j];
            if (i == j) costmat[i][j] = 0;           // self cost is zero
            DVR[i].dist[j] = costmat[i][j];
            DVR[i].from[j] = j;
        }
    }

    // Distance Vector update
    for (i = 0; i < nodes; i++)
        for (j = i + 1; j < nodes; j++)
            for (k = 0; k < nodes; k++)
                if (DVR[i].dist[j] > costmat[i][k] + DVR[k].dist[j]) {
                    DVR[i].dist[j] = DVR[i].dist[k] + DVR[k].dist[j];
                    DVR[j].dist[i] = DVR[i].dist[j];
                    DVR[i].from[j] = k;
                    DVR[j].from[i] = k;
                }

    // Neat formatted output
    cout << "\n------------------ Routing Tables ------------------\n";
    for (i = 0; i < nodes; i++) {
        cout << "\nRouter " << i + 1 << " Routing Table:\n";
        cout << left << setw(12) << "Destination"
             << setw(12) << "Next Hop"
             << setw(10) << "Distance" << "\n";
        cout << string(34, '-') << "\n";

        for (j = 0; j < nodes; j++) {
            cout << left << setw(12) << (j + 1)
                 << setw(12) << (DVR[i].from[j] + 1)
                 << setw(10) << DVR[i].dist[j] << "\n";
        }
    }

    cout << "\n=====================================================\n";
    return 0;
}
