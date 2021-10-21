#include <iostream>
#include "common_socket.h"
#include "common_protocol.h"
#include "client_client.h"


int main(int argc, char * argv[]) {
    try {
        Client client(argv[1], argv[2]);
        client.execute();
    }
    catch(std::exception & e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }


}

