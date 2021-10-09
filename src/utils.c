#include <utils.h>

int send_packet(const socket_info_t* socket_info, const void* packet, size_t packet_size)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(socket_info->address);
    server_addr.sin_port = htons(socket_info->port);
    assert(server_addr.sin_addr.s_addr != INADDR_NONE);

    int ret = connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    assert(ret == 0);

    ssize_t total_bytes_sent = 0;
    while(total_bytes_sent < packet_size)
    {
        ssize_t bytes_sent = send(fd, packet, packet_size, 0);
        assert(bytes_sent >= 0);
        total_bytes_sent += bytes_sent;
    }

    assert(total_bytes_sent == packet_size);

    printf("SENT %lu bytes\n", total_bytes_sent);

    close(fd);

    return 0;
}

Packet_t* wait_for_packet(const socket_info_t* socket_info)
{
    printf("waiting for packet...\n");

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(socket_info->address);
    server_addr.sin_port = htons(socket_info->port);
    assert(server_addr.sin_addr.s_addr != INADDR_NONE);

    int ret = bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    assert(ret >= 0);

    ret = listen(fd, 0);
    assert(ret >= 0);

    struct sockaddr_in client = {0};
    socklen_t sockaddr_len = sizeof(struct sockaddr_in);
    int connection = accept(fd, (struct sockaddr*)&client, &sockaddr_len);
    assert(connection >= 0);

    Packet_t* packet = malloc(get_packet_size_without_data());
    ssize_t total_bytes_read = read(connection, packet, get_packet_size_without_data());
    assert(total_bytes_read >= 0);

    packet = realloc(packet, get_packet_size_without_data() + packet->data_size);
    assert(packet != NULL);

    ssize_t data_bytes_read = 0;
    ssize_t current_bytes_read = 0;
    while((current_bytes_read = recv(connection, &packet->data + data_bytes_read, packet->data_size - data_bytes_read, MSG_WAITALL)) > 0)
    {
        printf("READ: %zd bytes into %p (%lu)\n", current_bytes_read, &packet->data + data_bytes_read, data_bytes_read);
        data_bytes_read += current_bytes_read;
    }

    total_bytes_read += data_bytes_read;

    //mayb doesnt work because of some memory alignment (thats why we have to read all in 1 with MSG_WAITALL)
    /*
    if(current_bytes_read == -1)
    {
       perror("Error");
       printf("TRIED TO READ: %zd bytes into %p (%lu)\n", current_bytes_read, &packet->data + data_bytes_read - current_bytes_read, data_bytes_read);
    }
    */

    assert(current_bytes_read != -1);
    assert(total_bytes_read == get_packet_size_without_data() + packet->data_size);

    close(connection);
    close(fd);

    return packet;
}

uint8_t* generate_random_data(size_t data_size)
{
    uint8_t* data = malloc(data_size);

    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, data, data_size);
    close(fd);

    return data;
}

void show_raw(const uint8_t* data, size_t data_size)
{
    for(size_t i = 0; i < data_size; ++i)
        printf("%x", data[i]);

    putchar('\n');
}
