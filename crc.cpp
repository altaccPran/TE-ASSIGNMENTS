#include <iostream>
#include <string>
#include <bitset>
#include <cstdlib>
#include <ctime>
using namespace std;

string crcdiv(string data, const string &gen) {
    int genLen = gen.size();
    string syndrome = data.substr(0, genLen);

    for (int i = genLen; i <= data.size(); i++) {
        if (syndrome[0] == '1') {
            for (int j = 0; j < genLen; j++)
                syndrome[j] = (syndrome[j] == gen[j]) ? '0' : '1';
        } else {
            for (int j = 0; j < genLen; j++)
                syndrome[j] = (syndrome[j] == '0') ? '0' : '1';
        }
        syndrome.erase(0, 1);
        if (i < data.size())
            syndrome.push_back(data[i]);
    }
    return syndrome;
}

string crcencode(string data, const string &gen) {
    data.append(gen.size() - 1, '0');
    return crcdiv(data, gen);
}

string crccheck(string codeword, const string &gen) {
    return crcdiv(codeword, gen);
}

int main() {
    srand(time(0));

    string generator;
    cout << "Enter generator polynomial (binary): ";
    cin >> generator;

    int zeros;
    cout << "Enter no. of zeroes to augment: ";
    cin >> zeros;

    cin.ignore();
    string input;
    cout << "Enter string: ";
    getline(cin, input);

    cout << "\n<Sender's Side>\n";

    // Process each character separately
    for (char c : input) {
        string binChar = bitset<7>(c).to_string();

        cout << "\nCharacter: " << c << endl;
        cout << "ASCII value: " << (int)c << endl;
        cout << "Binary value: " << binChar << endl;

        string remainder = crcencode(binChar, generator);
        cout << "CRC remainder: " << remainder << endl;

        string codeword = binChar + remainder;
        cout << "Codeword to be sent = " << codeword << endl;

        cout << "\n<Receiver's Side>\n";
        char choice;
        cout << "Do you want to induce error (y/n)? ";
        cin >> choice;

        string received = codeword;

        if (choice == 'y' || choice == 'Y') {
            // flip a random bit in this character’s codeword
            int bitPos = rand() % received.size();
            received[bitPos] = (received[bitPos] == '0') ? '1' : '0';
            cout << "\nRandom error generated!" << endl;
            cout << "Received Codeword = " << received << endl;

            string syndrome = crccheck(received, generator);
            cout << "Syndrome after error: " << syndrome << endl;
            cout << "Therefore this is error → CRC failed.\n";
        } else {
            cout << "\nNo error induced." << endl;
            cout << "Received Codeword = " << received << endl;

            string syndrome = crccheck(received, generator);
            cout << "Syndrome = " << syndrome << endl;
            if (stoi(syndrome, nullptr, 2) == 0) {
                cout << "There is no error." << endl;
                string dataword = received.substr(0, received.size() - (generator.size() - 1));
                cout << "Fetch the dataword: " << endl;
                cout << "Binary no: " << dataword << endl;
                char decoded = static_cast<char>(bitset<7>(dataword).to_ulong());
                cout << "Alphabet: " << decoded << endl;
            }
        }
    }

    return 0;
}
