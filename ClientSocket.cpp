#include "ClientSocket.h"

lb::ClientSocket::ClientSocket(const std::string& ip, const std::string& port) : ip_(ip), port_(port) {}
lb::ClientSocket::ClientSocket() {}
lb::ClientSocket::~ClientSocket() { close(lb::ClientSocket::sock); }

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

std::string lb::ClientSocket::fetch_response() const{
    std::string out;

    int bytes;

    // recieve data in 1024 bit chunks
    char data[1024];
    while(bytes = recv(lb::ClientSocket::sock, data, 1023, 0)){
        out += data;
    }

    return out;
}

void lb::ClientSocket::set_info(const std::pair<std::string, std::string>& info){
    ip_= info.first; port_= info.second;
}

std::pair<std::string, std::string> lb::ClientSocket::get_info(){
    return std::make_pair(ip_, port_);
}

std::map<std::string, std::string> lb::ClientSocket::htom(const std::string& http_response){
    std::map<std::string, std::string> header;

    int header_size = http_response.find("\r\n\r\n");

    int start = 0, end, middle;
    while((end=http_response.find("\r\n", start))<=header_size){
        if((middle=http_response.find(':', start))<end){
            header.insert(std::make_pair(http_response.substr(start, middle-start), http_response.substr(middle+2, end-middle-2)));
        }
        start = end+2;
    }

    header.insert(std::make_pair("HEADER-LENGTH-TOTAL", std::to_string(header_size+4)));

    return header;
}

int lb::ClientSocket::status_code(const std::string& http_response) {
    return std::stoi(http_response.substr(http_response.find(' ', 0)+1, 3));
}

std::string lb::ClientSocket::rmHeader(const std::string& http_response){
    auto header_map = lb::ClientSocket::htom(http_response);
    int header_length = std::stoi(header_map.at("HEADER-LENGTH-TOTAL"));
    return http_response.substr(header_length, http_response.size()-header_length);
}

std::string lb::ClientSocket::rmBody(const std::string& http_response){
    auto header_map = lb::ClientSocket::htom(http_response);
    int header_length = std::stoi(header_map.at("HEADER-LENGTH-TOTAL"));
    return http_response.substr(0, header_length);
}

std::string lb::ClientSocket::gen_request(const std::string& method, const std::string& uri, const std::vector<std::vector<std::string>>& args){
    std::string request = method + ' ' + uri + " HTTP/1.0";
    for(int i = 0; i < args.size(); i++){
        request += "\r\n"+args[i][0] + ": ";
        for(int o = 1; o < args[i].size()-1; o++){
            request += args[i][o] + ", ";
        }
        request += args[i][args[i].size()-1];
    }
    request += "\r\n\r\n";

    return request;
}

std::string lb::ClientSocket::gen_auth_response_single(std::string (*encfunction)(std::string) , const HeaderData& data){
    std::string auth_response, HA1, HA2;
    HA1 = encfunction(data.username+':'+data.realm+':'+data.password); HA2 = encfunction(data.method+':'+data.uri);
    if(data.qop == "auth" || data.qop == "auth-int"){
        auth_response = encfunction(HA1+':'+data.nonce+':'+data.nc+':'+data.cnonce+':'+data.qop+':'+HA2);
    }
    else
    {
        auth_response = encfunction(HA1+':'+data.nonce+':'+HA2);
    }
    return auth_response;
}

