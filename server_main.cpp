#include <iostream>
#include "common_socket.h"


int main(int argc, char * argv[]) {
    Socket socket;
    Socket peer;

    try {
        socket.bindAndListen(argv[1], 8);
        peer = socket.accept();
    }
    catch(std::exception & e){
        std::cerr << e.what() << std::endl;
    }

    Message msg;

    peer.receive(msg, 10);

    msg.print();
}