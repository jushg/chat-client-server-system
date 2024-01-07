#include "../include/core.hpp"

Server::Server(int port) {
    currentSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (currentSocket == -1) {
        std::cerr << "Error creating server socket\n";
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

    std::cout << "Server is listening for connections...\n";

    return 0;
}

void Server::run() {
    while (true) {
        sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(currentSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        std::cout << "Connection established with client\n";

        std::thread receiveThread(receiveMessages, clientSocket);
        std::thread sendThread(sendMessages, clientSocket);

        receiveThread.join();
        sendThread.join();

        close(clientSocket);
    }
}
