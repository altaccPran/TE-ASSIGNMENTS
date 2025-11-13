#include <bits/stdc++.h>
using namespace std;

int n;
int pid[10], at[10], bt[10], pri[10];

void printResult(int ct[], int tat[], int wt[]) {
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    float totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << pid[i] << "\t" << at[i] << "\t" << bt[i]
             << "\t" << ct[i] << "\t" << tat[i] << "\t" << wt[i] << "\n";
        totalWT += wt[i];
        totalTAT += tat[i];
    }
    cout << "\nAverage Waiting Time: " << totalWT / n;
    cout << "\nAverage Turnaround Time: " << totalTAT / n << "\n";
}

void fcfs() {
    int ct[10], tat[10], wt[10];

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (at[j] > at[j + 1]) {
                swap(at[j], at[j + 1]);
                swap(bt[j], bt[j + 1]);
                swap(pid[j], pid[j + 1]);
                swap(pri[j], pri[j + 1]);
            }
        }
    }

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < at[i]) time = at[i];
        time += bt[i];
        ct[i] = time;
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    cout << "\n--- FCFS Scheduling ---\n";
    printResult(ct, tat, wt);
}

void sjf_preemptive() {
    int bt_rem[10], ct[10], tat[10], wt[10];
    for (int i = 0; i < n; i++) bt_rem[i] = bt[i];

    int completed = 0, time = 0;

    while (completed < n) {
        int idx = -1, min_bt = 1e9;
        for (int i = 0; i < n; i++) {
            if (at[i] <= time && bt_rem[i] > 0 && bt_rem[i] < min_bt) {
                min_bt = bt_rem[i];
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        bt_rem[idx]--;
        time++;

        if (bt_rem[idx] == 0) {
            completed++;
            ct[idx] = time;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
        }
    }

    cout << "\n--- SJF Preemptive (SRTF) ---\n";
    printResult(ct, tat, wt);
}

void priority_preemptive() {
    int bt_rem[10], ct[10], tat[10], wt[10];
    for (int i = 0; i < n; i++) bt_rem[i] = bt[i];

    int completed = 0, time = 0;

    while (completed < n) {
        int idx = -1, max_pri = -1e9;
        for (int i = 0; i < n; i++) {
            if (at[i] <= time && bt_rem[i] > 0 && pri[i] > max_pri) {
                max_pri = pri[i];
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        bt_rem[idx]--;
        time++;

        if (bt_rem[idx] == 0) {
            completed++;
            ct[idx] = time;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
        }
    }

    cout << "\n--- Priority Preemptive ---\n";
    printResult(ct, tat, wt);
}

void priority_non_preemptive() {
    int ct[10], tat[10], wt[10];
    bool done[10] = {0};
    int completed = 0, time = 0;

    while (completed < n) {
        int idx = -1;
        int highestPriority = -1e9;
        
        // Find the highest priority process that has arrived
        for (int i = 0; i < n; i++) {
            if (!done[i] && at[i] <= time && pri[i] > highestPriority) {
                highestPriority = pri[i];
                idx = i;
            }
        }
        
        if (idx == -1) {
            // CPU idle if no process has arrived
            time++;
        } else {
            ct[idx] = time + bt[idx];       // Completion time
            tat[idx] = ct[idx] - at[idx];   // Turnaround time
            wt[idx] = tat[idx] - bt[idx];   // Waiting time
            time = ct[idx];                 // Advance current time
            done[idx] = true;
            completed++;
        }
    }

    cout << "\n--- Priority Non-Preemptive ---\n";
    printResult(ct, tat, wt);
}

void round_robin() {
    int tq;
    cout << "\nEnter Time Quantum: ";
    cin >> tq;

    int bt_rem[10], ct[10], tat[10], wt[10];
    for (int i = 0; i < n; i++) bt_rem[i] = bt[i];

    queue<int> q;
    int time = 0;
    int completed = 0;
    bool inQueue[10] = {0};

    int first = 0;
    for (int i = 1; i < n; i++) {
        if (at[i] < at[first]) first = i;
    }
    time = at[first];
    q.push(first);
    inQueue[first] = true;

    while (!q.empty()) {
        int idx = q.front();
        q.pop();
        inQueue[idx] = false;

        int runTime = min(tq, bt_rem[idx]);
        cout << "P" << pid[idx] << " runs from " << time
             << " to " << time + runTime << "\n";

        time += runTime;
        bt_rem[idx] -= runTime;

        for (int i = 0; i < n; i++) {
            if (i != idx && at[i] <= time && bt_rem[i] > 0 && !inQueue[i]) {
                q.push(i);
                inQueue[i] = true;
            }
        }

        if (bt_rem[idx] == 0) {
            completed++;
            ct[idx] = time;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
        } else {
            q.push(idx);
            inQueue[idx] = true;
        }
    }

    cout << "\n--- Round Robin ---\n";
    printResult(ct, tat, wt);
}

int main() {
    // Predefined data
    vector<int> pids = {1, 2, 3, 4};
    vector<int> ats  = {0, 1, 2, 3};
    vector<int> bts  = {5, 3, 1, 2};
    vector<int> pris = {7, 6, 5, 4};

    n = pids.size();

    for (int i = 0; i < n; i++) {
        pid[i] = pids[i];
        at[i] = ats[i];
        bt[i] = bts[i];
        pri[i] = pris[i];
    }

    cout << "========== CPU SCHEDULING ALGORITHMS ==========\n";
    cout << "1. FCFS (First Come First Serve)\n";
    cout << "2. SJF Preemptive (SRTF)\n";
    cout << "3. Priority Preemptive\n";
    cout << "4. Priority Non-Preemptive\n";
    cout << "5. Round Robin\n";
    cout << "===============================================\n";
    cout << "Enter choice: ";
    int ch;
    cin >> ch;

    switch (ch) {
        case 1: fcfs(); break;
        case 2: sjf_preemptive(); break;
        case 3: priority_preemptive(); break;
        case 4: priority_non_preemptive(); break;
        case 5: round_robin(); break;
        default: cout << "Invalid choice\n";
    }

    return 0;
}
