#include "frame_define.h"
#include "stdio.h"

char* send(const void* data_buf, const size_t size, const socket* dest_socket, const socket* src_socket, MAC dest, MAC src) {
    char* buf = (char*)malloc(1518);
    udp_header udp_h = parse_to_udp(dest_socket->port, src_socket->port, size, data_buf);
    ip_header ip_h = parse_to_ip(dest_socket->ip, src_socket->ip, size + 20 + 8);
    frame_header frame_h = parse_to_frame(dest, src, sizeof(ip_header) + sizeof(udp_header) + size);
    memcpy(buf, (char*)&frame_h, sizeof(frame_header));
    memcpy(buf + sizeof(frame_header), (char*)&ip_h, sizeof(ip_header));
    memcpy(buf + sizeof(frame_header) + sizeof(ip_header), (char*)&udp_h, sizeof(udp_header));
    memcpy(buf + sizeof(frame_header) + sizeof(ip_header) + sizeof(udp_header), data_buf, size);
    unsigned int crc = computeCRC((const unsigned char*)buf + sizeof(frame_header), sizeof(ip_header) + sizeof(udp_header) + size);
    memcpy(buf + sizeof(frame_header) + sizeof(ip_header) + sizeof(udp_header) + size, &crc, 4);
    return buf;
}

int receive(const void* buf, const socket* local_socket, MAC local_mac, void* data_buf, const size_t max_size) {
    unsigned short data_length = 0;
    int ret = SUCCESS;
    if(BORING_FAIL(ret = check_frame(buf, 14, local_mac))) {
        printf("check frame fail\n");
    } else if(BORING_FAIL(ret = check_ip(buf + 14, 20, local_socket->ip))) {
        printf("check ip fail\n");
    } else if(BORING_FAIL(ret = check_udp(buf + 34, local_socket->port, buf, &data_length))) {
        printf("check udp fail\n");
    } else {
        if(data_length > max_size) {
            printf("warn:[{%d}overflow maxsize{%d}]\n", data_length, max_size);
            ret = BUFF_OVER_FLOW;
            return ret;
        } else {
            memcpy(data_buf, buf + 42 ,data_length);
        }
    }
    return ret;
}
