#ifndef RADAR_CONTROL_H
#define RADAR_CONTROL_H

#include "ifxRadar_DeviceControl.h"
#include "ifxRadar_Error.h"
#include "radar_config.hpp"

class radar_control
{
    public:
        radar_control(radar_config *rc);
        virtual ~radar_control();

        ifx_Error_t pull_frame();

        ifx_Frame_t get_frame();

        ifx_Device_Handle_t get_device_handle();

    protected:

    private:
        radar_config* m_radar_config;
        // SDK Device handle
        ifx_Device_Handle_t m_device_handle;
        // SDK Frame
        ifx_Frame_t m_frame;

};

#endif // RADAR_CONTROL_H
