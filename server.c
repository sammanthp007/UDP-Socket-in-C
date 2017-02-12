#include "helper.h"

int main(int argc, char *argv[])
{
    int serv_socket, client_socket;    // define the socket
    short int port;         // port number
    struct sockaddr_in server_address;   // server address structure
    struct sockaddr_in client_address;   // client address
    char request_buf[MAX_LINE];     // char buffer to get request
    int len_addr, len_client_adr;    // length of srv and client addr

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

    len_addr = sizeof(server_address);

    // Set all bytes of the server address to 0
    memset(&server_address, 0, len_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    // Bind the server to the initialized socket
    if (bind(serv_socket, (struct sockaddr *) &server_address, len_addr) < 0)
    {
        print_error_and_exit("Can't bind");
    }

    // Since server uses UDP, simply receive a request and respond
    len_client_adr = sizeof(struct sockaddr_in);

    // So server can accept multiple user requests
    while (1)
    {
        if ((client_socket = recvfrom(serv_socket, request_buf, MAX_LINE, 0, (struct sockaddr *)&client_address, &len_client_adr)) < 0)
        {
            print_error_and_exit("While receiving");
        }

        // upon receiving
        printf("I received");

        // send the response
        if ((client_socket = sendto(serv_socket, "Got it", 6, 0, (struct sockaddr *)&client_address, len_client_adr)) < 0)
        {
            print_error_and_exit("while sending");
        }
    }

    return 0;
}
