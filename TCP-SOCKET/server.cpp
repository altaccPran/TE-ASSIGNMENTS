#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
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

// Simple calculator parser: "a op b"
std::string evaluateExpression(const std::string &expr) {
    std::istringstream iss(expr);
    double a, b;
    char op;
    if (!(iss >> a >> op >> b)) return "Invalid expression";
    double result = 0;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/': 
            if (b == 0) return "Error: Divide by zero";
            result = a / b; 
            break;
        default: return "Unsupported operator";
    }
    return std::to_string(result);
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        CLOSESOCK(server_fd);
        return 1;
    }

    listen(server_fd, 1);
    std::cout << "[*] Server listening on port " << PORT << "\n";

    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed\n";
        CLOSESOCK(server_fd);
        return 1;
    }
    std::cout << "[+] Client connected\n";

    // Receive command
    memset(buffer, 0, sizeof(buffer));
    recv(new_socket, buffer, sizeof(buffer), 0);
    std::string command(buffer);

    if (command == "HELLO") {
        // Exchange hello
        recv(new_socket, buffer, sizeof(buffer), 0);
        std::cout << "Client says: " << buffer << "\n";
        send(new_socket, "Hello from Server!", 19, 0);

    } else if (command == "FILE") {
        // Receive file
        memset(buffer, 0, sizeof(buffer));
        recv(new_socket, buffer, sizeof(buffer), 0);
        std::string filename = "received_" + std::string(buffer);
        std::ofstream outfile(filename, std::ios::binary);
        int bytes;
        while ((bytes = recv(new_socket, buffer, sizeof(buffer), 0)) > 0) {
            outfile.write(buffer, bytes);
        }
        outfile.close();
        std::cout << "[+] File received successfully\n";

    } else if (command == "CALC") {
        // Receive expression
        memset(buffer, 0, sizeof(buffer));
        recv(new_socket, buffer, sizeof(buffer), 0);
        std::string expr(buffer);
        std::cout << "Expression from client: " << expr << "\n";
        std::string result = evaluateExpression(expr);
        send(new_socket, result.c_str(), result.size(), 0);

    } else {
        std::cout << "Unknown command\n";
    }

    CLOSESOCK(new_socket);
    CLOSESOCK(server_fd);

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
