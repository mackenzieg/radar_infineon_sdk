#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "ifxRadarSDK.h"
#include "ifxRadar_Error.h"
#include "dsp.hpp"
#include "radar_control.hpp"
#include "radar_config.hpp"


#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void sleep_ms(int sleepMs)
{
#ifdef LINUX
    usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef WINDOWS
    Sleep(sleepMs);
#endif
}

bool running = true;

void signal_handle(int sig)
{
    if (sig != SIGINT)
        return;

    running = false;
}

int main(int argc, char** argv)
{
    signal(SIGINT, signal_handle);

    cout << "Running Radar SDK version: " << ifx_radar_sdk_get_version_string() << endl;

    cout << "Creating device handle and dsp chain" << endl;

    radar_config rc;

    json config;
    config["packet_type"] = "configuration";
    config["sdk_version"] = ifx_radar_sdk_get_version_string();
    config["config"] = rc.create_json();

    cout << config << endl;

    radar_control radar_control(&rc);

    dsp dsp(&rc);

    cout << "Starting Calibration" << endl;

    ifx_Error_t ret = IFX_OK;
	while (running)
    {
        ret = radar_control.pull_frame();

        if (ret != IFX_OK)
        {
            cout << "Waiting" << endl;
            continue;
        }

        ifx_Frame_t frame = radar_control.get_frame();

        dsp.run(frame);
    }

	cout << "Closing connection" << endl;

    return 0;
}
