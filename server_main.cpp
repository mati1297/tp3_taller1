#include <iostream>
#include "common_socket.h"
#include "server_server.h"

#define ARGUMENTS_SIZE 1

int main(int argc, char * argv[]) {
    if (argc != ARGUMENTS_SIZE + 1){
        std::cerr << "Error: la cantidad de argumentos debe ser "
                  << ARGUMENTS_SIZE << std::endl;
        return EXIT_FAILURE;
    }

    try {
        Server server;
        server.execute(argv[1]);
    }
    catch(const std::exception & e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
