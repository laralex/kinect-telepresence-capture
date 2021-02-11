#include <asio.hpp>
#include <cxxopts.hpp>

#include <string>
#include <optional>
#include <utility>

namespace ktp {

struct DaemonConfig {
  asio::ip::tcp::endpoint local_endpoint, remote_endpoint;
  std::optional<std::string> playback_filepath;

  static std::optional<DaemonConfig> ParseFromArgs(const cxxopts::ParseResult& result, int argc, char** argv) {
    DaemonConfig config;

    if (result.count("file")) {
      config.playback_filepath = result["file"].as<std::string>();
    }

    if (result.count("port") * result.count("endpoint") == 0) {
      return std::nullopt;
    }
    
    config.local_endpoint.address(asio::ip::address::from_string("127.0.0.1"));
    config.local_endpoint.port(result["port"].as<std::uint16_t>());

    if (config.local_endpoint.port() <= 0 || config.local_endpoint.port() >= (1 << 16)) {
      return std::nullopt;
    }

    auto remote_endpoint_str = result["endpoint"].as<std::string>();
    auto colon_position = remote_endpoint_str.find(':');

    if (colon_position < 0) {
      return std::nullopt;
    }

    asio::error_code parse_error;
    config.remote_endpoint.address(asio::ip::address::from_string(
      remote_endpoint_str.substr(0, colon_position), parse_error));
    if (parse_error) {
      return std::nullopt;
    }

    std::int32_t remote_port = // string-to-integer
      std::stoul(remote_endpoint_str.substr(colon_position + 1));
    if (remote_port <= 0 || remote_port >= (1 << 16)) {
      return std::nullopt;
    }
    config.remote_endpoint.port(remote_port);

    return config;
  }
}; 

} // namespace ktp