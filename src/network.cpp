#include "../include/core.hpp"

std::pair<std::string,bool> receiveMessageWithLength(int clientSocket) {
    // Receive the length of the message first
    uint32_t messageLength;
    ssize_t bytesReceived = recv(clientSocket, &messageLength, sizeof(messageLength), 0);
    if (bytesReceived != sizeof(messageLength)) {
        if(bytesReceived <= 0)  {
            std::cerr << CLOSE_CONNECTION_ERR;
            return {"", false};
        } 
        std::cerr << MESSAGE_LENGTH_SIZE_ERR;
        return {"", true};
    }
    messageLength = ntohl(messageLength);
    // Receive the actual message
    char * buffer = new char[messageLength + 1];
    size_t totalBytesReceived = 0;
    while (totalBytesReceived < messageLength) {
        bytesReceived = recv(clientSocket, buffer + totalBytesReceived, messageLength - totalBytesReceived, 0);
        if(bytesReceived <= 0)  {
            std::cerr << CLOSE_CONNECTION_ERR;
            return {"", false};
        } 
        totalBytesReceived += bytesReceived;
    }

    buffer[messageLength] = '\0'; // Null-terminate the received message
    auto ans = std::string(buffer);
    delete[] buffer;

    return {ans,true};
}

bool receiveWithLengthAndAck(int clientSocket) {
    size_t ackStatus = 0;
    try {
        auto res = receiveMessageWithLength(clientSocket);
        if(!res.second) return false;
        std::cout << "Receive: " << res.first << std::endl;
        send(clientSocket, &ackStatus, sizeof(ackStatus), 0);        
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation error: " << e.what() << std::endl;
    } 
    return true;
}

void receiveMessages(int clientSocket, std::atomic<int>& connectStatus) {
    while (receiveWithLengthAndAck(clientSocket));
    connectStatus = DISCONNECTED;
    std::cout << "Press enter to terminate current session\n";
}

void sendMessageWithLength(int clientSocket, const std::string& message) {
    // Send the length of the message first
    uint32_t messageLength = htonl(message.length());
    send(clientSocket, &messageLength, sizeof(messageLength), 0);
    // Send the actual message
    send(clientSocket, message.c_str(), message.length(), 0);
}

bool sendWithLengthAndWaitAck(int clientSocket, const std::string& message) {
    sendMessageWithLength(clientSocket,message);
    size_t ackStatus = -1;
    ssize_t bytesRead = recv(clientSocket, &ackStatus, sizeof(ackStatus), 0);
    if (bytesRead != sizeof(ackStatus)) {
        if (bytesRead <= 0) {
            std::cerr << CLOSE_CONNECTION_ERR;
            return false;
        }
        std::cerr <<ACK_RECV_ERR;
        return true;
    }
    if(ackStatus != 0) std::cerr << ACK_STATUS_RECV_ERR;
    return true;
}

void sendMessages(int clientSocket, std::atomic<int>& connectStatus) {
    std::string line;
    while (connectStatus == CONNECTED) {
        std::getline(std::cin,line);
        if (connectStatus != CONNECTED) return;
        auto startTime = std::chrono::high_resolution_clock::now();
        auto isSendConnected = sendWithLengthAndWaitAck(clientSocket,line);
        auto endTime = std::chrono::high_resolution_clock::now();
        // Calculate the duration
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        // Output the duration in microseconds
        std::cout << "Time taken to send \""<< line<< "\" is " << duration.count() << " microseconds\n";
        if(!isSendConnected) connectStatus = DISCONNECTED;
    }
}
