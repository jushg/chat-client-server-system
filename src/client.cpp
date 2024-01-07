
#include "../include/core.hpp"

Client::Client (int port, char* ip) {
    currentSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (currentSocket == -1) {
        std::cerr << "Error creating socket\n";
        return;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddress.sin_addr);
}

int Client::initConnect() {
    if (connect(currentSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(currentSocket);
        return -1;
    }
    std::cout << "Connected to server\n";
    return 0;
}

void Client::run() {
    std::thread receiveThread(receiveMessages, currentSocket);
    std::thread sendThread(sendMessages, currentSocket);
    receiveThread.join();
    sendThread.join();
}
