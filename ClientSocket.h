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
    class ClientSocket{
        public:
        
        ClientSocket(const std::string& ip, const std::string& port);
        ClientSocket();
        ~ClientSocket();

        ClientSocket(const ClientSocket&) = delete;
        ClientSocket& operator=(const ClientSocket&) = delete;
        ClientSocket(ClientSocket &&) = delete;
        ClientSocket& operator=(ClientSocket&&) = delete;

        int open_connection();
        int close_connection();

        int send_request(const std::string& request) const;
        std::string fetch_response() const;

        void change_ip(const std::string& ip);
        void change_port(const std::string& port);

        protected:
        std::string ip_;
        std::string port_;
        int sock;
    };
};

#endif