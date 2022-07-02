#include "ClientSocket.h"
#include "Camera.h"

#include <iostream>


int main(int argc, char *argv[]){
    lb::Camera c(argv[1], argv[2], argv[3], argv[4]);
    std::cout << c.send_request("image.cgi", "imageenhancements", "set", "0", "Brightness", "1") << '\n';
    //std::cout << c.send_request("image.cgi", "imageenhancements", "set", "0", "Brightness", "99") << '\n';
    //std::cout << c.send_request("image.cgi", "imageenhancements", "set", "0", "Brightness", "1") << '\n';
}

/*
int main(){
    lb::ClientSocket cs;

    cs.set_info(std::make_pair(argv[1], argv[2]));

    lb::HeaderData info;

    info.username = argv[3]; info.password = argv[4];

    info.realm = "iPolis_00-09-18-52-39-00";
    info.nonce = "203a982dccdafeb97a07affb5e18450e"; info.cnonce = "024897c82e2d7cab"; info.nc = "2";

    info.method = "GET"; info.uri = "/stw-cgi/attributes.cgi/attributes";
    info.qop = "auth";

    info.response = cs.gen_auth_response_single(md5, info);

    std::string auth_args = "Digest username=\""+info.username+"\", realm=\""+info.realm+"\", nonce=\""+info.nonce+"\", uri=\""+info.uri+"\", response=\""+info.response+"\", qop=\""+info.qop+"\", nc="+info.nc+", cnonce=\""+info.cnonce+"\"";

    std::string request = cs.gen_request(info.method, info.uri, {{"Host", "ivos.ddns.net"}, {"Authorization", auth_args}});

    std::cout << request << '\n';


    std::cout << "opening connection...";
    if(cs.open_connection() != 0){
        std::cout << "fail\n";
        return 1;
    }
    std::cout << "success\n";


    std::cout << "sending request...";
    if(cs.send_request(request) != 0){
        std::cout << "fail\n";
        return 1;
    }
    std::cout << "success\n";

    std::cout << "printing response...\n\n";
    std::cout << cs.fetch_response() << '\n';
}
*/
