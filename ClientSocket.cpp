#include "ClientSocket.h"


namespace lb{
    ClientSocket::ClientSocket(const std::string& ip, const std::string& port) : ip_(ip), port_(port) {}
    ClientSocket::ClientSocket() {}
    ClientSocket::~ClientSocket() {}

    int ClientSocket::open_connection(){
        addrinfo args, *serverinfo;
        // init arguments
        memset(&args, 0, sizeof(args));
        args.ai_family = AF_UNSPEC;
        args.ai_socktype = SOCK_STREAM;

        // search for server info by dns/ip
        if(getaddrinfo(ClientSocket::ip_.c_str(), ClientSocket::port_.c_str(), &args, &serverinfo) != 0){
            return 1;
        }

        addrinfo * connct;
        // loop through the linked list to connect
        for(connct = serverinfo; connct != NULL; connct=connct->ai_next){
            ClientSocket::sock = socket(connct->ai_family, connct->ai_socktype, connct->ai_protocol);
            if(ClientSocket::sock == -1){
                continue;
            }
            if(connect(ClientSocket::sock, connct->ai_addr, connct->ai_addrlen) == -1){
                close(ClientSocket::sock);
                continue;
            }
            break;
        }

        // if connecting failed
        if(connct == NULL){
            return 2;
        }

        freeaddrinfo(serverinfo);
        return 0;
    }

    int ClientSocket::close_connection(){
        if(ClientSocket::sock!=-1){
            close(ClientSocket::sock);
            return 0;
        }
        return 1;
    }

    int ClientSocket::send_request(const std::string& request) const {
        if(send(ClientSocket::sock, request.c_str(), request.size(), 0) == -1){
            return 1;
        }
        return 0;
    }

    std::string ClientSocket::fetch_response() const {
        std::string out;

        int bytes;
        // recieve data in 1024 bit chunks
        char data[1024];
        while(bytes = recv(ClientSocket::sock, data, 1024, 0)){
            for(int i = 0; i < bytes; i++){
                out += data[i];
            }
        }

        return out;
    }

    void ClientSocket::change_ip(const std::string& ip) { ip_=ip; }

    void ClientSocket::change_port(const std::string& port) { port_=port; }
}