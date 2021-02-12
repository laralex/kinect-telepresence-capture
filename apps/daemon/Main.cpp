#include "Config.hpp"
#include "camera/Kinect.hpp"
#include "camera/Playback.hpp"
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
  spdlog::info("> Config - Local endpoint  {}:{}", 
    daemon_config->local_endpoint.address().to_string(), daemon_config->local_endpoint.port());
  spdlog::info("> COnfig - Remote endpoint {}:{}", 
    daemon_config->remote_endpoint.address().to_string(), daemon_config->remote_endpoint.port());
    
  if (daemon_config->playback_filepath) {
    spdlog::info("> Config - Playback file: {}", daemon_config->playback_filepath.value());
    auto handle = camera::PlaybackHandleRAII::OpenFile(daemon_config->playback_filepath.value())   ;
    if (handle) {
      auto tags = camera::ExtractPlaybackTags(handle->Handle());
      spdlog::info("> Camera modes"); 
      spdlog::info("K4A_COLOR_MODE = {}", tags.K4A_COLOR_MODE().value_or("<UKNOWN>"));    
      spdlog::info("K4A_DEPTH_MODE = {}", tags.K4A_DEPTH_MODE().value_or("<UKNOWN>"));    
      spdlog::info("K4A_IR_MODE = {}", tags.K4A_IR_MODE().value_or("<UKNOWN>"));    
      spdlog::info("K4A_IMU_MODE = {}", tags.K4A_IMU_MODE().value_or("<UKNOWN>")); 
      spdlog::info("> Playback delays");   
      spdlog::info("(delay from color channel) K4A_DEPTH_DELAY_NS = {}", tags.K4A_DEPTH_DELAY_NS().value_or("<UKNOWN>"));    
      spdlog::info("(delay from external sync pulse) K4A_SUBORDINATE_DELAY_NS = {}", tags.K4A_SUBORDINATE_DELAY_NS().value_or("<UKNOWN>"));    
      spdlog::info("(multiple cameras delay) K4A_START_OFFSET_NS = {}", tags.K4A_START_OFFSET_NS().value_or("<UKNOWN>"));    
      spdlog::info("> Playback hardware:");
      spdlog::info("K4A_DEVICE_SERIAL_NUMBER = {}", tags.K4A_DEVICE_SERIAL_NUMBER().value_or("<UKNOWN>"));   
      spdlog::info("K4A_COLOR_FIRMWARE_VERSION = {}", tags.K4A_COLOR_FIRMWARE_VERSION().value_or("<UKNOWN>"));    
      spdlog::info("K4A_DEPTH_FIRMWARE_VERSION = {}", tags.K4A_DEPTH_FIRMWARE_VERSION().value_or("<UKNOWN>"));   
      spdlog::info("> .mkv tracks identifiers:"); 
      spdlog::info("K4A_COLOR_TRACK = {}", tags.K4A_COLOR_TRACK().value_or("<UKNOWN>"));    
      spdlog::info("K4A_DEPTH_TRACK = {}", tags.K4A_DEPTH_TRACK().value_or("<UKNOWN>"));    
      spdlog::info("K4A_IR_TRACK = {}", tags.K4A_IR_TRACK().value_or("<UKNOWN>"));    
      spdlog::info("K4A_IMU_TRACK = {}", tags.K4A_IMU_TRACK().value_or("<UKNOWN>"));  
      spdlog::info("Extracting playback frames");
      
      camera::ImageCallback color_callback = [](auto color, size_t frame_idx) {
        spdlog::info("#{} Color frame {}x{} stride {}",
          frame_idx,
          k4a_image_get_height_pixels(color),
          k4a_image_get_width_pixels(color),
          k4a_image_get_stride_bytes(color));
      };

      camera::ImageCallback depth_callback = [](auto depth, size_t frame_idx) {
        spdlog::info("#{} Depth frame {}x{}",
          frame_idx,
          k4a_image_get_height_pixels(depth),
          k4a_image_get_width_pixels(depth));
      };

      auto error_code = camera::ProcessPlaybackFrames(handle->Handle(), color_callback, depth_callback);
      if (error_code) {
        spdlog::error(*error_code);
      }
    } else {
      spdlog::error("Failed to open the playback file");
    }

    
  }

  return 0;
}
