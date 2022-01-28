#include "pipe_networking.h"

int server_setup() {
    struct addrinfo * hints, * results;
    hints = calloc(1,sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    hints->ai_flags = AI_PASSIVE; //only needed on server
    int err = getaddrinfo(NULL, "9845", hints, &results);  //Server sets node to NULL
    if(err == -1) {
        printf("Server: unable to get IP info");
        return -1;
    }

    //create socket
    int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (sd==-1) {
        printf("Server: unable to create socket");
        return -1;
    }

    err = bind(sd, results->ai_addr, results->ai_addrlen);
    if(err == -1) {
        printf("Server: unable to get bind socket");
        return -1;
    }

    printf("listening on socket\n");
    err = listen(sd, 10);
    if(err == -1) {
        printf("Server: unable to listen");
        return -1;
    }

    free(hints);
    freeaddrinfo(results);

    return sd;
}

int server_connect(int sd) {
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    client_socket = accept(sd,(struct sockaddr *)&client_address, &sock_size);
    if(client_socket == -1) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    return client_socket;
}

int client_connect() {
    struct addrinfo * hints, * results;
    hints = calloc(1,sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    hints->ai_flags = AI_PASSIVE; //only needed on server
    int err = getaddrinfo("127.0.0.1", "9845", hints, &results);  //Server sets node to NULL
    if(err == -1) {
        printf("Client: unable to get IP info");
        return -1;
    }

    //create socket
    int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if(sd == -1) {
        printf("Client: unable to create socket");
        return -1;
    }

    err = connect(sd, results->ai_addr, results->ai_addrlen);;
    if(err == -1) {
        printf("Client: unable to connect to server");
        return -1;
    }
    //DO STUFF

    free(hints);
    freeaddrinfo(results);

    return sd;
}
