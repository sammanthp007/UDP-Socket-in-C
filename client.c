#include "helper.h"

int main(int argc, char *argv[])
{
    int c_udp_soc, len_serv_addr, len_c_udp_soc, n;
    short int server_UDP_port, client_tcp_port;
    struct sockaddr_in server_addr, client_addr;
    char *endptr;              // for strtol()
    char *sAddr;               // for server IP addr
    char *sudpPort;               // for server udp port
    char *ctcpPort;             // for client tcp port
    char input_buffer[MAX_LINE];


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
        print_error_and_exit("<client> <TCPport> <serverIP> <serverUDP port>");
    }

    /* create UDP socket */
    if ((c_udp_soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        print_error_and_exit("Creating Socket");
    }

    // configure the server address struct to send to
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_UDP_port);

    /* Set server ip */

    if (inet_aton(argv[2], &server_addr.sin_addr) <= 0) {
        print_error_and_exit("Setting UDP IP server");
    }

    len_serv_addr = sizeof(server_addr);

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
            strcat(sending_msg, input_buffer);

            // add the end of line character to sending msg
            sending_msg[buffer_len + 4] = '\0';
            write(1,sending_msg, strlen(sending_msg));

            // send using UDP
            // buffer_len + 5 for including the last char
            if (n = sendto(c_udp_soc, sending_msg, strlen(sending_msg), 0, (struct sockaddr *)&server_addr, len_serv_addr) < 0)
            {
                print_error_and_exit("Sending");
            }

            // receive from server
            if ((n = recvfrom(c_udp_soc, sending_msg, MAX_LINE, 0, (struct sockaddr *)&server_addr, &len_serv_addr)) > 0)
            {
                sending_msg[n] = '\0';
                write(1, sending_msg, n);
            }
        }
        else if (strlen(input_buffer) == 2 && strncmp(input_buffer, "t", 1) == 0){
            printf("Enter file name: \n");
            // fgets also takes in the last \n
            fgets(input_buffer, MAX_LINE, stdin);

            // Save the name of the file for future use
            char file_name[MAX_LINE];
            strcpy(file_name, input_buffer);
            int buffer_len = strlen(input_buffer);
            file_name[buffer_len - 1] = '\0';

            /* Send message to server in the form "FILE\nxxx\n" */
            char message[MAX_LINE + 10];

            strcpy(message, "FILE\n");
            strcat(message, input_buffer);

            message[buffer_len + 5] = '\0';

            if (n = sendto(c_udp_soc, message, strlen(message), 0, (struct sockaddr *)&server_addr, len_serv_addr) < 0)
            {
                print_error_and_exit("sending");
            }

            /* receive message from server */
            if ((n = recvfrom(c_udp_soc, message, MAX_LINE, 0, (struct sockaddr *)&server_addr, &len_serv_addr)) > 0)
            {
                // because noise gets added during transmission
                message[n] = '\0';
                write(1, message, n);


                // Find the just data section of the message
                int ii = strcspn(message, "\n");
                char just_data[n];
                memcpy(just_data, &message[ii + 1], n - ii);

                //save the just the data to a file
                FILE* dat = fopen(file_name, "wb");

                fprintf(dat, just_data);
                fclose(dat);

                printf("Server has responded. The content of server response");
                printf(" has been saved in %s successfully.\n", file_name);
            }
        }
        else if (strlen(input_buffer) == 2 && strncmp(input_buffer, "q", 1) == 0)
        {
            return EXIT_SUCCESS;
        }
    }
}

