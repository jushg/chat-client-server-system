#include "../include/core.hpp"

Server::Server(int port) {
    currentSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (currentSocket == -1) {
        std::cerr << SOCKET_CREATION_ERR;
        return;
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
}
int Server::initConnect() {
    if (bind(currentSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding server socket\n";
        close(currentSocket);
        return -1;
    }

    if (listen(currentSocket, 10) == -1) {
        std::cerr << "Error listening for connections\n";
        close(currentSocket);
        return -1;
    }


    return 0;
}

void Server::run() {
    while (true) {
        std::cout << "Server is listening for connections...\n";

        sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int sendSocket = accept(currentSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        int recvSocket = accept(currentSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (sendSocket == -1 || recvSocket == -1) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        std::cout << "Connection established with client\n";
        std::atomic<int> isConnected(1);


        std::thread receiveThread(receiveMessages, recvSocket, std::ref(isConnected));
        std::thread sendThread(sendMessages, sendSocket, std::ref(isConnected));

        receiveThread.join();
        sendThread.join();

        close(sendSocket);
        close(recvSocket);
        std::cout << "Connection closed with client\n";

    }
}

Server::~Server() {
    close(currentSocket);
}
