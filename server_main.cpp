#include <iostream>
#include "common_socket.h"
#include "common_server.h"


int main(int argc, char * argv[]) {

    try {
        Server server;
        server.execute(argv[1]);
    }
    catch (const std::exception & e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}