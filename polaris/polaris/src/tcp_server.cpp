#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "../include/tcp_server.h"

TCPServer::TCPServer(int port) : port(port), server_fd(-1), client_fd(-1) {}
    
TCPServer::~TCPServer() {
        closeServer();
    }
    
    bool TCPServer::start() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            std::cerr << "Soket olusturma hatasi" << std::endl;
            return false;
        }
        
        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);
        
        if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Baglama hatasi" << std::endl;
            return false;
        }
        
        if (listen(server_fd, 1) < 0) {
            std::cerr << "Dinleme hatasi" << std::endl;
            return false;
        }
        
        std::cout << "Sunucu calisiyor (TCP): " << port << std::endl;
        return true;
    }
    
    bool TCPServer::acceptClient() {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "Client kabul edilmedi" << std::endl;
            return false;
        }
        std::cout << "Client baglandi" << std::endl;
        return true;
    }
    
    std::string TCPServer::receiveData() {
        char buffer[1024] = {0};
        int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            std::cerr << "Okuma hatasi: " << std::endl;
            return "";
        }
        return std::string(buffer, bytes_read);
    }
    
    void TCPServer::sendData(const std::string& data) {
        send(client_fd, data.c_str(), data.length(), 0);
    }
    
    void TCPServer::closeServer() {
        if (client_fd != -1) close(client_fd);
        if (server_fd != -1) close(server_fd);
    }