#include <protocol.h>

Packet_t* generate_packet(uint64_t type, const uint8_t* data, uint64_t data_size)
{
    Packet_t* packet = calloc(1, get_packet_size_without_data() + data_size);

    srand(time(NULL));

    packet->magic = PACKET_MAGIC;
    packet->id = rand();
    packet->type = type;
    packet->data_size = data_size;
    packet->data = NULL;
    memcpy(&packet->data, data, data_size);

    packet->checksum = get_packet_checksum(packet);

    printf("GENERATED PACKET:\n");
    print_packet(packet);

    return packet;
}

int handle_packet(const Packet_t* packet)
{
    int ret = is_packet_valid(packet);
    if(ret < 0)
    {
        printf("Packet is invalid!\n");
        return ret;
    }

    printf("PACKET VALIDATED!\n");
    printf("RECIEVED PACKET:\n");

    switch(packet->id)
    {
        default:
            print_packet(packet);
            break;
    }

    return 0;
}

int is_packet_valid(const Packet_t* packet)
{
    if(packet->magic != PACKET_MAGIC)
    {
        perror("Invalid packet magic!\n");
        return -EINVAL;
    }

    uint64_t checksum = get_packet_checksum(packet);
    if(packet->checksum != checksum)
    {
        printf("packet checksum: %llu\n", checksum);
        printf("expected checksum: %llu\n", packet->checksum);
        perror("Invalid packet checksum!\n");

        printf("PACKET AFTER RECIEVED:\n");
        print_packet(packet);

        return -EINVAL;
    }

    return 0;
}

uint64_t get_packet_checksum(const Packet_t* packet)
{
    //FIXME: This is not a proper checksum implementation.

    uint64_t sum = 0;
    uint64_t i = sizeof(packet->checksum); //start calculating checksum from outside checksum var in struct
    for(; i < get_packet_size_without_data() + packet->data_size; ++i)
        sum += ((uint8_t*)packet)[i];

    return sum;
}

void print_packet(const Packet_t* packet)
{
    printf("Packet:\n");
    printf("\tmagic: %llu\n", packet->magic);
    printf("\tchecksum: %llu\n", packet->checksum);
    printf("\tid: %llu\n", packet->id);
    printf("\ttype: %llu\n", packet->type);
    printf("\tdata size: %llu\n", packet->data_size);
    printf("\tdata string: %s\n\tdata raw: ", (char*)&packet->data);
    show_raw((uint8_t*)&packet->data, packet->data_size);
}

size_t get_packet_size_without_data()
{
    return sizeof(Packet_t) - sizeof(uint8_t*);
}
