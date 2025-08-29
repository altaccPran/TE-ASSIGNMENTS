#include <bits/stdc++.h>
#include <chrono>
#include <thread>
using namespace std;

string ackOrTimeout(double prob = 0.4) {
    double r = (double) rand() / RAND_MAX;
    return (r < prob) ? "TIMEOUT" : "ACK";
}

void sleep_ms(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void transmission(int i, int N, int tf, int tt) {
    while (i <= tf) {
        cout << "Current Window: [" << i << " - " << min(i + N - 1, tf) << "]" << endl;
        int z = 0;
        bool timeoutOccurred = false;
        int timeoutFrame = -1;

        for (int k = i; k < i + N && k <= tf; k++) {
            cout << "Sending Frame " << k << "..." << endl;
            tt++;
            sleep_ms(500);
        }

        for (int k = i; k < i + N && k <= tf; k++) {
            string f = ackOrTimeout();
            if (f == "ACK") {
                cout << "Acknowledgment for Frame " << k << "..." << endl;
                sleep_ms(500);
                z++;
            } else {
                cout << "Timeout!! Frame Number : " << k << " Not Received" << endl;
                timeoutOccurred = true;
                timeoutFrame = k;
                break;
            }
        }

        if (timeoutOccurred) {
            int start = timeoutFrame;
            int end = min(start + N - 1, tf);
            cout << "\nRetransmitting Window [" << start << " - " << end << "]...\n" << endl;
            i = start;
        } else {
            i = i + z;
            if (i <= tf)
                cout << "Sliding window moved to [" << i << " - " << min(i + N - 1, tf) << "]\n" << endl;
        }
    }
}

void selectiveRepeat(int totalFrames, int windowSize) {
    vector<bool> ackReceived(totalFrames + 1, false);
    int base = 1, nextSeqNum = 1, totalSent = 0;

    while (base <= totalFrames) {
        while (nextSeqNum < base + windowSize && nextSeqNum <= totalFrames) {
            if (!ackReceived[nextSeqNum]) {
                cout << "Sending Frame " << nextSeqNum << "..." << endl;
                totalSent++;
                sleep_ms(300);
            }
            nextSeqNum++;
        }

        for (int i = base; i < base + windowSize && i <= totalFrames; i++) {
            if (!ackReceived[i]) {
                string f = ackOrTimeout();
                if (f == "ACK") {
                    cout << "Acknowledgment received for Frame " << i << endl;
                    ackReceived[i] = true;
                } else {
                    cout << "Timeout for Frame " << i << "! Retransmitting..." << endl;
                }
                sleep_ms(300);
            }
        }

        while (base <= totalFrames && ackReceived[base]) base++;

        if (base > totalFrames) break;

        cout << "Current Window: [" << base << " - " << min(base + windowSize - 1, totalFrames) << "]\n" << endl;
        sleep_ms(500);
    }

    cout << "Total frames sent (including retransmissions): " << totalSent << endl;
}

int main() {
    srand(time(0));
    int m, n;
    cout << "Enter m: ";
    cin >> m;

    do {
        int i = 1, tf, N, tt = 0;
        cout << "Choose the ARQ protocol: 1. Go-Back-N 2. Selective Repeat 3. Exit: ";
        cin >> n;

        if (n == 1) {
            cout << "Enter the Total number of frames: ";
            cin >> tf;
            N = pow(2, m) - 1;
            cout << "\nWindow Size : " << N << endl;
            transmission(i, N, tf, tt);
        }

        if (n == 2) {
            cout << "Enter the Total number of frames: ";
            cin >> tf;
            N = pow(2, m - 1);
            cout << "\nWindow Size : " << N << endl;
            selectiveRepeat(tf, N);
        }
    } while (n != 3);

    return 0;
}
