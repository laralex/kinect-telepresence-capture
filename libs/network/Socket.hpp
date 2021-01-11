#pragma once

#include <string>
#include <string_view>
#include <optional>

namespace ktp { namespace network {
    class Socket {
    public:
        static std::optional<Socket> try_make(std::string_view host, std::uint16_t port) noexcept {
            if (is_port_valid(port) && is_host_valid(host)) {
                return { Socket(host, port) };
            }
            return std::nullopt;
        }

        std::uint16_t get_port() const noexcept {
            return this->port;
        }

        const std::string& get_host() const noexcept {
            return this->host;
        }

        static bool is_port_valid(std::uint16_t port) noexcept {
            return port <= 65353;
        }

        static bool is_host_valid(std::string_view host) noexcept {
            return true;
        }
    protected:
        Socket(std::string_view host, std::uint16_t port)
            : host(host), port(port) { }

        std::string host;
        std::uint16_t port;
    };

    class TcpSocket : public Socket {};
    class UdpSocket : public Socket {};
}}