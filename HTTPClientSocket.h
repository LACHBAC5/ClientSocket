#ifndef HTTPClientSocketHEADER
#define HTTPClientSocketHEADER

#include <string>
#include <vector>
#include <map>

#include "ClientSocket.h"
#include "HTTPAuthorization.h"

namespace lb{
    class HTTPClientSocket : public ClientSocket{
        public:
        HTTPClientSocket(const std::string& ip, const std::string& port, const std::string& username, const std::string& password);
        HTTPClientSocket(const std::string& ip, const std::string& port);
        HTTPClientSocket();
        ~HTTPClientSocket();

        HTTPClientSocket(const HTTPClientSocket&) = delete;
        HTTPClientSocket& operator=(const HTTPClientSocket&) = delete;
        HTTPClientSocket(HTTPClientSocket &&) = delete;
        HTTPClientSocket& operator=(HTTPClientSocket&&) = delete;

        void change_username(const std::string& username);
        void change_password(const std::string& password);

        std::string send_http_request(const std::string& method, const std::string& uri, const std::vector<std::vector<std::string>>& args, const std::string& body);
        std::string fetch_http_header() const;

        static std::map<std::string, std::string> htom(const std::string& http_header);
        static std::string mtoh(const std::map<std::string, std::string>& http_mheader);

        static int status_code(const std::string& http_response);
        static std::string rmHeader(const std::string& http_response);
        static std::string rmBody(const std::string& http_response);

        private:
        // server info
        std::string username_, password_;

        // authorization info
        bool authorization=false;
        lb::HTTPAuhtorization authArg;
    };
}


#endif