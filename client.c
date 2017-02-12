#include "helper.h"

int main(int argc, char *argv[])
{
    int client_soc, len_client_soc, n;
    short int server_UDP_port, client_tcp_port;
    struct sockaddr_in server_addr, client_addr;
    char *endptr;              // for strtol()
    char *sAddr;               // for server IP addr
    char *sudpPort;               // for server udp port
    char *ctcpPort;             // for client tcp port
    char input_buffer[MAX_LINE];

    // input must be in the format:
    // <client> <TCP port> <server IP> <server UDP port>

    if (argc == 4)
    {
        // get client tcp port
        client_tcp_port = strtol(argv[1], &endptr, 0);
        if (*endptr) {
            print_error_and_exit("Invalid client TCPport supplied");
        }
        
        // get server UDP port
        server_UDP_port = strtol(argv[3], &endptr, 0);
        if (*endptr) {
            print_error_and_exit("Invalid server UDP port");
        }

    }
    else {
        print_error_and_exit("Invalid number of arguments.");
        print_error_and_exit("<client> <TCPport> <serverIP> <serverUDP port>");
    }

    // create UDP socket
    if ((client_soc = socket(AF_INET, SOCK_DGRAM, 0) < 0))
    {
        print_error_and_exit("Cannot create Socket");
    }

    // 

    // TODO check the three arguments to make sure they are to make sure they are what they need to be

    // The client runs until client tells it to stop
    while (1)
    {
        // Prompt user
        printf("Enter s for message, t for file transfer, q to exit:\n");
        fgets(input_buffer, MAX_LINE, stdin);

        // when user inputs 2
        if (strlen(input_buffer) == 2 && strncmp(input_buffer, "s", 1) == 0)
        {
            // prompt user for message
            printf("Enter message in form: \n");
            // get message
            fgets(input_buffer, MAX_LINE, stdin);

            int buffer_len = strlen(input_buffer);

            // Send message
            // Create a new sending buffer whose size can take in all msg
            char sending_msg[buffer_len + 5];
            // add CAP\n
            strcpy(sending_msg, "CAP\n");
            // buffer already has the trailing \n from fgets
            strcpy(sending_msg, input_buffer);

            // add the end of line character to sending msg
            sending_msg[buffer_len + 4] = '\0';

            // send using UDP

        }
    }
}

