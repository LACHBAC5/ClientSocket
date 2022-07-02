#include "Camera.h"

lb::Camera::Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password) : cs(ip, port) {
    info.username = username; info.password = password;
    info.cnonce = "024897c82e2d7cab"; info.nc = "1";
    info.method = "GET";
}

lb::Camera::~Camera() {}

std::string lb::Camera::send_request(const std::string& cgi, const std::string& msubmenu, const std::string& action, const std::string& channel, const std::string& setting, const std::string& value){
    info.uri = "/stw-cgi/"+cgi+"?msubmenu="+msubmenu+"&action="+action+"&Channel="+channel+"&"+setting+"="+value;
    info.response = cs.gen_auth_response_single(md5, info);

    std::string auth_args = "Digest username=\""+info.username+"\", realm=\""+info.realm+"\", nonce=\""+info.nonce+"\", uri=\""+info.uri+"\", response=\""+info.response+"\", qop=\""+info.qop+"\", nc="+info.nc+", cnonce=\""+info.cnonce+"\"";

    std::string request = cs.gen_request(info.method, info.uri, {{"Host", "ivos.ddns.net"}, {"Authorization", auth_args}});

    cs.open_connection();
    if(cs.send_request(request)!=0){
        return "";
    }

    std::string response = cs.fetch_response();

    if(cs.status_code(response) == 401){
        auto header_map = cs.htom(response);
        std::string auth = header_map.at("WWW-Authenticate");

        int pos = auth.find("realm=\"")+7;
        info.realm = auth.substr(pos, auth.find("\"", pos)-pos);
        pos = auth.find("nonce=\"")+7;
        info.nonce = auth.substr(pos, auth.find("\"", pos)-pos);
        pos = auth.find("qop=\"")+5;
        info.qop = auth.substr(pos, auth.find("\"", pos)-pos);

        info.response = cs.gen_auth_response_single(md5, info);
        auth_args = "Digest username=\""+info.username+"\", realm=\""+info.realm+"\", nonce=\""+info.nonce+"\", uri=\""+info.uri+"\", response=\""+info.response+"\", qop=\""+info.qop+"\", nc="+info.nc+", cnonce=\""+info.cnonce+"\"";
        request = cs.gen_request(info.method, info.uri, {{"Host", "ivos.ddns.net"}, {"Authorization", auth_args}});
        
        cs.open_connection();
        if(cs.send_request(request)!=0){
            return "";
        }

        response = cs.fetch_response();
    }
    info.nc = std::to_string(std::stoi(info.nc));

    return response;
}
