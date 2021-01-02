#include "Packet.hpp"

using namespace ktp::network;

void Packet::set_data(int new_data) {
    this->data = new_data;
}
