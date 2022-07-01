#include "Camera.h"

lb::Camera::Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password) : socket(ip, port), username_(username), password_(password) {
    socket.open_connection();
}

lb::Camera::~Camera(){
    socket.close_connection();
}

std::string lb::Camera::send_request(const std::string& method, const std::string& uri){
    std::string response;
    if(nc_ == 0){
        if(socket.send_request(method + ' ' + uri + " HTTP/1.0\r\n\r\n")!=0){
            std::cout << "failed1\n";
            return "";
        }
        response = socket.fetch_response();

        auto header = socket.parse_header_as_map(response);

        std::string authenticate = header.at("WWW-Authenticate");

        int pos = authenticate.find("realm=\"")+7;
        realm_ = authenticate.substr(pos, authenticate.find('\"', pos)-pos);
        pos = authenticate.find("nonce=\"")+7;
        nonce_ = authenticate.substr(pos, authenticate.find('\"', pos)-pos);
        pos = authenticate.find("qop=\"")+5;
        qop_ = authenticate.substr(pos, authenticate.find('\"', pos)-pos);
        nc_ = 1;
    }

    socket.open_connection();
    std::string autho = socket.make_digest_access_string(username_, password_, method, realm_, nonce_, uri, qop_, std::to_string(nc_), cnonce_);
    
    if(socket.send_request(method + ' ' + uri + " HTTP/1.0\r\n" + autho + "\r\n\r\n")!=0){
        nc_ = 0;
        return "";
    }
    nc_++;
    return socket.fetch_response();
}
