#ifndef CLIENTSOCKETHEADER
#define CLIENTSOCKETHEADER

#include <netdb.h>
// memset
#include <cstring>
// close(file descriptor)
#include <unistd.h>

#include <string>
#include <map>

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

        static std::map<std::string, std::string> parse_header_as_map(const std::string&);
        static std::string remove_header(const std::string&);
        static int get_status(const std::string&);

        private:
        std::string ip_;
        std::string port_;
        int sock;
    };
};

#endif