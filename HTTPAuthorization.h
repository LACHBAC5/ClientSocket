#pragma once

#include <string>
#include "md5.h"

namespace lb{
    class HTTPAuhtorization{
        public:
        std::string type, realm, nonce, qop, nc, cnonce;

        std::string gen_auth_arg_response(const std::string& method, const std::string& uri, const std::string& username, const std::string& password) const;
        std::string gen_authorization(const std::string& method, const std::string& uri, const std::string& username, const std::string& password) const;
    };
}
