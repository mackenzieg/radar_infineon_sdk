#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "ifxRadarSDK.h"
#include "ifxRadar_Error.h"
#include "dsp.hpp"
#include "radar_control.hpp"
#include "radar_config.hpp"

#include <boost/asio.hpp>

#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

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

#define BUFF_SIZE 1024

char send_buff[BUFF_SIZE];

int send_data(int sock, const char *buff, int *length)
{
    int total = 0;
    int bytes_left = *length;
    int n;

    // Send length of packet first
    n = send(sock, length, 4, 0);

    while (total < *length)
    {
        n = send(sock, buff+total, bytes_left, 0);
        if (n == -1)
        {
            break;
        }

        total += n;
        bytes_left -= n;
    }

    *length = total;

    return n == -1 ? -1 : 0;
}

void send_string(int sock, string data)
{
    int length = (int) data.length();
    send_data(sock, data.c_str(), &length);
}

auto LogPrinter = [](const std::string& strLogMsg) { std::cout << strLogMsg << std::endl;  };

int main(int argc, char** argv)
{
    signal(SIGINT, signal_handle);

    cout << "Running Radar SDK version: " << ifx_radar_sdk_get_version_string() << endl;

    cout << "Creating device handle and dsp chain" << endl;

    radar_config rc;

    boost::asio::io_service io_service;

    tcp::socket socket(io_service);

    socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("192.168.0.101"), 4242 ));
    boost::system::error_code error;

    json config;
    config["packet_type"] = "configuration";
    config["sdk_version"] = ifx_radar_sdk_get_version_string();
    config["config"] = rc.create_json();

    radar_control radar_control(&rc);

    dsp dsp(&rc);

    string config_str = config.dump();

    cout << "Sending configuration file" << endl;

    uint32_t len = config_str.length();
    boost::asio::write( socket, boost::asio::buffer(&len, 4), error );
    boost::asio::write( socket, boost::asio::buffer(config_str), error );

    cout << "Starting loop" << endl;

    ifx_Error_t ret = IFX_OK;

    int x = 0;
	while (running)
    {
        ret = radar_control.pull_frame();

        if (ret != IFX_OK)
        {
            continue;
        }

        ifx_Frame_t frame = radar_control.get_frame();



        json data;
        data["packet_type"] = "data";
        data["data"] = dsp.run(frame);

        string data_str = data.dump();
        uint32_t length = data_str.length();

        uint32_t len = data_str.length();
        boost::asio::write( socket, boost::asio::buffer(&len, 4), error );
        boost::asio::write( socket, boost::asio::buffer(data_str), error );

        int fr = rc.get_device_metrics()->m_frame_rate * 2;

        if (x == 0)
        {
            cout << "Sending data..." << endl;
        }

        ++x;
        x %= fr;
    }

	cout << "Closing connection" << endl;

    return 0;
}
