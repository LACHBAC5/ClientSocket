#include "ClientSocket.h"

#include <iostream>

#include "md5.h"

/* browser get request
GET /stw-cgi/attributes.cgi/attributes HTTP/1.1
Host: ivos.ddns.net:321
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:102.0) Gecko/20100101 Firefox/102.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Authorization: Digest username="admin", realm="iPolis_00-09-18-52-39-00", nonce="8c7501ff04be51c53be9477e55e97e17", uri="/stw-cgi/attributes.cgi/attributes", response="bae7ecaf4ee6e15e706d21936b6f5d4f", qop=auth, nc=00000002, cnonce="83ba2d99eed620dd"
Connection: keep-alive
Upgrade-Insecure-Requests: 1
*/

std::string make_digest(std::string username, std::string password, std::string method, std::string realm, std::string nonce, std::string uri, std::string qop, std::string nc, std::string cnonce){
    std::string response = md5(md5(username+':'+realm+':'+password)+':'+nonce+':'+nc+':'+cnonce+':'+qop+':'+md5(method+':'+uri));
    return "Digest username=\""+username+"\", realm=\""+realm+"\", nonce=\""+nonce+"\", uri=\""+uri+"\", response=\""+response+"\", qop="+qop+", nc="+nc+", cnonce=\""+cnonce+"\"";
}

int main(int argc, char *argv[]){


    std::string Digest = make_digest(argv[3], argv[4], "GET", "iPolis_00-09-18-52-39-00", "376cb247948cfa008a6ab98abd3d781a", "/stw-cgi/attributes.cgi/attributes", "auth", "00000001", "83ba2d99eed620dd");

    lb::ClientSocket soc(argv[1], argv[2]);
    std::cout << soc.open_connection();
    std::cout << soc.send_request("GET /stw-cgi/attributes.cgi/attributes HTTP/1.0\r\nHost: ivos.ddns.net:321\r\nWWW-Authenticate: " + Digest + "\r\n\r\n");

    soc.fetch_response();

    soc.close_connection();
}