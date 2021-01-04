#include "network/Packet.hpp"
#include "camera/Kinect.hpp"
#include <spdlog/spdlog.h>
#include <iostream>

using namespace ktp;

int main(int argc, char** argv) {
    spdlog::info(argc);
    spdlog::info(argv[0]);
    camera::KinectTest();
    using namespace ktp::network;
    Packet p;
    p.set_data(42+argc);

    return 0;
}
