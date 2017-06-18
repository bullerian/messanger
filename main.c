#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SRV_ADDR_STR    "127.0.0.1"
#define SRV_BUFF_LEN    255
#define SRV_PORT        1313
#define SRV_MAXCON      3

struct ifaddrs* get_locIP_list(void);
int server(void);


// GLOBAL VARS
struct sockaddr_in InterfaceIP;
int SrvUDPfd;
char buffer[SRV_BUFF_LEN];
/////


int main()
{
    //get_locIP_list();
    while (1) {
        server();
    }
    return 0;
}

int server(void){
    struct sockaddr_in serverUdp, clientUDP;
    socklen_t recvlen=0;
    memset(&serverUdp, 0, sizeof(serverUdp));
    memset(&buffer, 0, SRV_BUFF_LEN);

    // setting socket address parameters
    serverUdp.sin_family=AF_INET;
    // convert string
    if (!inet_pton(AF_INET, SRV_ADDR_STR, (void *) &serverUdp.sin_addr)){
        perror("Can't convert string to network address");
    }
    serverUdp.sin_port=htons(SRV_PORT);

    // create socket
    if ((SrvUDPfd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Socket not created");
    }

    // bind socket to servers interface IP
    if(bind(SrvUDPfd, (struct sockaddr *) &serverUdp, sizeof(serverUdp)) < 0){
        perror("Socket not binded");
    }

    // wait for message
    if(recvfrom(SrvUDPfd, &buffer, SRV_BUFF_LEN, 0,\
                (struct sockaddr *) &clientUDP, &recvlen) < 0){
        perror("Message recieve error");
    }

    printf("%s", &buffer);
    return 0;
}

struct ifaddrs* get_locIP_list(void){
    struct ifaddrs *addrs;

    // get struct list of local interfaces IPs
    getifaddrs(&addrs);
//    struct ifaddrs* interface = addrs;

//    while (interface)
//    {
//        if (interface->ifa_addr && interface->ifa_addr->sa_family == AF_INET)
//        {
//            struct sockaddr_in *pAddr = (struct sockaddr_in *)interface->ifa_addr;
//            printf("%s: %s\n", interface->ifa_name, inet_ntoa(pAddr->sin_addr));
//        }

//        interface = interface->ifa_next;
//    }

    //freeifaddrs(addrs);
    return addrs;
}
