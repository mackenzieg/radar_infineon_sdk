#ifndef RADAR_H
#define RADAR_H

#include "ifxRadar_DeviceConfig.h"
#include "ifxRadar_DeviceControl.h"
#include "ifxRadar_Error.h"
#include "device_config.hpp"

class radar
{
    public:
        radar();
        virtual ~radar();

        ifx_Error_t pull_frame();

        ifx_Frame_t* get_frame();

    protected:

    private:
        device_config m_device_config;
        # SDK Device handle
        ifx_Device_Handle_t m_device_handle;
        # SDK Frame
        ifx_Frame_t m_frame;

};

#endif // RADAR_H
