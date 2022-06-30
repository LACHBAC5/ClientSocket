#ifndef CLIENTSOCKETHEADER
#define CLIENTSOCKETHEADER

#include <netdb.h>
// memset
#include <cstring>
// close(file descriptor) 
#include <unistd.h>

#include <string>
#include <map>

#include <iostream>

namespace lb{
    class ClientSocket{
        public:
        
        ClientSocket(const std::string&, const std::string&);

        ~ClientSocket();

        int open_connection();

        int close_connection();

        int send_request(const std::string&) const;

        std::string fetch_response() const;

        std::string get_ip();
        std::string get_port();
        int get_status_code();

        private:
        std::string ip_;
        std::string port_;
        int sock;
        mutable int status_code;

        static std::map<std::string, std::string> parse_header_as_map(const std::string&);
        static int check_status_code(const std::string&);
    };
};

#endif