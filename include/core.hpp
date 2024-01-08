#ifndef CORE_NETWORK_HPP
#define CORE_NETWORK_HPP

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>

bool receiveWithLengthAndAck(int clientSocket);
void receiveMessages(int clientSocket);
void sendMessages(int clientSocket);
void sendWithLengthAndWaitAck(int clientSocket, char* buffer, size_t len);

class Client {
sockaddr_in serverAddress;
int recvSocket, sendSocket;
public:
    Client (int port, char* ip);
    ~Client();
    int initConnect();
    void run();
};

class Server {
    sockaddr_in serverAddress;
    int currentSocket;
public:
    Server(int port);
    ~Server();
    int initConnect();

    void run();
};



#endif