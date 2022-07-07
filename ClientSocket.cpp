#include "ClientSocket.h"

lb::ClientSocket::ClientSocket(const std::string& ip, const std::string& port) : ip_(ip), port_(port) {}
lb::ClientSocket::ClientSocket() {}
lb::ClientSocket::~ClientSocket() {}

int lb::ClientSocket::open_connection(){
    addrinfo args, *serverinfo;
    // init arguments
    memset(&args, 0, sizeof(args));
    args.ai_family = AF_UNSPEC;
    args.ai_socktype = SOCK_STREAM;

    // search for server info by dns/ip
    if(getaddrinfo(lb::ClientSocket::ip_.c_str(), lb::ClientSocket::port_.c_str(), &args, &serverinfo) != 0){
        return 1;
    }

    addrinfo * connct;
    // loop through the linked list to connect
    for(connct = serverinfo; connct != NULL; connct=connct->ai_next){
        lb::ClientSocket::sock = socket(connct->ai_family, connct->ai_socktype, connct->ai_protocol);
        if(lb::ClientSocket::sock == -1){
            continue;
        }
        if(connect(lb::ClientSocket::sock, connct->ai_addr, connct->ai_addrlen) == -1){
            close(lb::ClientSocket::sock);
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

int lb::ClientSocket::close_connection(){
    if(lb::ClientSocket::sock!=-1){
        close(lb::ClientSocket::sock);
        return 0;
    }
    return 1;
}

int lb::ClientSocket::send_request(const std::string& request) const {
    if(send(lb::ClientSocket::sock, request.c_str(), request.size(), 0) == -1){
        return 1;
    }
    return 0;
}

std::string lb::ClientSocket::fetch_response() const {
    std::string out;

    int bytes;
    // recieve data in 1024 bit chunks
    char data[1024];
    while(bytes = recv(lb::ClientSocket::sock, data, 1024, 0)){
        data[bytes]= '\0';
        out += data;
    }

    return out;
}

void lb::ClientSocket::change_ip(const std::string& ip) { ip_=ip; }

void lb::ClientSocket::change_port(const std::string& port) { port_=port; }