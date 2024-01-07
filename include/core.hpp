#ifndef CORE_NETWORK_HPP
#define CORE_NETWORK_HPP

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>


void receiveMessages(int clientSocket);
void sendMessages(int clientSocket);

class SocketBase {
protected:
    sockaddr_in serverAddress;
    int currentSocket;
public:
    ~SocketBase() {
        close(currentSocket);
    }
};

class Client: public SocketBase {
public:
    Client (int port, char* ip);
    int initConnect();
    void run();
};

class Server: public SocketBase {
public:
    Server(int port);
    int initConnect();

    void run();
};



#endif