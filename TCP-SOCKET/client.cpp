#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define CLOSESOCK closesocket
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSESOCK close
#endif

#define PORT 5000

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    SOCKET sock;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        CLOSESOCK(sock);
        return 1;
    }
    std::cout << "[+] Connected to server\n";

    int choice;
    std::cout << "Choose option:\n1) Hello\n2) File Transfer\n3) Calculator\n> ";
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
        send(sock, "HELLO", 5, 0);
        send(sock, "Hello from Client!", 19, 0);
        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);
        std::cout << "Server says: " << buffer << "\n";

    } else if (choice == 2) {
        send(sock, "FILE", 4, 0);
        std::string filename;
        std::cout << "Enter filename to send: ";
        std::getline(std::cin, filename);
        std::ifstream infile(filename, std::ios::binary);
        if (!infile.is_open()) {
            std::cerr << "File not found!\n";
            CLOSESOCK(sock);
            return 1;
        }
        send(sock, filename.c_str(), filename.size(), 0);
        while (!infile.eof()) {
            infile.read(buffer, sizeof(buffer));
            int bytes = infile.gcount();
            if (bytes > 0) send(sock, buffer, bytes, 0);
        }
        infile.close();
        std::cout << "[+] File sent successfully\n";

    } else if (choice == 3) {
        send(sock, "CALC", 4, 0);
        std::string expr;
        std::cout << "Enter expression (e.g., 5 + 3): ";
        std::getline(std::cin, expr);
        send(sock, expr.c_str(), expr.size(), 0);
        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);
        std::cout << "Result from server: " << buffer << "\n";
    }

    CLOSESOCK(sock);

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
