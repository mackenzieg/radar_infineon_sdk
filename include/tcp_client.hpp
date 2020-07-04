#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <string>
#include <deque>
#include <boost/asio.hpp>

enum connection_static {
    NOT_CONNECTED,
    CONNECTED
};

class tcp_client {

public:
    tcp_client(std::string host, int port);

    void start();

    void connect();

    bool is_connected();

private:
    connection_status status = NOT_CONNECTED;

    std::deque

    boost::asio::io_service ioservice;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::socket socket;
    boost::asio::steady_timer timer{ioservice, boost::asio::chrono::seconds{2}};

    // Header is a single integer length
    const int HEADER_LEN = 0x04;

    void connect_handler(const boost::system::error_code& error);

    void reconnect_handler(const boost::system::error_code& error);

    std::string get_remote_address_string();


};


#endif //TCP_CLIENT_HPP
