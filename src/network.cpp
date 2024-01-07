#include "../include/core.hpp"


void receiveWithLengthAndAck(int clientSocket) {
    size_t bufferSize = 1024; // Initial buffer size
    size_t ackStatus = 0;
    ssize_t bytesRead, bytesSent;
    try {
        bytesRead = recv(clientSocket, &bufferSize, sizeof(bufferSize), 0);
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                std::cerr << "Connection closed by other side\n";
            } else {
                std::cerr << "Error receiving data\n";
            }
            return;
        }
        std::unique_ptr<char[], void (*)(char*)> buffer(new char[bufferSize+1], [](char* p) {
            delete[] p;
        });
        bytesRead = recv(clientSocket, &buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                std::cerr << "Connection closed by other side\n";
            } else {
                std::cerr << "Error receiving data\n";
            }
            ackStatus = 0;
        }
        buffer[bytesRead] = '\0';
        std::cout << "Receive: " << buffer << std::endl;
        bytesSent = send(clientSocket, &ackStatus, sizeof(ackStatus), 0);
        if (bytesSent <= 0) {
            std::cerr << "Error sending data length\n";
        }
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation error: " << e.what() << std::endl;
    } 
}

void receiveMessages(int clientSocket) {
    while (true) {
        receiveWithLengthAndAck(clientSocket);
    }
}

void sendWithLengthAndWaitAck(int clientSocket, char* buffer, size_t len) {
    ssize_t bytesSent,bytesRead;
    bytesSent = send(clientSocket, &len, sizeof(len), 0);
    if (bytesSent <= 0) {
        std::cerr << "Error sending data length\n";
        return;
    }
    bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
    if (bytesSent <= 0) {
        std::cerr << "Error sending data\n";
        return;
    }

    size_t ackStatus = -1;
    bytesRead = recv(clientSocket, &ackStatus, sizeof(ackStatus), 0);
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
        std::cout << "Sending: " << line << std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();
        sendWithLengthAndWaitAck(clientSocket,&line[0], line.size());
        auto endTime = std::chrono::high_resolution_clock::now();
        // Calculate the duration
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

        // Output the duration in microseconds
        std::cout << "Time taken to send: " << duration.count() << " microseconds" << std::endl;

    }
}
