#include "../include/core.hpp"

int main() {
    int type, port; 
    std::string ip;

    std::cout << "Basic usage: If starting as client input 0 [host-ip] [port], if starting as server input 1 [port]\n";
    std::cin >> type;
    if(type == 0) {
        std::cin >> ip >> port;
        std::cin.ignore();
        Client cli = Client(port, &ip[0]);
        if(cli.initConnect() == -1) {
            return -1;
        }
        cli.run();
    } else {
        std::cin >> port;
        std::cin.ignore();
        Server serv = Server(port);
        if(serv.initConnect() == -1) {
            return -1;
        }
        serv.run();
    }
    return 0;
}
