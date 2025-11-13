#include <iostream>
using namespace std;

/* -------------------- BULLY ALGORITHM -------------------- */
void bullyElection() {
    int n, crashed, starter;
    cout << "\n--- Bully Algorithm ---\n";
    cout << "Enter number of processes: ";
    cin >> n;

    int alive[20];

    cout << "Enter crashed process id: ";
    cin >> crashed;

    // Mark alive processes
    for (int i = 1; i <= n; i++)
        alive[i] = (i == crashed) ? 0 : 1;

    cout << "Election started by: ";
    cin >> starter;

    cout << "\nMessages sent:\n";
    for (int i = starter + 1; i <= n; i++) {
        cout << starter << " → " << i;
        if (alive[i]) cout << " (OK)\n";
        else cout << " (No reply)\n";
    }

    // Highest alive process becomes coordinator
    int coordinator = -1;
    for (int i = n; i >= 1; i--) {
        if (alive[i]) {
            coordinator = i;
            break;
        }
    }

    cout << "\nNew Coordinator = Process " << coordinator << "\n";
}


/* -------------------- RING ALGORITHM -------------------- */
void ringElection() {
    int n, initiator;
    cout << "\n--- Ring Algorithm ---\n";
    cout << "Enter number of processes in ring: ";
    cin >> n;

    int pid[20];
    cout << "Enter process IDs in ring order:\n";
    for (int i = 0; i < n; i++)
        cin >> pid[i];

    cout << "Enter initiator index (0 to n-1): ";
    cin >> initiator;

    int maxId = pid[initiator];
    int i = initiator;

    cout << "\nToken passing sequence:\n";
    do {
        cout << pid[i] << " → ";
        if (pid[i] > maxId)
            maxId = pid[i];

        i = (i + 1) % n;
    } while (i != initiator);

    cout << "BACK\n";
    cout << "\nNew Coordinator = Process " << maxId << "\n";
}


/* -------------------- MAIN -------------------- */
int main() {
    int choice;

    cout << "\nElection Algorithms Simulation";
    cout << "\n1. Bully Algorithm";
    cout << "\n2. Ring Algorithm";
    cout << "\nEnter choice: ";
    cin >> choice;

    if (choice == 1)
        bullyElection();
    else if (choice == 2)
        ringElection();
    else
        cout << "Invalid choice!";

    return 0;
}
