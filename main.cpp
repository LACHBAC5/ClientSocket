#include "ClientSocket.h"

#include <iostream>

/*
Authorization: 
Digest 
username="admin", 
realm="iPolis_00-09-18-52-39-00", 
nonce="b42f8a4229a68761bdf247d3f03d5618",  
qop=auth, 
*/

int main(){
    lb::ClientSocket soc("ivos.ddns.net", "321");
    std::cout << soc.open_connection();
    std::cout << soc.send_request("GET /stw-cgi/image.cgi?msubmenu=imageenhancements&action=set&Channel=0&Brightness=20 HTTP/1.0\r\nWWW-Authenticate: Digest realm=iPolis_00-09-18-52-39-00 nonce=692172c8112fdc038f7907871631f906 qop=auth\r\n\r\n");

    soc.fetch_response();

}