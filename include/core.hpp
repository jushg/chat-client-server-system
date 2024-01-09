#ifndef CORE_NETWORK_HPP
#define CORE_NETWORK_HPP

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

void receiveMessages(int clientSocket, std::atomic<int>& isConnected);
void sendMessages(int clientSocket, std::atomic<int>& isConnected);


#define CONNECTED 1
#define DISCONNECTED 0


const std::string CLOSE_CONNECTION_ERR = "Connection closed by other side\n";
const std::string MESSAGE_LENGTH_SIZE_ERR = "Error receiving message length\n";
const std::string ACK_RECV_ERR = "Error receiving acknowledgement status\n";
const std::string ACK_STATUS_RECV_ERR = "Error sending, other side send error\n";
const std::string SOCKET_CREATION_ERR = "Error creating socket\n";

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