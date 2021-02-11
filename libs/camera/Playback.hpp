#include <k4a/k4a.h>
#include <k4arecord/record.h>
#include <k4arecord/playback.h>
#include <string>
#include <iostream>

namespace ktp { namespace camera {

    void OpenKinectPlaybackFile(const std::string& path){
        k4a_playback_t playback_handle = NULL;
        if (k4a_playback_open(path.c_str(), &playback_handle) != K4A_RESULT_SUCCEEDED)
        {
            printf("Failed to open recording\n");
            return;
        }

        uint64_t recording_length = k4a_playback_get_last_timestamp_usec(playback_handle);
        printf("Recording is %lld seconds long\n", recording_length / 1000000);

        k4a_playback_close(playback_handle);
    }

}}