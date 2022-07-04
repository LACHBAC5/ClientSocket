#include "ClientSocket.h"
#include "Camera.h"

#include <iostream>


int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT, argv[3]=USERNAME, argv[4]=PASSWORD


    lb::Camera c(argv[1], argv[2], argv[3], argv[4]);
    std::cout << c.send_request("image.cgi", "imageenhancements", "set", {{"Brightness", "99"}}) << '\n';
}