#include "tcp_client.hpp"

#include <iostream>
#include <boost/chrono.hpp>

tcpclient::tcpclient(std::string host, int port) :
        endpoint(boost::asio::ip::address::from_string(host), port), socket(ioservice)
{

    std::cout << "Initiating tcp client" << std::endl;
}

void tcp_client::start()
{
    connect();
    ioservice.run();
}

void tcp_client::connect()
{
    socket.async_connect(endpoint, std::bind(&tcp_client::connect_handler, this, std::placeholders::_1))
}

bool tcp_client::is_connected()
{
    return status != NOT_CONNECTED;
}

void tcp_client::connect_handler(const boost::system::error_code &error)
{
    if (!error) {
        status = CONNECTED;
        cout << "Connected to " << get_remote_address_string() << endl;
        // Start here
    } else {
        status = NOT_CONNECTED;
        socket.close();
        timer.expires_from_now(boost::asio::chrono::seconds{2});
        timer.async_wait(&tcp_client::reconnect_handler, this, std::placeholders::_1))
    }
}

void tcp_client::reconnect_handler(const boost::system::error_code &error)
{
    connect();
}

std::string tcpclient::get_remote_address_string() {
    return _socket.remote_endpoint().address().to_string() + ":" + std::to_string(_socket.remote_endpoint().port());
}