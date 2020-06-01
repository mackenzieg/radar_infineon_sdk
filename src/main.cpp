#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "ifxRadarSDK.h"
#include "ifxRadar_Error.h"
#include "dsp.hpp"
#include "radar_control.hpp"
#include "radar_config.hpp"

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

    dsp dsp(&rc);
    printf("Pulling frame from device\n");

    ifx_Error_t ret = IFX_OK;
	while (running)
    {
        ret = radar_control.pull_frame();
        // TODO check for buffer overflow instead of everything
        if (ret != IFX_OK)
        {
            continue;
        }

        ifx_Frame_t frame = radar_control.get_frame();

        dsp.run(frame);
    }

    printf("Closing connection\n");

    return 0;
}
