#include "../include/core.hpp"


void receiveMessages(int clientSocket) {
    char buffer[1024];
    while (true) {
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                std::cerr << "Connection closed by other side\n";
            } else {
                std::cerr << "Error receiving data\n";
            }
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Receive: " << buffer << std::endl;
    }
}

void sendMessages(int clientSocket) {
    char buffer[1024];
    while (true) {
        
        std::cin.getline(buffer, sizeof(buffer));
        std::cout << "Send: " << buffer << std::endl;
        ssize_t bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
        if (bytesSent <= 0) {
            std::cerr << "Error sending data\n";
            break;
        }
    }
}
