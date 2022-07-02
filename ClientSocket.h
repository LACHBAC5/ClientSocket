#ifndef CLIENTSOCKETHEADER
#define CLIENTSOCKETHEADER

#include <netdb.h>
// memset
#include <cstring>
// close(file descriptor)
#include <unistd.h>

#include <string>
#include <vector>
#include <map>

#include "md5.h"

namespace lb{
    struct HeaderData{
        std::string username;
        std::string password;
        std::string response;
        std::string method;
        std::string cnonce;
        std::string nonce;
        std::string realm;
        std::string uri;
        std::string qop;
        std::string nc;
    };
    

    class ClientSocket{
        public:
        
        ClientSocket(const std::string&, const std::string&);
        ClientSocket();
        ~ClientSocket();

        int open_connection();
        int close_connection();

        int send_request(const std::string&) const;
        std::string fetch_response() const;

        void set_info(const std::pair<std::string, std::string>&);
        std::pair<std::string, std::string> get_info();


        ClientSocket(const ClientSocket&) = delete;
        ClientSocket& operator=(const ClientSocket&) = delete;
        ClientSocket(ClientSocket &&) = delete;
        ClientSocket& operator=(ClientSocket&&) = delete;


        static std::map<std::string, std::string> htom(const std::string&);
        static int status_code(const std::string&);

        static std::string rmHeader(const std::string&);
        static std::string rmBody(const std::string&);

        static std::string gen_request(const std::string&, const std::string&, const std::vector<std::vector<std::string>>&);
        static std::string gen_auth_response_single(std::string (*)(std::string), const HeaderData&);

        private:
        std::string ip_;
        std::string port_;
        int sock;
    };
};

#endif
