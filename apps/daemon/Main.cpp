#include "Config.hpp"
#include "camera/Kinect.hpp"
#include <spdlog/spdlog.h>
#include <cxxopts.hpp>
#include <iostream>

using namespace ktp;

int main(int argc, char** argv) {
  cxxopts::Options options("ktp_capture_daemon", "Kinect Telepresence -- Camera data processing deamon");
  options.allow_unrecognised_options();
  options.add_options()
    ("f,file", "Playback filepath", 
      cxxopts::value<std::string>())
    ("p,port", "Local TCP port to open/listen by this daemon", 
      cxxopts::value<std::uint16_t>()->default_value("44222"))
    ("e,endpoint", "Remote endpoint (IP:port) of immersion side in a form \"111.222.333.333:1234\"", 
      cxxopts::value<std::string>())
    ("h,help", "Print usage");


  cxxopts::ParseResult result;
  try {
    result = options.parse(argc, argv);
  } catch(cxxopts::OptionParseException e) {
    spdlog::error("CXXOPTS library error during parsing of command line arguments: {}", e.what());
    return 1;
  }

  if (result.count("help")) {
    spdlog::info(options.help());
    return 0;
  }

  auto daemon_config = DaemonConfig::ParseFromArgs(result, argc, argv);
  if (!daemon_config) {
    spdlog::critical("Could not extract all necessary configuration settings from command line arguments. Usage:\n{}", options.help());
    return 0;
  }
  spdlog::info("Parsed configuration from command line arguments");
  if (daemon_config->playback_filepath) {
    spdlog::info("> Playback file: {}", daemon_config->playback_filepath.value());
  }
  spdlog::info("> Local endpoint  {}:{}", daemon_config->local_endpoint.address().to_string(), daemon_config->local_endpoint.port());
  spdlog::info("> Remote endpoint {}:{}", daemon_config->remote_endpoint.address().to_string(), daemon_config->remote_endpoint.port());

  camera::KinectTest();
  return 0;
}
