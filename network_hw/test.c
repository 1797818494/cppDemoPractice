#include "send_and_receive_api.h"
typedef struct host{
    IP ip;
    MAC mac;
} host;
int main() {
    host host_a = {1, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}};
    host host_b = {2, {0x02, 0x03, 0x04, 0x05, 0x06, 0x07}};
    char buf[] = "hello world!";
    socket src_sock = create_socket(host_a.ip, 10000);
    socket dest_sock = create_socket(host_b.ip, 10001);
    char* send_data = send(buf, strlen(buf), &dest_sock, &src_sock, host_a.mac, host_b.mac);
    assert(send_data != NULL);
    // ======================after the transfer======================

    // ===========================================================
    char rec_buf[1024] = {0};
    int ret = receive(send_data, &dest_sock, host_b.mac, rec_buf, 1024);
    if(BORING_FAIL(ret)) {
        printf("error happen errno{%d}\n", ret);
        return 0;
    }
    //assert(strlen(buf) == strlen(rec_buf));
    printf("\nsender:%s receive:%s\n", buf, rec_buf);
}