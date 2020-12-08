#include <iostream>
#include "basic_server.h"

int main(int argc, char const *argv[]) {
    
    std::cout << "Server started" << std::endl;


    auto server = basic_server();
    server.run();

    return 0;
}
