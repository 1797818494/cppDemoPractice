#include "stddef.h"
#include "memory.h"
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

#define IP unsigned int 
#define PORT unsigned short
#define BORING_SUCC(val) (val == 0)
#define BORING_FAIL(val) (val != 0)
#define ERROR int
#define UDP_TYPE 17
#define MAC_NOT_MACTH -1
#define IP_NOT_MACTH -2
#define IP_TYPE_NOT_MATCH -3
#define BUFF_OVER_FLOW -4
#define CORRUPT -5
#define PORT_NOT_MATCH -6
#define SUCCESS 0

unsigned int packet_id = 0;
typedef unsigned char MAC[6];

typedef struct udp_header {
    unsigned short dest_port;
    unsigned short src_port;
    unsigned short length;
    unsigned short check_sum;
} udp_header; // 8 bytes

typedef struct ip_header {
    unsigned char version_and_header_length; // 4 bit version and 4 bit header_length
    unsigned char tos; 
    unsigned short total_length;
    unsigned short packet_id;
    unsigned short flag_and_offset; // 3bit 其中2个比特有效，分别代表是否允许分片，以及当前包是否为分片包, 目前全为0,因为实现不包含ip分片
    unsigned char ttl; 
    unsigned char type;
    unsigned short check_sum;
    unsigned int src_ip;
    unsigned int dest_ip; 
    unsigned char option[0]; // 目前不支持可选字段
} ip_header;  // 20 bytes

typedef struct frame_header {
    unsigned char dest_mac[6];
    unsigned char src_mac[6];
    unsigned short length; // 0800 ipv4
} frame_header; // 14 bytes

typedef struct socket {
    IP ip;
    PORT port;
} socket;

socket create_socket(IP ip, PORT port) {
    return socket{ip, port};
}

unsigned int computeCRC(const unsigned char *data, size_t length) {
    unsigned int crc = 0xFFFFFFFF;
    size_t i, bit;
    for (i = 0; i < length; ++i) {
        crc ^= data[i];
        for (bit = 0; bit < 8; ++bit) {
            if (crc & 0x80000000)
                crc = (crc << 1) ^ 0x04C11DB7;
            else
                crc <<= 1;
        }
    }
    return ~crc;
}

unsigned short make_check_sum(const void* buf, size_t size, unsigned short check_sum) {
    const unsigned short* data = (const unsigned short*)buf;
    unsigned short sum = 0;
    sum -= check_sum;
    // 将数据视为16位字节序列，并计算校验和
    while (size > 1) {
        sum += *data++;
        size -= 2;
    }
    if (size > 0) {
        sum += *((const unsigned char*)data);
    }

    // 将进位加到校验和的高位
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    // 取反得到校验和
    return (unsigned short)(~sum);
}
int is_check_sum_complete(const void* buf, size_t size, unsigned short check_sum_target) {
    if(make_check_sum(buf, size, check_sum_target) == check_sum_target) {
        return SUCCESS;
    }
    return CORRUPT;
}
// =================sender=====================
frame_header parse_to_frame(MAC dest, MAC src, unsigned short length) {
    frame_header header;
    unsigned short ip_type = 0x0800;
    memcpy((void*)&header, dest, 6);
    memcpy((void*)&header + 6, src, 6);
    memcpy((void*)&header + 12, &length, 2);
    return header;
}

ip_header parse_to_ip(IP dest, IP src, size_t total_length) {
    ip_header header;
    printf("%d", (int)sizeof(header));
    memset(&header, 0, sizeof(header));
    header.dest_ip = dest;
    header.src_ip = src;
    header.type = UDP_TYPE;
    header.packet_id = packet_id++;
    header.version_and_header_length = (4 << 4) | 20;
    header.ttl = 64;
    header.total_length = total_length;
    header.check_sum = make_check_sum(&header, 20, 0);
    return header;
}
udp_header parse_to_udp(PORT dest, PORT src, unsigned short length, const void* buf) {
    udp_header header;
    memset(&header, 0, sizeof(header));
    memcpy((void*)&header, &dest, 2);
    memcpy((void*)&header + 2, &src, 2);
    memcpy((void*)&header + 4, &length, 2);
    const int buf_size = sizeof(udp_header) + length; // length is data's length
    char* temp_buf = (char*)malloc(buf_size);
    memcpy((void*)temp_buf, &header, sizeof(header));
    memcpy((void*)temp_buf + sizeof(header), buf, length);
    header.check_sum = make_check_sum(temp_buf, buf_size, 0);
    free(temp_buf);
    return header;
}   

// ==================reciver=====================
frame_header get_frame_from_bytes(const void* buf, const size_t size) {
    assert(size == 14);
    return *(frame_header*)(buf);    
}

ip_header get_ip_from_bytes(const void* buf, const size_t size) {
    assert(size == 20);
    return *(ip_header*)(buf);
}
udp_header get_udp_from_bytes(const void* buf, const size_t size) {
    assert(size == 8);
    return *(udp_header*)(buf);
}


int check_frame(const void* buf, const size_t size, MAC mac) {
    frame_header header = get_frame_from_bytes(buf, size);
    if(!memcmp(mac, header.dest_mac, 6)) {
        return MAC_NOT_MACTH;
    }
    unsigned int crc = computeCRC((const unsigned char*)buf + sizeof(frame_header), header.length);
    if(crc != *(unsigned int*)((const unsigned char*)buf + 14 + header.length)) {
        printf("\n%u\n", crc);
        return CORRUPT;
    }
    return SUCCESS;
}

int check_ip(const void* buf, const size_t size, IP ip) {
    ip_header header = get_ip_from_bytes(buf, size);
    if(header.dest_ip != ip) {
        return IP_NOT_MACTH;
    }
    if(!BORING_SUCC(is_check_sum_complete(&header, 20, header.check_sum))) {
        return CORRUPT;
    }
    assert(header.type == UDP_TYPE);
    return SUCCESS;
}

int check_udp(const void* buf, PORT port, const void* total_buf, unsigned short *data_length) {
    udp_header header = get_udp_from_bytes(buf, 8);
    if(!(header.dest_port == port)) {
        return PORT_NOT_MATCH;
    }
    if(BORING_FAIL(is_check_sum_complete(total_buf + sizeof(frame_header) + sizeof(ip_header), header.length + 8, header.check_sum))) {
        return CORRUPT;
    }
    *data_length = header.length;
    return SUCCESS;
}

