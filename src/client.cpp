
#include "../include/core.hpp"

Client::Client (int port, char* ip) {
    recvSocket = socket(AF_INET, SOCK_STREAM, 0);
    sendSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (recvSocket == -1 || sendSocket == -1) {
        std::cerr << "Error creating socket\n";
        return;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddress.sin_addr);
}
Client::~Client() {
    close(recvSocket);
    close(sendSocket);
}

int Client::initConnect() {
    if (connect(recvSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(recvSocket);
        return -1;
    }

    if (connect(sendSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(sendSocket);
        return -1;
    }

    std::cout << "Connected to server\n";
    return 0;
}

void Client::run() {
    std::thread receiveThread(receiveMessages, recvSocket);
    std::thread sendThread(sendMessages, sendSocket);
    receiveThread.join();
    sendThread.join();
}
