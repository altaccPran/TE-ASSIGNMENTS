#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

using namespace std;

void hostname_to_ip(const char* hostname) {
    struct addrinfo hints, *res, *p;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        cerr << "Failed to resolve hostname" << endl;
        return;
    }

    cout << "IP addresses for " << hostname << ":" << endl;

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        const char *ipver;

        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        cout << "  " << ipver << ": " << ipstr << endl;
    }

    freeaddrinfo(res);
}

void ip_to_hostname(const char* ip) {
    struct sockaddr_in sa;
    char host[NI_MAXHOST];

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;

    if (inet_pton(AF_INET, ip, &sa.sin_addr) <= 0) {
        cerr << "Invalid IP address" << endl;
        return;
    }

    if (getnameinfo((struct sockaddr*)&sa, sizeof(sa), host, sizeof(host),
                    NULL, 0, NI_NAMEREQD)) {
        cerr << "Could not resolve hostname for " << ip << endl;
    } else {
        cout << "Hostname for " << ip << ": " << host << endl;
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        cerr << "Failed to initialize Winsock" << endl;
        return 1;
    }
#endif

    cout << "DNS Lookup Program" << endl;
    cout << "1. Hostname to IP" << endl;
    cout << "2. IP to Hostname" << endl;
    cout << "Enter choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        string hostname;
        cout << "Enter hostname (e.g. www.google.com): ";
        cin >> hostname;
        hostname_to_ip(hostname.c_str());
    } else if (choice == 2) {
        string ip;
        cout << "Enter IP address (e.g. 8.8.8.8): ";
        cin >> ip;
        ip_to_hostname(ip.c_str());
    } else {
        cout << "Invalid choice" << endl;
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

