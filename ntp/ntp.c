#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>

const char *ntp_array[] = {
    "cn.ntp.org.cn",
    "time5.cloud.tencent.com",
    "time4.cloud.tencent.com",
    "time3.cloud.tencent.com",
    "time2.cloud.tencent.com",
    "time1.cloud.tencent.com",
};

#define NTP_TIMESTAMP_DELTA 2208988800ull

#define LI(packet)                                                             \
    (uint8_t)((packet.li_vn_mode & 0xC0) >> 6) // (li   & 11 000 000) >> 6
#define VN(packet)                                                             \
    (uint8_t)((packet.li_vn_mode & 0x38) >> 3) // (vn   & 00 111 000) >> 3
#define MODE(packet)                                                           \
    (uint8_t)((packet.li_vn_mode & 0x07) >> 0) // (mode & 00 000 111) >> 0

// Structure that defines the 48 byte NTP packet protocol.
typedef struct {
    uint8_t li_vn_mode; // Eight bits. li, vn, and mode.
                        // li.   Two bits.   Leap indicator.
                        // vn.   Three bits. Version number of the protocol.
                        // mode. Three bits. Client will pick mode 3 for client.
    uint8_t stratum; // Eight bits. Stratum level of the local clock.
    uint8_t poll; // Eight bits. Maximum interval between successive messages.
    uint8_t precision; // Eight bits. Precision of the local clock.
    uint32_t rootDelay; // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock
                             // source.
    uint32_t refId; // 32 bits. Reference clock identifier.
    uint32_t refTm_s; // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f; // 32 bits. Reference time-stamp fraction of a second.
    uint32_t origTm_s; // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f; // 32 bits. Originate time-stamp fraction of a second.
    uint32_t rxTm_s; // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f; // 32 bits. Received time-stamp fraction of a second.
    uint32_t txTm_s; // 32 bits and the most important field the client
                     // cares about. Transmit time-stamp seconds.
    uint32_t txTm_f; // 32 bits. Transmit time-stamp fraction of a second.
} ntp_packet; // Total: 384 bits or 48 bytes.

int ntp_update(const char *host, int timeout)
{
    int ret = 0;
    int sockfd, n;
    int portno = 123;
    ntp_packet packet;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    fd_set set;
    struct timeval tv;

    if (!host) {
        return -1;
    }
    memset(&packet, 0, sizeof(ntp_packet));
    packet.li_vn_mode = 0x1b;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Create a UDP socket.
    if (sockfd < 0) {
        printf("ERROR opening socket!\n");
        return -1;
    }
    server = gethostbyname(host);
    if (!server) {
        printf("ERROR opening socket\n");
        return -1;
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR connecting!\n");
        goto err;
    }
    n = write(sockfd, (char *)&packet, sizeof(ntp_packet));
    if (n < 0) {
        printf("ERROR writing to socket\n");
        goto err;
    }

    FD_ZERO(&set);
    FD_SET(sockfd, &set);
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    ret = select(sockfd + 1, &set, NULL, NULL, &tv);
    if (ret == -1) {
        printf("ERROR select\n");
        goto err;
    } else if (ret == 0) {
        printf("ERROR timeout\n");
        goto err;
    } else {
        n = read(sockfd, (char *)&packet, sizeof(ntp_packet));
        if (n < 0) {
            printf("ERROR reading from socket\n");
            goto err;
        }
        packet.txTm_s = ntohl(packet.txTm_s);
        packet.txTm_f = ntohl(packet.txTm_f);
        time_t txTm = (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);
        // printf("Time: %s", ctime((const time_t *)&txTm));
        printf("Time: %s", asctime((gmtime(&txTm))));
    }
    close(sockfd);
    return 0;
err:
    close(sockfd);
    return -1;
}

int main(int argc, char *argv[])
{
    int rc, i;
    int s1, s2;
    struct timespec tp;

    for (i = 0; i < sizeof(ntp_array) / sizeof(ntp_array[0]); i++) {
        clock_gettime(CLOCK_MONOTONIC, &tp);
        s1 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
        rc = ntp_update(ntp_array[i], 1);
        clock_gettime(CLOCK_MONOTONIC, &tp);
        s2 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
        printf("Host[%s] Time cost: %d ms\n", ntp_array[i], s2 - s1);
        if (!rc) {
            printf("NTP update success!\n");
        } else {
            printf("NTP update failed!\n");
        }
    }

    return 0;
}
