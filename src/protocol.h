#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>

#define PACKET_MAGIC 0xC0C0AF00

typedef struct __attribute__((packed))
{
    uint64_t checksum;
    uint64_t magic;
    uint64_t id;
    uint64_t type;
    uint64_t data_size;
    uint8_t* data;
} Packet_t;

#include <utils.h>

Packet_t* generate_packet(uint64_t type, const uint8_t* data, uint64_t data_size);
int handle_packet(const Packet_t* packet);
int is_packet_valid(const Packet_t* packet);
uint64_t get_packet_checksum(const Packet_t* packet);
size_t get_packet_size_without_data();
void print_packet(const Packet_t* packet);

#endif
