#include "camera/Kinect.hpp"
#include <spdlog/spdlog.h>
#include <cxxopts.hpp>
#include <iostream>

using namespace ktp;

int main(int argc, char** argv) {
  cxxopts::Options options("test", "A brief description");

  options.add_options()
    ("p,playback", "Playback file", cxxopts::value<std::string>())
    ("h,help", "Print usage");

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    spdlog::info(options.help());
    return 0;
  }

  std::string playback_filepath;
  if (result.count("playback")){
    playback_filepath = result["playback"].as<std::string>();
    spdlog::info("Recognized playback file: {}", playback_filepath);
  }

  camera::KinectTest();
  return 0;
}
