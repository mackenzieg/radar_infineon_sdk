#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "ifxRadarSDK.h"
#include "ifxRadar_Error.h"
#include "dsp.hpp"
#include "radar_control.hpp"
#include "radar_config.hpp"

#include "TCPClient.h"

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT "4242"
#define HOST "192.178.0.101"

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

void connect_server(int sock, sockaddr_in* addr)
{
    while  (true)
    {
        cout << "Trying connection to host" << endl;
        int val = connect(sock, (struct sockaddr *)addr, sizeof(addr));
        cout << val << endl;
        if (val >= 0)
        {
            return;
        }
        cout << "Failed, retrying in 1000ms" << endl;
        sleep_ms(1000);
    }
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

uint32_t parseIPV4string(char* ipAddress) {
    char ipbytes[4];
    sscanf(ipAddress, "%uhh.%uhh.%uhh.%uhh", &ipbytes[3], &ipbytes[2], &ipbytes[1], &ipbytes[0]);
    return ipbytes[0] | ipbytes[1] << 8 | ipbytes[2] << 16 | ipbytes[3] << 24;
}

auto LogPrinter = [](const std::string& strLogMsg) { std::cout << strLogMsg << std::endl;  };


int main(int argc, char** argv)
{
    signal(SIGINT, signal_handle);

    cout << "Running Radar SDK version: " << ifx_radar_sdk_get_version_string() << endl;

    cout << "Creating device handle and dsp chain" << endl;

    radar_config rc;

    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);

    const char ip[] = "192.168.0.101";
    int portNum = 4242;

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    inet_aton(ip, &server_addr.sin_addr);

    connect_server(sock_cli, &server_addr);

    json config;
    config["packet_type"] = "configuration";
    config["sdk_version"] = ifx_radar_sdk_get_version_string();
    config["config"] = rc.create_json();

    cout << config << endl;

    radar_control radar_control(&rc);

    dsp dsp(&rc);

    string config_str = config.dump();

    cout << "Sending configuration file" << endl;

    send_string(sock_cli, config_str);

    cout << "Starting loop" << endl;

    ifx_Error_t ret = IFX_OK;
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

        send_string(sock_cli, data_str);
    }

	cout << "Closing connection" << endl;

    return 0;
}
