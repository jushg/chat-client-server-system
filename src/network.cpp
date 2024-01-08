#include "../include/core.hpp"



std::string receiveMessageWithLength(int clientSocket) {
    // Receive the length of the message first
    uint32_t messageLength;
    ssize_t bytesReceived = recv(clientSocket, &messageLength, sizeof(messageLength), 0);
    if (bytesReceived != sizeof(messageLength)) {
        std::cerr << "Error receiving message length\n";
        return "";
    }

    messageLength = ntohl(messageLength);

    // Receive the actual message
    char * buffer = new char[messageLength + 1];
    size_t totalBytesReceived = 0;
    while (totalBytesReceived < messageLength) {
        bytesReceived = recv(clientSocket, buffer + totalBytesReceived, messageLength - totalBytesReceived, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Error receiving message\n";
            return "";
        }
        totalBytesReceived += bytesReceived;
    }

    buffer[messageLength] = '\0'; // Null-terminate the received message
    auto ans = std::string(buffer);
    delete[] buffer;
    return ans;
}

bool receiveWithLengthAndAck(int clientSocket) {
    size_t ackStatus = 0;
    try {
        auto str = receiveMessageWithLength(clientSocket);
        std::cout << "Receive: " << str << std::endl;
        ssize_t bytesSent = send(clientSocket, &ackStatus, sizeof(ackStatus), 0);
        if (bytesSent <= 0) {
            std::cerr << "Error sending data length\n";
        }
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation error: " << e.what() << std::endl;
    } 
    return true;
}

void receiveMessages(int clientSocket) {
    while (receiveWithLengthAndAck(clientSocket));
}

// Function to send a complete message
void sendMessageWithLength(int clientSocket, const std::string& message) {
    // Send the length of the message first
    uint32_t messageLength = htonl(message.length());
    send(clientSocket, &messageLength, sizeof(messageLength), 0);

    // Send the actual message
    send(clientSocket, message.c_str(), message.length(), 0);
}

void sendWithLengthAndWaitAck(int clientSocket, const std::string& message) {
    sendMessageWithLength(clientSocket,message);
    size_t ackStatus = -1;
    ssize_t bytesRead = recv(clientSocket, &ackStatus, sizeof(ackStatus), 0);
    if (bytesRead <= 0) {
        if (bytesRead == 0) {
            std::cerr << "Connection closed by other side\n";
        } else {
            std::cerr << "Error receiving data\n";
        }
        return;
    }
    if(ackStatus != 0) {
        std::cerr << "Error sending, other side send error\n";
        return;
    } 
}

void sendMessages(int clientSocket) {
    std::string line;
    while (true) {
        std::getline(std::cin,line);
        auto startTime = std::chrono::high_resolution_clock::now();
        sendWithLengthAndWaitAck(clientSocket,line);
        auto endTime = std::chrono::high_resolution_clock::now();
        // Calculate the duration
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        // Output the duration in microseconds
        std::cout << "Time taken to send "<< line<< " is " << duration.count() << " microseconds" << std::endl;
    }
}
