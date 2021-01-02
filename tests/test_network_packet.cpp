#include "network/Packet.hpp"
#include <gtest/gtest.h>

using namespace ktp::network;

GTEST_TEST(Packet, SetData)
{
    Packet p;
    p.set_data(42);
    ASSERT_EQ(p.data, 42);
}