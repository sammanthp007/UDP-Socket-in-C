#include "helper.h"

int main(int argc, char *argv[])
{
    int serv_socket;    // define the socket
    short int port;         // port number
    struct sockaddr_in server_address;   // server address structure
    struct sockaddr_in client_address;   // client address
    char request_buf[MAX_LINE];     // char buffer to get request
    int length_of_addr;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no UDP port provided");
        exit(0);
    }

    // Get the port number
    port = atoi(argv[1]);

    // Create a server socket
    if ((serv_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        print_error_and_exit("Can't open socket");
    }

    length_of_addr = sizeof(server_address);

    // Set all bytes of the server address to 0
    memset(&server_address, 0, length_of_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);





    return 0;
}
