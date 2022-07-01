#include "Camera.h"

#include <iostream>

int main(int argc, char *argv[]){
    lb::Camera c(argv[1], argv[2], argv[3], argv[4]); // ip port user password
    std::cout << c.send_request("GET", "/stw-cgi/image.cgi?msubmenu=imageenhancements&action=set&Channel=0&Brightness=1") << '\n';
}