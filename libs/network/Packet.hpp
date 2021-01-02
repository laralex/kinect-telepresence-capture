#pragma once

namespace ktp {
    namespace network {
        struct Packet {
            int data;

            void set_data(int new_data);
        };
    }
}
