#include "network/Packet.hpp"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << argc << ' ' << argv[0] << std::endl;
    
    using namespace ktp::network;
    Packet p;
    p.set_data(42+argc);
    
    return 0;
}
