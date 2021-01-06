#include "Kinect.hpp"
#include <k4a/k4a.h>
#include <k4abt.hpp>
#include <spdlog/spdlog.h>

using namespace ktp::camera;

void ktp::camera::KinectTest() {
    uint32_t device_count = k4a_device_get_installed_count();
    k4abt::frame some_frame;
    if (device_count == 0)
    {
        spdlog::warn("No K4A devices found");
        spdlog::info("Body tracking test handle: {}", (std::size_t)some_frame.handle());
        return;
    }
    spdlog::info("Found {} K4A devices", device_count);
}