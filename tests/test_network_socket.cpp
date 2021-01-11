#include "network/Socket.hpp"
#include <gtest/gtest.h>

using namespace ktp::network;

GTEST_TEST(Socket, TryMake)
{
    auto host = "127.0.0.1";
    auto port = 1234;
    {
        auto t_opt = TcpSocket::try_make(host, port);
        ASSERT_TRUE(t_opt);
        ASSERT_EQ(t_opt->get_host(), host);
        ASSERT_EQ(t_opt->get_port(), port);
    }
    port = 65535;
    {
        auto t_opt = TcpSocket::try_make(host, port);
        ASSERT_FALSE(t_opt);
    }
}