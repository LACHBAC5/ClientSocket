#include "HTTPClientSocket.h"

namespace lb{
    HTTPClientSocket::HTTPClientSocket(const std::string& ip, const std::string& port, const std::string& username, const std::string& password) : ClientSocket(ip, port), username_(username), password_(password) {}
    HTTPClientSocket::HTTPClientSocket(const std::string& ip, const std::string& port) : ClientSocket(ip, port) {}
    HTTPClientSocket::HTTPClientSocket() {}
    HTTPClientSocket::~HTTPClientSocket() {}

    void HTTPClientSocket::change_username(const std::string& username) { username_=username; }
    void HTTPClientSocket::change_password(const std::string& password) { password_=password; }

    std::string HTTPClientSocket::send_http_request(const std::string& method, const std::string& uri, const std::map<std::string, std::vector<std::string>>& args, const std::string& body){
        // generate request
        std::string message = method + ' ' + uri + " HTTP/1.0";

        for(const auto& [name, value] : args){
            message += "\r\n"+name+":";
            for(int i = 0; i < value.size()-1; i++){
                message += value[i] + ", ";
            }
            message += value[value.size()-1];
        }

        // add authorization
        if(authorization){
            authArg.cnonce = "024897c82e2d7cab";
            message += "\r\n"+authArg.gen_authorization(method, uri, username_, password_);
            authArg.nc = std::to_string(std::stoi(authArg.nc)+1);
        }

        // add body
        if(body.size() > 0){
            message += "\r\n\r\n" + body;
        }
        message += "\r\n\r\n";

        // send request and recv header
        if(send_request(message)!=0){
            return "";
        }
        if((message=fetch_http_header()).size()==0){
            return "";
        }
        
        // check for authorization
        if(status_code(message) == 401 && !authorization){
            auto header_map = htom(message);
            std::string auth_response = header_map.at("WWW-Authenticate");

            int pos = auth_response.find(' ');
            authArg.type = auth_response.substr(0, pos);

            pos = auth_response.find("realm=\"")+7;
            authArg.realm = auth_response.substr(pos, auth_response.find("\"", pos)-pos);
            pos = auth_response.find("nonce=\"")+7;
            authArg.nonce = auth_response.substr(pos, auth_response.find("\"", pos)-pos);
            pos = auth_response.find("qop=\"")+5;
            authArg.qop = auth_response.substr(pos, auth_response.find("\"", pos)-pos);
            authArg.nc = "1";

            authorization=true;
            if(header_map.at("Connection") == "close"){
                open_connection();
            }

            return send_http_request(method, uri, args, body);
        }

        return message;
    }

    std::string HTTPClientSocket::fetch_http_header() const {
        std::string out;
        int bytes, times=0;

        char buffer[1];
        while(bytes = recv(sock, buffer, 1, 0)){
            out += buffer;
            if(buffer[0] == '\r' || buffer[0] == '\n'){
                times++;
                if(times == 4){
                    return out;
                }
            }
            else
            {
                times=0;
            }
        }
        return "";
    }

    std::map<std::string, std::string> HTTPClientSocket::htom(const std::string& http_header){
        std::map<std::string, std::string> header;

        int header_size = http_header.find("\r\n\r\n");

        int start = 0, end, middle;
        while((end=http_header.find("\r\n", start))<=header_size){
            if((middle=http_header.find(':', start))<end){
                header.insert(std::make_pair(http_header.substr(start, middle-start), http_header.substr(middle+2, end-middle-2)));
            }
            start = end+2;
        }

        header.insert(std::make_pair("HEADER-LENGTH-TOTAL", std::to_string(header_size+4)));
        // ?add status to map

        return header;
    }

    std::string HTTPClientSocket::mtoh(const std::map<std::string, std::string>& http_mheader){
        return "";
    }

    int HTTPClientSocket::status_code(const std::string& http_response) {
        return std::stoi(http_response.substr(http_response.find(' ', 0)+1, 3));
    }

    std::string HTTPClientSocket::rmHeader(const std::string& http_response){
        auto header_map = HTTPClientSocket::htom(http_response);
        int header_length = std::stoi(header_map.at("HEADER-LENGTH-TOTAL"));
        return http_response.substr(header_length, http_response.size()-header_length);
    }

    std::string HTTPClientSocket::rmBody(const std::string& http_response){
        auto header_map = HTTPClientSocket::htom(http_response);
        int header_length = std::stoi(header_map.at("HEADER-LENGTH-TOTAL"));
        return http_response.substr(0, header_length);
    }
}