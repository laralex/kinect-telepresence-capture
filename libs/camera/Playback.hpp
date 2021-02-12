#include <k4a/k4a.h>
#include <k4arecord/record.h>
#include <k4arecord/playback.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <string>
#include <map>
#include <functional>

namespace ktp { 
namespace camera {

using ImageCallback = std::function<void(k4a_image_t, size_t)>;

class PlaybackHandleRAII {
public:

  static std::optional<PlaybackHandleRAII> OpenFile(const std::string& path) {
    k4a_playback_t playback_handle = NULL;
    if (k4a_playback_open(path.c_str(), &playback_handle) != K4A_RESULT_SUCCEEDED)
    {
        return std::nullopt;
    }
    return { PlaybackHandleRAII(playback_handle) }; 
  }
 
  ~PlaybackHandleRAII() {
    //k4a_playback_close(m_playback_handle);
  }

  PlaybackHandleRAII(PlaybackHandleRAII&& other) noexcept // move constructor
    : m_playback_handle(other.m_playback_handle) {

  }

  PlaybackHandleRAII& operator=(PlaybackHandleRAII&& other) noexcept {
    this->m_playback_handle = other.m_playback_handle;
  }

  PlaybackHandleRAII(const PlaybackHandleRAII& other) noexcept = delete;
  PlaybackHandleRAII& operator=(const PlaybackHandleRAII& other) noexcept = delete;
  
  k4a_playback_t Handle() const { 
    return m_playback_handle; 
  }

private:
  PlaybackHandleRAII(k4a_playback_t handle) : m_playback_handle(handle) {}
  k4a_playback_t m_playback_handle;
};

struct PlaybackTags {
  std::map<const char*, std::string> m_tags_map;

  std::optional<const char*> K4A_COLOR_MODE() { return GetTagValue("K4A_COLOR_MODE"); }
  std::optional<const char*> K4A_DEPTH_MODE() { return GetTagValue("K4A_DEPTH_MODE"); }
  std::optional<const char*> K4A_IR_MODE() { return GetTagValue("K4A_IR_MODE"); }
  std::optional<const char*> K4A_IMU_MODE() { return GetTagValue("K4A_IMU_MODE"); }
  std::optional<const char*> K4A_CALIBRATION_FILE() { return GetTagValue("K4A_CALIBRATION_FILE"); }
  std::optional<const char*> K4A_DEPTH_DELAY_NS() { return GetTagValue("K4A_DEPTH_DELAY_NS"); }
  std::optional<const char*> K4A_WIRED_SYNC_MODE() { return GetTagValue("K4A_WIRED_SYNC_MODE"); }
  std::optional<const char*> K4A_SUBORDINATE_DELAY_NS() { return GetTagValue("K4A_SUBORDINATE_DELAY_NS"); }
  std::optional<const char*> K4A_COLOR_FIRMWARE_VERSION() { return GetTagValue("K4A_COLOR_FIRMWARE_VERSION"); }
  std::optional<const char*> K4A_DEPTH_FIRMWARE_VERSION() { return GetTagValue("K4A_DEPTH_FIRMWARE_VERSION"); }
  std::optional<const char*> K4A_DEVICE_SERIAL_NUMBER() { return GetTagValue("K4A_DEVICE_SERIAL_NUMBER"); }
  std::optional<const char*> K4A_START_OFFSET_NS() { return GetTagValue("K4A_START_OFFSET_NS"); }
  std::optional<const char*> K4A_COLOR_TRACK() { return GetTagValue("K4A_COLOR_TRACK"); }
  std::optional<const char*> K4A_DEPTH_TRACK() { return GetTagValue("K4A_DEPTH_TRACK"); }
  std::optional<const char*> K4A_IR_TRACK() { return GetTagValue("K4A_IR_TRACK"); }
  std::optional<const char*> K4A_IMU_TRACK() { return GetTagValue("K4A_IMU_TRACK"); }
  
  
  std::optional<const char*> GetTagValue(const char* tag) {
    auto item_iter = m_tags_map.find(tag);
    if (item_iter == m_tags_map.end()) {
      return std::nullopt;
    }
    return { item_iter->second.c_str() };
  }

};

std::optional<size_t> TestOpenPlayback(const std::string& path){
  auto handle = PlaybackHandleRAII::OpenFile(path);
  if (!handle) {
    return std::nullopt;
  }
  uint64_t recording_length = k4a_playback_get_last_timestamp_usec(handle->Handle());
  return { recording_length / 1000000 };
}

std::optional<const char*> ProcessPlaybackFrames(k4a_playback_t handle, ImageCallback color_callback, ImageCallback depth_callback) {
  k4a_capture_t capture = NULL;
  k4a_image_t color, depth;
  k4a_stream_result_t result = K4A_STREAM_RESULT_SUCCEEDED;
  size_t frame_idx = 0;
  while (result == K4A_STREAM_RESULT_SUCCEEDED)
  {
    result = k4a_playback_get_next_capture(handle, &capture);
    if (result == K4A_STREAM_RESULT_SUCCEEDED)
    {
      auto color = k4a_capture_get_color_image(capture);
      auto depth = k4a_capture_get_depth_image(capture);
      if (color) {
        color_callback(color, frame_idx);
        k4a_image_release(color);
      }

      if (depth) {
        depth_callback(depth, frame_idx);
        k4a_image_release(depth);
      }

      k4a_capture_release(capture);
    }
    else if (result == K4A_STREAM_RESULT_EOF)
    {
      return std::nullopt; // end of file
    }

    ++frame_idx;
  }
  if (result == K4A_STREAM_RESULT_FAILED)
  {
    return { "Failed to read entire playback" };
  }
  return { "Unknown result. Neither suceeded nor failed to read playback" };
}

std::optional<std::string> ExtractPlaybackTag(k4a_playback_t playback_handle, const char* tag, size_t maximum_tag_size = 256) {
  size_t tag_size = maximum_tag_size; // upper bound
  std::string result;
  result.resize(tag_size);

  k4a_buffer_result_t buffer_result = 
    k4a_playback_get_tag(playback_handle, tag, &(result[0]), &tag_size);
  
  if (buffer_result != K4A_BUFFER_RESULT_SUCCEEDED)
  {
    return std::nullopt;
  }

  return { result };
}

PlaybackTags ExtractPlaybackTags(k4a_playback_t playback_handle) {
  const char* all_tags[] = {
    "K4A_COLOR_MODE", "K4A_COLOR_MODE", 
    "K4A_IR_MODE", "K4A_IMU_MODE",
    "K4A_CALIBRATION_FILE", "K4A_DEPTH_DELAY_NS", 
    "K4A_WIRED_SYNC_MODE", "K4A_SUBORDINATE_DELAY_NS",
    "K4A_COLOR_FIRMWARE_VERSION", "K4A_DEPTH_FIRMWARE_VERSION",
    "K4A_DEVICE_SERIAL_NUMBER", "K4A_START_OFFSET_NS",
    "K4A_COLOR_TRACK", "K4A_DEPTH_TRACK",
    "K4A_IR_TRACK", "K4A_IMU_TRACK"
  };

  PlaybackTags result; 
  for(auto tag : all_tags) {
    auto tag_value = ExtractPlaybackTag(playback_handle, tag);
    if (tag_value) {
      result.m_tags_map.emplace(tag, *tag_value);
    }
  } 

  return result;
}

}}