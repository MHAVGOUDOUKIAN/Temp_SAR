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
    if(argc<3) { log("> [ERROR] not enough arguments given"); return -1; }

    int client_sock;
    hostent* m_host;
    sockaddr_in host_addr;

    //
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock<0) { std::cout << "> [ERROR] can't init socket" << std::endl;}
    //
    m_host = gethostbyname(argv[1]);
    if(m_host == nullptr) { std::cout << "> [ERROR] no such host" << std::endl; }
    //
    bzero((char*)&host_addr, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    bcopy((char*) m_host->h_addr, (char*)&host_addr.sin_addr.s_addr, m_host->h_length);
    host_addr.sin_port = htons(atoi(argv[2]));

    if(connect(client_sock, (struct sockaddr *) &host_addr, sizeof(host_addr)) < 0) { 
        std::cout << "> [ERROR] connection impossible" << std::endl;
        return -1;
    }

    std::cout << "> Connected to: " << argv[1] << std::endl;
    

    char buffer[256];
    bool stay=true;

    while(stay) {
        bzero(buffer,256);
        std::cout << "> Please, enter a message then press enter to send: ";
        fgets(buffer, 256, stdin);
        int n = write(client_sock, buffer, sizeof(buffer));
        if(n<0) { std::cout << "> [ERROR] Cannot send messages" << std::endl; }
        n = read(client_sock, buffer, sizeof(buffer));
        if(n<0) { std::cout << "> [ERROR] Cannot receive messages" << std::endl;}
        std::cout << "> '" << argv[1] << "' sent you back: " << buffer << std::endl;
    }
    close(client_sock);

    return 0;
}