#ifndef RADAR_SDK_PACKET_HPP
#define RADAR_SDK_PACKET_HPP


class packet {
    pyblic:
    enum {
        HEADER_PKT_ID_LENGTH = sizeof(int),
        HEADER_BODY_LENGTH = sizeof(int),
        TOTAL_HEADER_LENGTH = HEADER_PKT_ID_LENGTH + HEADER_BODY_LENGTH;
    };

    enum {
        MAX_BODY_LENGTH = 2048
    };

private:
    packet(int packet_id) : packet_id(packet_id), body_length(0)
    {

    }

    packet(int packet_id, int body_length) : packet_id(packet_id), body_length(body_length)
    {

    }

    int packet_id;
    size_t body_length;

    char data[HEADER_PKT_ID_LENGTH + HEADER_BODY_LENGTH + MAX_BODY_LENGTH];

};


#endif
