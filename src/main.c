#include <protocol.h>
#include <utils.h>

int main(int argc, char** argv)
{
    socket_info_t socket_info;
    strncpy(socket_info.address, "127.0.0.1", sizeof(socket_info.address));
    socket_info.port = 12345;

    uint64_t packet_id = 1;
    uint64_t data_size = 20000;
    uint8_t* data = generate_random_data(data_size);

    Packet_t* packet = generate_packet(packet_id, data, data_size);
    free(data);

    pthread_t wait_for_packet_thread;
    pthread_create(&wait_for_packet_thread, NULL, (void*)wait_for_packet, &socket_info);

    sleep(2); //wait until the wait_for_packet_thread is running
    send_packet(&socket_info, (void*)packet, get_packet_size_without_data() + packet->data_size);
    free(packet);

    Packet_t* return_packet = NULL;
    pthread_join(wait_for_packet_thread, (void**)&return_packet);

    handle_packet(return_packet);

    free(return_packet);

    return 0;
}
