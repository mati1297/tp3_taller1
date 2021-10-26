#include <iostream>
#include "common_socket.h"
#include "common_protocol.h"
#include "client_client.h"

#define ARGUMENTS_SIZE 2

int main(int argc, char * argv[]) {
    if (argc != ARGUMENTS_SIZE + 1){
        std::cerr << "Error: la cantidad de argumentos debe ser "
                  << ARGUMENTS_SIZE << std::endl;
        return EXIT_FAILURE;
    }

    // Se ejecuta el cliente.
    try {
        Client client(argv[1], argv[2]);
        client.execute();
    }
    // Se catchea e imprime el error.
    catch(std::exception & e){
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...){
        std::cerr << "Error desconocido" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

