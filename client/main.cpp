#include <iostream>
#include "basic_client.h"

int main(int argc, char const *argv[]) {
    
    std::cout << "Client started" << std::endl;



    auto client = basic_client("127.0.0.1", 9090);

    client.send("hello from client");
    std::cout << client.receive() << std::endl;
    client.disconnect();

    return 0;
}
