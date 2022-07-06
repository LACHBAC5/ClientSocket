#include "Camera.h"

#include <iostream>
#include <vector>

int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT, argv[3]=USERNAME, argv[4]=PASSWORD
    lb::Camera a(argv[1], argv[2], argv[3], argv[4]);
    a.load_from_file("cgis.xml");

    std::string value;

    bool quit = false;
    while(!quit){
        a.print_layer();

        std::string message; std::cout << "\ninput> ";
        std::cin >> message;

        if(message == "prev"){
            a.previous();
        }
        else if(message == "quit"){
            quit = true;
        }
        else
        {
            if(a.get_root_name() == "parameter"){
                std::cout << "value>"; std::cin>>value;

                std::cout << value << '\n';
            }
            else
            {
                a.next(message);
            }
        }
    }
}