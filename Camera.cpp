#include "Camera.h"

lb::Camera::Camera(const std::string& ip, const std::string& port) : cs(ip, port) {
    cs.open_connection();
}
lb::Camera::~Camera(){
    cs.close_connection();
}