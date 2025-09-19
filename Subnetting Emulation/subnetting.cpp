#include <iostream>
#include <sstream>
#include <array>
#include <cmath>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <limits>

using namespace std;

long long ipToInt(const string &ip) {
    stringstream ss(ip);
    string part;
    long long res = 0;
    for (int i = 0; i < 4; ++i) {
        if (!getline(ss, part, '.')) return -1;
        int val;
        try {
            size_t idx;
            val = stoi(part, &idx);
            if (idx != part.size()) return -1;
        } catch (...) {
            return -1;
        }
        if (val < 0 || val > 255) return -1;
        res = (res << 8) + val;
    }
    return res;
}

string intToIp(uint32_t ip) {
    stringstream ss;
    for (int i = 3; i >= 0; --i) {
        ss << ((ip >> (8 * i)) & 0xFF);
        if (i) ss << '.';
    }
    return ss.str();
}

pair<int, array<int, 4>> calculateSubnetMaskClassC(int numSubnets) {
    int defaultMaskBits = 24;
    int bitsBorrowed = 0;
    if (numSubnets > 1) bitsBorrowed = static_cast<int>(ceil(log2(static_cast<double>(numSubnets))));
    int newMaskBits = defaultMaskBits + bitsBorrowed;
    int lastOctet = 0;
    for (int i = 0; i < bitsBorrowed; ++i) lastOctet += 1 << (7 - i);
    array<int, 4> mask = {255, 255, 255, lastOctet};
    return make_pair(newMaskBits, mask);
}

int pingIp(const string &ip) {
#ifdef _WIN32
    string cmd = "ping -n 1 " + ip + " >nul 2>&1";
#else
    string cmd = "ping -c 1 " + ip + " >/dev/null 2>&1";
#endif
    return system(cmd.c_str());
}

string checkIpReason(const string &destIp, const string &networkIp, const array<int, 4> &mask) {
    long long destInt = ipToInt(destIp);
    long long networkInt = ipToInt(networkIp);
    if (destInt == -1 || networkInt == -1) return "Invalid IP address.";
    uint32_t maskInt = 0;
    for (int i = 0; i < 4; ++i) maskInt = (maskInt << 8) + static_cast<uint32_t>(mask[i]);
    uint32_t networkAddr = static_cast<uint32_t>(networkInt) & maskInt;
    uint32_t broadcastAddr = networkAddr | (~maskInt);
    uint32_t destU = static_cast<uint32_t>(destInt);
    if (destU == networkAddr) return "The IP address is the network address.";
    else if (destU == broadcastAddr) return "The IP address is the broadcast address.";
    else if ((destU & maskInt) != networkAddr) return "The IP address is outside the local subnet.";
    else return "Host might be down or unreachable.";
}

void printDefaultMask() {
    cout << "Default Class C subnet mask: 255.255.255.0 (/24)\n";
}

void printSubnetRanges(const string &networkIp, int maskBits, int numSubnets) {
    long long baseInt = ipToInt(networkIp);
    if (baseInt == -1) {
        cout << "Invalid base network IP.\n";
        return;
    }
    int hostBits = 32 - maskBits;
    if (hostBits < 0) {
        cout << "Invalid mask bits.\n";
        return;
    }
    uint32_t blockSize = (hostBits >= 32) ? 0u : (1u << hostBits);
    cout << "\nSubnet Ranges (Network to Broadcast):\n";
    uint32_t base = static_cast<uint32_t>(baseInt);
    for (int i = 0; i < numSubnets; ++i) {
        uint32_t networkAddr = base + static_cast<uint32_t>(i) * blockSize;
        uint32_t broadcastAddr = networkAddr + blockSize - 1;
        cout << intToIp(networkAddr) << " to " << intToIp(broadcastAddr) << "\n";
        cout << "----------------------------------\n";
    }
}

int main() {
    printDefaultMask();
    int numSubnets;
    cout << "Enter number of subnets needed: ";
    if (!(cin >> numSubnets) || numSubnets < 1) {
        cout << "Number of subnets must be at least 1.\n";
        return 0;
    }
    auto result = calculateSubnetMaskClassC(numSubnets);
    int newMaskBits = result.first;
    array<int, 4> mask = result.second;
    cout << "Calculated subnet mask for " << numSubnets << " subnets:\n";
    cout << "New subnet mask: /" << newMaskBits << " or "
         << mask[0] << "." << mask[1] << "." << mask[2] << "." << mask[3] << "\n";
    cout << "Enter the network base IP address (default 192.168.5.0): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string networkIp;
    getline(cin, networkIp);
    if (networkIp.empty()) networkIp = "192.168.5.0";
    if (ipToInt(networkIp) == -1) {
        cout << "Invalid network IP address.\n";
        return 0;
    }
    printSubnetRanges(networkIp, newMaskBits, numSubnets);
    cout << "Enter destination IP address to ping: ";
    string destIp;
    getline(cin, destIp);
    if (ipToInt(destIp) == -1) {
        cout << "Invalid destination IP address.\n";
        return 0;
    }
    int responseCode = pingIp(destIp);
    if (responseCode == 0)
        cout << "Success: Ping to " << destIp << " was successful.\n";
    else {
        cout << "Failure: Ping to " << destIp << " failed.\n";
        cout << "Possible reason: " << checkIpReason(destIp, networkIp, mask) << "\n";
    }
    return 0;
}
