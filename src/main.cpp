#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "ifxRadarSDK.h"
#include "ifxRadar_Error.h"
#include "radar_control.hpp"
#include "range_spectrum.hpp"

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

	printf("Running Radar SDK version: %s\n", ifx_radar_sdk_get_version_string());

    radar_config rc;

    radar_control radar_control(&rc);

    range_spectrum range_spectrum(&rc);
    printf("Pulling frame from device\n");

    ifx_Error_t ret = IFX_OK;
	while (running)
    {
        ret = radar_control.pull_frame();
        // No data avaliable data to pull
        if (ret == IFX_ERROR_FIFO_OVERFLOW)
        {
            continue;
        }

        ifx_Frame_t* frame = radar_control.get_frame();

        printf("Num of active rx: %d\n", frame->num_rx);

        range_spectrum.run(&(frame->rx_data[0]));
    }

    printf("Closing connection\n");

    return 0;
}
