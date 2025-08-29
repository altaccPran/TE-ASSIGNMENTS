#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
using namespace std;

struct Frame {
    int seq;
    bool acked;
    bool received; // For SR
};

class Peer {
public:
    string name;
    int windowSize;
    int seqBits;
    int seqRange;
    int totalFrames;
    int base;
    int nextSeqNum;
    vector<Frame> frames;
    bool selectiveRepeat;

    Peer(string n, int m, bool sr) {
        name = n;
        seqBits = m;
        seqRange = (1 << m);      // 2^m
        selectiveRepeat = sr;
        windowSize = sr ? (1 << (m - 1)) : (seqRange - 1);
        totalFrames = seqRange;   // one full cycle only
        base = 0;
        nextSeqNum = 0;
        for (int i = 0; i < totalFrames; i++) {
            frames.push_back({i % seqRange, false, false});
        }
    }

    bool canSend() {
        return nextSeqNum < base + windowSize && nextSeqNum < totalFrames;
    }

    void sendFrame(Peer &receiver) {
        if (canSend()) {
            cout << name << " sending Frame " << frames[nextSeqNum].seq << endl;
            if (rand() % 100 < 20) {
                cout << "   (Frame " << frames[nextSeqNum].seq << " LOST)" << endl;
            } else {
                receiver.receiveFrame(frames[nextSeqNum].seq, *this, nextSeqNum);
            }
            nextSeqNum++;
        }
    }

    void receiveFrame(int seqNum, Peer &sender, int index) {
        cout << name << " received Frame " << seqNum << " from " << sender.name << endl;

        if (selectiveRepeat) {
            frames[index].received = true;
            if (rand() % 100 < 20) {
                cout << name << " tried to send ACK[" << seqNum << "] but it was LOST" << endl;
            } else {
                cout << name << " sending ACK[" << seqNum << "]" << endl;
                sender.ackFrame(index, selectiveRepeat);
            }

            for (int i = base; i < index; i++) {
                if (!frames[i].received) {
                    cout << name << " sending NAK[" << frames[i].seq << "] (missing frame in SR)" << endl;
                }
            }

        } else {
            if (seqNum == frames[base].seq) {
                if (rand() % 100 < 20) {
                    cout << name << " tried to send ACK[" << seqNum << "] but it was LOST" << endl;
                } else {
                    cout << name << " sending ACK[" << seqNum << "]" << endl;
                    sender.ackFrame(index, selectiveRepeat);
                }
            } else {
                cout << name << " discarded Frame " << seqNum << " (out of order in GBN)" << endl;
                cout << name << " sending NAK[" << frames[base].seq << "] (expected frame)" << endl;
            }
        }
    }

    void ackFrame(int index, bool sr) {
        cout << name << " received ACK[" << frames[index].seq << "]" << endl;
        frames[index].acked = true;

        int oldBase = base;
        while (base < totalFrames && frames[base].acked) {
            base++;
        }

        // Only prompt if the window actually shifted
        if (base > oldBase) {
            char cont;
            cout << "\nWindow shifted. Do you want to continue? (y/n): ";
            cin >> cont;
            if (cont != 'y' && cont != 'Y') {
                cout << "\nSimulation stopped by user.\n";
                exit(0);
            }
        }
    }

    bool allAcked() {
        return base >= totalFrames;
    }

    void timeout(Peer &receiver) {
        cout << name << " TIMEOUT! Resending window..." << endl;
        if (selectiveRepeat) {
            for (int i = base; i < nextSeqNum; i++) {
                if (!frames[i].acked) {
                    cout << name << " retransmitting Frame " << frames[i].seq << endl;
                    if (rand() % 100 < 20) {
                        cout << "   (Frame " << frames[i].seq << " LOST again)" << endl;
                    } else {
                        receiver.receiveFrame(frames[i].seq, *this, i);
                    }
                }
            }
        } else {
            for (int i = base; i < nextSeqNum; i++) {
                cout << name << " retransmitting Frame " << frames[i].seq << endl;
                if (rand() % 100 < 20) {
                    cout << "   (Frame " << frames[i].seq << " LOST again)" << endl;
                } else {
                    receiver.receiveFrame(frames[i].seq, *this, i);
                }
            }
        }
    }
};

int main() {
    srand(time(0));

    int m, choice;
    cout << "Enter m (number of bits for sequence number): ";
    cin >> m;
    cout << "1) Go-Back-N\n2) Selective Repeat\nChoice: ";
    cin >> choice;

    bool selectiveRepeat = (choice == 2);

    Peer A("PeerA", m, selectiveRepeat);
    Peer B("PeerB", m, selectiveRepeat);

    cout << "\nProtocol Selected: " << (selectiveRepeat ? "Selective Repeat" : "Go-Back-N") << endl;
    cout << "Window Size = " << A.windowSize << endl;
    cout << "Total Frames = " << A.totalFrames << endl;
    cout << "Simulating transmission...\n\n";

    while (!A.allAcked() || !B.allAcked()) {
        if (!A.allAcked()) {
            if (A.canSend()) A.sendFrame(B);
            else A.timeout(B);
        }

        if (!B.allAcked()) {
            if (B.canSend()) B.sendFrame(A);
            else B.timeout(A);
        }

        usleep(300000); // simulate delay
    }

    cout << "\nAll frames acknowledged for both peers. Transmission complete!" << endl;
    return 0;
}

