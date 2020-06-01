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
    printf("Starting\n");

    signal(SIGINT, signal_handle);

	printf("Running Radar SDK version: %s\n", ifx_radar_sdk_get_version_string());

    radar_config rc;

    radar_control radar_control(&rc);

    printf("Creating device handle and dsp processing chain\n");
    dsp dsp(&rc);

    sleep_ms(1000);

    printf("Starting DSP process\n");

    ifx_Error_t ret = IFX_OK;
	while (running)
    {
        ret = radar_control.pull_frame();
        // TODO check for buffer overflow instead of everything
        if (ret == IFX_ERROR_FIFO_OVERFLOW)
        {
            continue;
        }

        ifx_Frame_t frame = radar_control.get_frame();

        dsp.run(frame);
    }

    printf("Closing connection\n");

    return 0;
}
