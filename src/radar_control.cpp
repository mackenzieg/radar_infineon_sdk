#include "radar_control.hpp"

radar_control::radar_control(radar_config* rc) : m_radar_config(rc)
{
    printf("Configuring device\n");
    // Create device handle
    if (ifx_device_create(m_radar_config->get_device_config(), &m_device_handle))
    {
        //TODO implement error handling
    }
    // Allow SDK to create a frame
    if (ifx_device_create_frame_from_device_handle(m_device_handle, &m_frame))
    {
        // TODO implement error handling
    }
}

radar_control::~radar_control()
{
    ifx_device_destroy(m_device_handle);
    ifx_device_destroy_frame(&m_frame);
}

ifx_Error_t radar_control::pull_frame()
{
    return ifx_device_get_next_frame(m_device_handle, &m_frame);
}

ifx_Frame_t* radar_control::get_frame()
{
    return &m_frame;
}

ifx_Device_Handle_t* radar_control::get_device_handle()
{
    return &m_device_handle;
}
