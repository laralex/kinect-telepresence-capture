#include "network/Packet.hpp"
#include <spdlog/spdlog.h>
#include <iostream>

int main(int argc, char** argv) {
    spdlog::info(argc);
    spdlog::info(argv[0]);
    
    using namespace ktp::network;
    Packet p;
    p.set_data(42+argc);

    return 0;
}
