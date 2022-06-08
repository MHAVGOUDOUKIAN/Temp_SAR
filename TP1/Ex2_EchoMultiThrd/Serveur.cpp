#include <iostream>
#include <set>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <thread>
#include <assert.h>
#include <vector>
#include <csignal>
#include <string>

int nb_msg=0;

void log(std::string msg) {
    std::cout << msg << std::endl;
}

void signalHandler(int unused)
{
    // I define this function to prevent the server to terminate if a client disconnect
}

int chat(int socket, sockaddr_in inf, std::string name) {
    bool stay=true;
    int n;
    while(stay) {
        char buffer[256];
        bzero(buffer,255);
        n = read(socket, buffer, sizeof(buffer)); // -1 to avoid EOF
        if(n<0) { std::cout << "> [ERROR] Cannot receive messages" << std::endl;
            stay=false;
        }
        else { std::cout << "> Message received from " << name << "("<< inet_ntoa(inf.sin_addr) <<")"<< ": " << buffer << std::endl;}
        n = write(socket, buffer, sizeof(buffer));
        if(n<0) { std::cout << "> [ERROR] Cannot send messages to "<< inet_ntoa(inf.sin_addr) << std::endl;
            stay=false;
        }
        else { std::cout << "> Message sent to " << name << "("<< inet_ntoa(inf.sin_addr) <<")" << std::endl;}
    }
    close(socket);
    return 1;
}

int main(int argc, char *argv[]) {
    signal(SIGPIPE, signalHandler); 
    if(argc<2) { log("> [ERROR] not enough arguments given"); return -1; }

    int server_sock;
    sockaddr_in server,nClient;
    int nClient_sock;
    socklen_t nClientLenght = sizeof(nClient);
    char buffer[256];

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

    std::vector<std::thread*> l_t;

    while(1) {
        std::cout << "> Waiting for connection" << std::endl;
        nClient_sock = accept(server_sock, (struct sockaddr *) &nClient, &nClientLenght);
        if( nClient_sock< 0) {
            std::cout << "> [ERROR] Error on accept" << std::endl;
            return -1; 
        }
        std::cout << "> Connexion established" << std::endl;
        
        std::string name= "client";
        name+=std::to_string(nb_msg);
        l_t.push_back(new std::thread(chat, nClient_sock, nClient, name));
        nb_msg++;
    }

    for(int i=0; i < l_t.size(); i++) {
        l_t[i]->join();
    }

    log("salut");

    return 0;
}