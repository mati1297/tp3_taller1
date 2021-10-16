#include <iostream>
#include "common_socket.h"
#include "common_protocol.h"


int main(int argc, char * argv[]) {
    Socket socket;

    try {
        socket.connect(argv[1], argv[2]);
    }
    catch(std::exception & e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    Protocol protocol(&socket);

    std::string queue;

    std::cin >> queue;

    std::string message;

    std::cin >> message;

    protocol.sendPush(queue, message);
}

