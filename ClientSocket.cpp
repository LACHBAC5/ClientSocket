#include "ClientSocket.h"

lb::ClientSocket::ClientSocket(const std::string& ip, const std::string& port) : ip_(ip), port_(port) {}

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

    /*
    // save status of the response
    lb::ClientSocket::status_code = lb::ClientSocket::check_status_code(out);

    // parse header info
    const auto header_info = lb::ClientSocket::parse_header_as_map(out);

    // cut header and return requested body
    int header_length = std::stoi(header_info.at("HEADER-LENGTH-TOTAL"));
    int message_length = std::stoi(header_info.at("Content-Length"));
    return out.substr(header_length, message_length);
    */

    return out;
}

std::string lb::ClientSocket::get_ip() {
    return lb::ClientSocket::ip_;
}

std::string lb::ClientSocket::get_port() {
    return lb::ClientSocket::port_;
}

int lb::ClientSocket::get_status(const std::string& http_response) {
    return std::stoi(http_response.substr(http_response.find(' ', 0)+1, 3));
}

std::map<std::string, std::string> lb::ClientSocket::parse_header_as_map(const std::string& http_response){
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

std::string lb::ClientSocket::remove_header(const std::string& http_response){
    std::string http_response_body;
    auto header_map = lb::ClientSocket::parse_header_as_map(http_response);
    int header_length = std::stoi(header_map.at("HEADER-LENGTH-TOTAL"));
    return http_response.substr(header_length, http_response.size()-header_length);
}

std::string lb::ClientSocket::make_digest_access_string(const std::string& username, const std::string& password, const std::string& method, const std::string& realm, const std::string& nonce, const std::string& uri, const std::string& qop, const std::string& nc, const std::string& cnonce){
    std::string response = md5(md5(username+':'+realm+':'+password)+':'+nonce+':'+nc+':'+cnonce+':'+qop+':'+md5(method+':'+uri));
    return "Authorization: Digest username=\""+username+"\", realm=\""+realm+"\", nonce=\""+nonce+"\", uri=\""+uri+"\", response=\""+response+"\", qop=\""+qop+"\", nc="+nc+", cnonce=\""+cnonce+"\"";
}