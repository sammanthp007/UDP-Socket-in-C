#include "helper.h"

int main(int argc, char *argv[])
{
    int serv_socket;                    /* define the socket */
    short int port;                     /* port number */
    struct sockaddr_in server_address;  /* server address structure */
    struct sockaddr_in client_address;  /* client address */
    char request_buf[MAX_LINE];         /* char buffer to get request */
    int len_addr;       /* length of srv and client addr */
    int n;                              /* send and receive transmissions */

    if (argc < 2)
    {
        print_error_and_exit("No UDP port");
    }

    /* Get the port number */
    port = atoi(argv[1]);

    /* Create a server socket */
    if ((serv_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        print_error_and_exit("Can't open socket");
    }

    /* give size of the sockaddr_in, since repeated uses */
    len_addr = sizeof(server_address);

    /* Set all bytes of the server address to 0 */
    memset(&server_address, 0, len_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    /* Bind the server to the initialized socket */
    if (bind(serv_socket, (struct sockaddr *) &server_address, len_addr) < 0)
    {
        print_error_and_exit("Binding");
    }

    // So server can accept multiple user requests
    while (1)
    {
        n = recvfrom(serv_socket, request_buf, MAX_LINE, 0, (struct sockaddr *)&client_address, &len_addr);
        if (n < 0)
        {
            print_error_and_exit("While receiving");
        }

        // upon receiving

        // Because noise can get in during transmission
        request_buf[n] = '\0';

        /* Capitalize and send message using UDP */
        if (strncmp(request_buf, "CAP", 3) == 0)
        {
            // get the content
            char content[n - 4];
            memcpy (content, &request_buf[4], n - 5);
            content[n - 5] = '\0';

            // capitalize the content
            Cap(content);

            // create a message to send
            char msg[strlen(content)];
            strcpy(msg, content);
            strcat(msg, "\n");

            // send the response
            if ((n = sendto(serv_socket, msg, strlen(msg), 0, (struct sockaddr *)&client_address, len_addr)) < 0)
            {
                print_error_and_exit("while sending");
            }
        }

        /* Send the content of a file */
        if (strncmp(request_buf, "FILE", 4) == 0){
            // get the content
            char content[n - 5];
            memcpy (content, &request_buf[5], n - 6);
            content[n - 6] = '\0';

            // look if there is a file called content in the directory
            FILE * fpointer;
            if ((fpointer = fopen(content, "r")) == NULL)
            {
                char return_val[] = "9\nNOT FOUND";
                // send the response
                if ((n = sendto(serv_socket, return_val, strlen(return_val), 0, (struct sockaddr *)&client_address, len_addr)) < 0)
                {
                    print_error_and_exit("while sending");
                }
            }
            else
            {
                // get the content of the file
                char *filecontent = ReadFile(content);
                if (filecontent)
                {
                    // create and modify msg
                    char msg[strlen(filecontent)];
                    filecontent[strlen(filecontent) - 1] = '\0';
                    int char_in_file = strlen(filecontent);
                    sprintf(msg, "%d", char_in_file);
                    strcat(msg, "\n");
                    strcat(msg, filecontent);

                    // send message to client
                if ((n = sendto(serv_socket, msg, strlen(msg), 0, (struct sockaddr *)&client_address, len_addr)) < 0)
                {
                    print_error_and_exit("while sending");
                }
                    free(filecontent);
                }
            }
        }



    }

    return 0;
}
