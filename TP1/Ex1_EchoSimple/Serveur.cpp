#include <iostream>
#include <set>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <thread>
#include <assert.h>

void log(std::string msg) {
    std::cout << msg << std::endl;
}

int main(int argc, char *argv[]) {
    if(argc<2) { log("> [ERROR] not enough arguments given"); return -1; }

    int server_sock;
    sockaddr_in server,nClient;
    int nClient_sock;
    socklen_t nClientLenght = sizeof(nClient);
    char buffer[256];
    int n;

    // Configuration
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock==-1) { 
        std::cout << "> [ERROR] can't init socket" << std::endl;
        return -1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));

    if (bind(server_sock, (struct sockaddr *) &server, sizeof(server)) == -1) { 
        std::cout << "[ERROR] port already used " << std::endl; 
        return -1;    
    }

    listen(server_sock,5);

    // Application
    bool stay=true;

    while(1) {
        std::cout << "> Waiting for connection" << std::endl;
        nClient_sock = accept(server_sock, (struct sockaddr *) &nClient, &nClientLenght);
        if( nClient_sock< 0) {
            std::cout << "> [ERROR] Error on accept" << std::endl;
            return -1; 
        }
        std::cout << "> Connexion established" << std::endl;
        stay=true;

        while(stay) {
            char buffer[256];
            bzero(buffer,255);
            n = read(nClient_sock, buffer, sizeof(buffer)); // -1 to avoid EOF
            if(n<0) { std::cout << "> [ERROR] Cannot receive messages" << std::endl;}
            else { std::cout << "> Message received: " << buffer << std::endl;}
            n = write(nClient_sock, buffer, sizeof(buffer));
            if(n<0) { std::cout << "> [ERROR] Cannot send messages" << std::endl;}
            else { std::cout << "> Message sent" << std::endl;}
        }
        close(nClient_sock);
    }

    return 0;
}