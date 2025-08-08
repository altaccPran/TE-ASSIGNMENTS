#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()

using namespace std;

void divide(const vector<int>& n, const vector<int>& k, vector<int>& ans, vector<int>& rem) {
    int i = 0;
    rem.clear();
    ans.clear();

    // Initialize rem with the first k.size() bits from n
    for (int y = 0; y < (int)k.size(); y++) {
        rem.push_back(n[y]);
        i++;
    }

    while (i < (int)n.size()) {
        if (rem[0] == 1) {
            ans.push_back(1);
            for (int j = 0; j < (int)k.size(); j++) {
                int xor_val = k[j] ^ rem[j];
                rem[j] = xor_val;
            }
            if (rem[0] == 0) {
                for (int x = 0; x < (int)rem.size() - 1; x++) {
                    rem[x] = rem[x + 1];
                }
            }
        }
        else if (rem[0] == 0) {
            ans.push_back(0);
            for (int j = 0; j < (int)k.size(); j++) {
                int xor_val = rem[j] ^ 0;
                rem[j] = xor_val;
            }
            if (rem[0] == 0) {
                for (int x = 0; x < (int)rem.size() - 1; x++) {
                    rem[x] = rem[x + 1];
                }
            }
        }
        rem[rem.size() - 1] = n[i];
        i++;
    }

    if (rem[0] == 1) {
        ans.push_back(1);
        for (int j = 0; j < (int)k.size(); j++) {
            int xor_val = k[j] ^ rem[j];
            rem[j] = xor_val;
        }
    }
    else if (rem[0] == 0) {
        ans.push_back(0);
        for (int j = 0; j < (int)k.size(); j++) {
            int xor_val = rem[j] ^ 0;
            rem[j] = xor_val;
        }
    }

    cout << "Syndrome: ";
    for (int bit : rem) cout << bit;
    cout << "\n";
}

int main() {
    vector<int> n;
    vector<int> k = {1, 0, 1, 0, 1};

    cout << "Enter data: ";
    string data;
    getline(cin, data);

    // Convert string to ASCII sum
    int v = 0;
    for (char c : data) {
        v += static_cast<int>(c);
    }

    // Convert to binary string
    string binary = "";
    int temp = v;
    if (temp == 0) binary = "0";
    while (temp > 0) {
        binary = (temp % 2 == 0 ? "0" : "1") + binary;
        temp /= 2;
    }

    // Fill vector n with bits
    for (char c : binary) {
        n.push_back(c - '0');
    }

    // Append zeros for CRC bits
    for (size_t i = 0; i < k.size() - 1; i++) {
        n.push_back(0);
    }

    cout << "Data: ";
    for (int bit : n) cout << bit;
    cout << "\n";

    cout << "Generator: ";
    for (int bit : k) cout << bit;
    cout << "\n";

    vector<int> ans, rem;
    divide(n, k, ans, rem);

    // Add remainder bits to n
    int lengthn = (int)n.size();
    int lengthk = (int)k.size();
    int z = 0;
    for (int f = lengthn - 1; f > lengthn - lengthk; f--) {
        n[f] = rem[z];
        z++;
    }

    cout << "Do you want to add error?\n1-->Yes\n2-->No\n";
    int ch; 
    cin >> ch;

    srand(time(nullptr)); // Seed random generator

    if (ch == 1) {
        int r = rand() % lengthn;
        cout << "Bit changed at position " << r + 1 << "\n";
        n[r] = 1 - n[r];  // Flip the bit

        vector<int> ans1, rem1;
        divide(n, k, ans1, rem1);
        cout << "Error detected\n";
    }
    else if (ch == 2) {
        vector<int> ans2, rem2;
        divide(n, k, ans2, rem2);
    }

    return 0;
}
