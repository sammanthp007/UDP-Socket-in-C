#include "helper.h"

int main(int argc, char *argv[])
{
    int client_soc, len_client_soc, n;
    struct sockaddr_in server_addr, client_addr;
    struct hostent *hp;
    char buffer[MAX_LINE];

    if (argc != 3)
    {
        print_error_and_exit("Need more argument");
    }

    if ((client_soc = socket(AF_INET, SOCK_DGRAM, 0) < 0))
    {
        print_error_and_exit("Cannot create Socket");
    }




}

