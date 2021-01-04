#include "Kinect.hpp"
#include <k4a/k4a.h>
#include <spdlog/spdlog.h>

using namespace ktp::camera;

void ktp::camera::KinectTest() {
    uint32_t device_count = k4a_device_get_installed_count();
    if (device_count == 0)
    {
        spdlog::warn("No K4A devices found");
        return;
    }
    spdlog::info("Found {} K4A devices", device_count);
}