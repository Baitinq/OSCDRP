#ifndef _UTILS_H_
#define _UTILS_H_

#include <protocol.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct
{
    char address[20];
    int port;
} socket_info_t;

int send_packet(const socket_info_t* socket_info, const void* packet, size_t packet_size);
Packet_t* wait_for_packet(const socket_info_t* socket_info);

uint8_t* generate_random_data(size_t data_size);
void show_raw(const uint8_t* data, size_t data_size);

#endif
