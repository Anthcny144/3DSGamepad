#pragma once
#include <3ds.h>
#include "Input.hpp"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <string.h>

struct Packet {
    Input input;
    int frequency;
    int packet_id;
};

namespace Network {
    extern u32 *SOC_buffer;
    extern int sock;
    extern sockaddr_in dest_addr;

    extern std::string ds_ip;
    extern std::string dest_ip;
    extern bool valid_dest_ip;
    extern int port;
    extern int frequency;
    extern int packet_id;

    extern std::string error;

    extern const u32 align, buffer_size;

    void init();
    void exit();
    void get_console_ip();
    int init_buffer();
    Result init_soc_service();
    int init_sock();
    void set_dest();
    int send(Packet& packet);
    
    bool is_valid_ip(const std::string& ip);
};
