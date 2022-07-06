#include "HTTPAuthorization.h"

std::string lb::HTTPAuhtorization::gen_auth_arg_response(const std::string& method, const std::string& uri, const std::string& username, const std::string& password) const {
    std::string auth_response, HA1, HA2;
    HA1 = md5(username+':'+realm+':'+password); HA2 = md5(method+':'+uri);
    if(qop == "auth" || qop == "auth-int"){
        auth_response = md5(HA1+':'+nonce+':'+nc+':'+cnonce+':'+qop+':'+HA2);
    }
    else
    {
        auth_response = md5(HA1+':'+nonce+':'+HA2);
    }
    return auth_response;
}

std::string lb::HTTPAuhtorization::gen_authorization(const std::string& method, const std::string& uri, const std::string& username, const std::string& password) const {
    return "Authorization: "+type+" username=\""+username+"\", realm=\""+realm+"\", nonce=\""+nonce+"\", uri=\""+uri+"\", response=\""+gen_auth_arg_response(method, uri, username, password)+"\", qop=\""+qop+"\", nc="+nc+", cnonce=\""+cnonce+"\"";
}