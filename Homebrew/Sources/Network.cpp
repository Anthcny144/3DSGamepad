#include <3ds.h>
#include "Network.hpp"

namespace Network {
    u32 *SOC_buffer = nullptr;
    int sock;
    sockaddr_in dest_addr;
    
    std::string ds_ip = "-";
    std::string dest_ip = "not set";
    bool valid_dest_ip = false;
    int port = 14418;
    int frequency = 60;
    int packet_id = 0;
    
    std::string error = "";
    
    const u32 align = 0x1000,
              buffer_size = 0x100000;
    
    void init() {    
        int res_buffer = init_buffer();
        if (res_buffer == -1)
            error = "Could not align SOC_buffer: " + std::to_string(res_buffer);

        if (error.empty()) {
            Result res_soc_service = init_soc_service();
            if (res_soc_service != 0)
                error = "Could not init SOC: " + std::to_string(res_soc_service);
        }
    
        if (error.empty()) {
            int res_sock = init_sock();
            if (res_sock < 0)
                error = "Could not init socket: " + std::to_string(res_sock);
        }
    }

    void exit() {
        if (sock >= 0) {
            closesocket(sock);
            sock = -1;
        }

        socExit();

        if (SOC_buffer != nullptr) {
            free(SOC_buffer);
            SOC_buffer = nullptr;
        }
    }

    void get_console_ip() {
        u32 ip = gethostid();
        ds_ip = std::to_string((ip >> 0) & 0xFF) + "." +
                std::to_string((ip >> 8) & 0xFF) + "." +
                std::to_string((ip >> 16) & 0xFF) + "." +
                std::to_string((ip >> 24) & 0xFF);
    }
    
    int init_buffer() {
        SOC_buffer = (u32*)memalign(align, buffer_size);
        if (!SOC_buffer)
            return -1;
    
        return 0;
    }
    
    Result init_soc_service() {
        Result ret;
    
        ret = socInit(SOC_buffer, buffer_size);
        if (ret != 0) {
            free(SOC_buffer);
            SOC_buffer = nullptr;
            return ret;
        }
    
        return 0;
    }
    
    int init_sock() {
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        return sock;
    }
    
    void set_dest() {
        sockaddr_in dest_addr;
        memset(&dest_addr, 0, sizeof(dest_addr));
    
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        dest_addr.sin_addr.s_addr = inet_addr(dest_ip.c_str());
        Network::dest_addr = dest_addr;
    }
    
    int send(Packet& packet) {
        return sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    }
    
    bool is_valid_ip(const std::string& ip) {
        s8 pts[3] = {-1, -1, -1};
        int found_pts = 0;
    
        for (size_t i = 0; i < ip.length(); i++) {
            if (ip[i] == '.') {
                if (found_pts == 3)
                    return false;
    
                pts[found_pts++] = static_cast<s8>(i);
            }
        }
    
        if (found_pts != 3)
            return false;
    
        // Segments
        size_t starts[4] = { 0, static_cast<size_t>(pts[0]) + 1, static_cast<size_t>(pts[1]) + 1, static_cast<size_t>(pts[2]) + 1 };
        size_t ends[4]   = { static_cast<size_t>(pts[0]), static_cast<size_t>(pts[1]), static_cast<size_t>(pts[2]), ip.length() };
    
        for (int i = 0; i < 4; i++) {
            size_t start = starts[i];
            size_t end = ends[i];
    
            // Empty segment
            if (start >= end)
                return false;
    
            size_t len = end - start;
    
            if (len > 3)
                return false;
    
            int val = 0;
            for (size_t j = start; j < end; j++) {
                char c = ip[j];
                if (c < '0' || c > '9')
                    return false;
    
                val = val * 10 + (c - '0');
            }
    
            if (val < 0 || val > 255)
                return false;
        }
    
        return true;
    }
};
