#include "helper.h"

int main(int argc, char *argv[])
{
    int serv_socket, tcp_soc;                    /* define sockets */
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

        /* Remove noise */
        request_buf[n] = '\0';

        /* Capitalize and send message using UDP */
        if (strncmp(request_buf, "CAP", 3) == 0)
        {
            /* get the content */
            char content[n - 4];
            memcpy (content, &request_buf[4], n - 5);
            content[n - 5] = '\0';

            /* capitalize the content */
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

        /* Request for a file */
        if (strncmp(request_buf, "FILE", 4) == 0){

            /* get file_name */
            char file_name[n - 5];
            int size_filename = n - 6 - 5;
            memcpy (file_name, &request_buf[5], size_filename);
            file_name[size_filename] = '\0';

            /* get 4 byte TCP port */
            char TCP_ip[4];
            memcpy (TCP_ip, &request_buf[n - 5], 4);
            char *nendptr;
            short int tcp_port = strtol(TCP_ip, &nendptr, 0);

            FILE * fpointer;
            /* file not found */
            if ((fpointer = fopen(file_name, "rb")) == NULL)
            {
                char return_val[] = "NOT FOUND\n";
                /* send response */
                if ((n = sendto(serv_socket, return_val, strlen(return_val), 0, (struct sockaddr *)&client_address, len_addr)) < 0)
                {
                    print_error_and_exit("while sending");
                }
            }
            /* file found */
            else
            {
                printf("File %s is found.\n", file_name);

                /* get size of the file */
                fseek(fpointer, 0, SEEK_END);
                int file_sz = ftell(fpointer);

                /* put fpointer to first */
                rewind(fpointer);

                /* Create the OK code, max is OK\n64000\n\0 */
                char ok_code[10];
                sprintf(ok_code, "OK\n%d\n\0", file_sz);

                /* send the OK code to client */
                if ((n = sendto(serv_socket, ok_code, strlen(ok_code), 0, (struct sockaddr *)&client_address, len_addr)) < 0)
                {
                    print_error_and_exit("while sending ok");
                }

                /* create a TCP socket */
                if ((tcp_soc = socket(AF_INET,SOCK_STREAM,0))<0) {
                    print_error_and_exit("create tcp socket");
                }

                /* set the remote servers port */
                client_address.sin_port = htons(tcp_port);

                int read_so_far = 0;
                int total = file_sz;
                printf("TOTAL bytes>%d\n", total);
                int read_now = MAX_LINE;


                /* repetedly perform this */
                sleep(1);


                /* connect to remote server */
                if (connect(tcp_soc, (struct sockaddr *)&client_address, sizeof(client_address)) < 0) {
                    print_error_and_exit("Connection refused");
                }
                int num_read = 1;
                int num_sent = 1;
                int curr_sent = 1;
                char buffer [1000];
                FILE * fptr = fopen(file_name, "rb");
                while(num_sent > 0 && total > curr_sent)
                    {
                        if((num_read = fread(buffer, 1, MAX_LINE, fptr)) < 0)
                        {
                            printf("Problem reading from file! Exiting...\n");
                            exit(0);
                        }
                        if((num_sent = write(tcp_soc, buffer, num_read)) < 0)
                        {
                            printf("Error writing to client.Exiting...\n");
                            printf("Errno: %d", errno);
                            exit(0);
                        }
                        printf("Sent: %d bytes\n", num_sent);
                        memset(buffer, 0, sizeof(buffer));  
                        curr_sent += num_sent;
                        printf("Sent so far: %d bytes\n", curr_sent);
                    }
                /*while (total > read_so_far)
                {
                    /* get the content of the file 
                    if (read_now + read_so_far >= total) {
                        read_now = total - read_so_far;
                    }

                    /*char *filecontent = ReadFile(file_name, read_so_far, read_now);

                    int read_size;
        // read_size = fread(buffer, sizeof(char), length, handler);


                    int cont_size = strlen(filecontent) + 1;
                    char msg[cont_size];
                    if (filecontent)
                    {
                        /* create and modify msg 
                        filecontent[strlen(filecontent)] = '\0';
                        strcpy(msg, filecontent);
                    }
                    else {
                        printf("empty file here");
                        strcpy(msg, "");
                    }


                    printf("READ_SO_FAR: %d\n", read_so_far);

                    /* send a message 
                    int data_len;
                    int siz = strlen(msg);
                    printf("SIZE OF EACH MESSAGE: %d\n", siz);
                    data_len = write(tcp_soc, filecontent, read_now);
                    // data_len = write(tcp_soc, msg, strlen(msg));

                    printf("DATA ACTUALLY SENT: %d\n", data_len);*/


                    /* free the file pointer 
                    free(filecontent);
                    read_so_far += read_now;
                }*/
                printf("%s sent.\n", file_name);

                /* close tcp connection */
                if (close(tcp_soc) < 0) {
                    print_error_and_exit("while closing connection");
                }
            }
        }
    }

    return 0;
}
