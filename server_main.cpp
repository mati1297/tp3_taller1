#include <iostream>
#include "common_socket.h"
#include "server_server.h"
#include "common_invalid_parameter_addr.h"

#define ARGUMENTS_SIZE 1

#define EXIT_SUCCESS 0
#define EXIT_BAD_PARAMETER 1
#define EXIT_ERROR 2

int main(int argc, char * argv[]) {
    if (argc != ARGUMENTS_SIZE + 1){
        std::cerr << "Error: la cantidad de argumentos debe ser "
                  << ARGUMENTS_SIZE << std::endl;
        return EXIT_BAD_PARAMETER;
    }
    // Se ejecuta el server.
    try {
        Server server(argv[1]);
        server.execute();
    }
    // Se catchean excepciones y se imprime el status.
    catch(const InvalidParameterAddr & e){
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_BAD_PARAMETER;
    }
    catch(const std::exception & e){
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_ERROR;
    }
    catch(...){
        std::cerr << "Error desconocido" << std::endl;
        return EXIT_ERROR;
    }

    return 0;
}
