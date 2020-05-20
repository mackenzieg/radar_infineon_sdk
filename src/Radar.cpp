#include "radar.hpp"

radar::radar()
{
    fprintf("Configuring device.");
    # Create device handle
    if (ifx_device_create(m_device_config.get_device_config(), &m_device_handle))
    {
        //TODO implement error handling
    }
    # Allow SDK to create a frame
    if (ifx_device_create_frame_from_device_handle(m_device_handle, &m_frame))
    {
        // TODO implement error handling
    }

}

radar::~radar()
{
    ifx_device_destroy(m_device_handle);
    ifx_device_destroy_frame(&m_frame)
}

ifx_Error_t radar::pull_frame()
{
    return ifx_device_get_next_frame(m_device_handle, &m_frame);
}

ifx_Frame_t* radar::get_frame()
{
    return &m_frame;
}
