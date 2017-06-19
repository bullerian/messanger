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


#define SRV_BUFF_LEN    255
#define SRV_PORT        1313
#define SRV_MAXCON      3
#define UDP_BUFF_SIZE   513

struct ifaddrs* get_locIP_list(void);
int server(void);


// GLOBAL VARS
struct sockaddr_in InterfaceIP;
int SrvUDPfd;
char buffer[SRV_BUFF_LEN];
enum type_e {CONN_REQ, PRESENSE_REQ, STATUS, PEER_TABLE, MESSAGE};
struct message_s {
    enum type_e type;
    size_t len;

};

//////UDP////
#define UDP_IP_STR  "127.0.0.1"
#define UDP_PORT    1313
int UDP_fd;

struct locaIf{
    char * name;
    in_addr_t ip;
    in_addr_t bcast;
} Broadcast;

//////////////


int main()
{


    // TODO: add resolving of mashine ip and broadcast address


    // UDP socket setup
    struct sockaddr_in thisUDPsock, peerUDPaddr;
    memset(&thisUDPsock, 0, sizeof(thisUDPsock));
    int broadcast=1;


    // fill socket structure
    thisUDPsock.sin_family=AF_INET;
    // convert string to int IP
    if (!inet_pton(AF_INET, UDP_IP_STR, (void *) &thisUDPsock.sin_addr)){
        perror("Can't convert string to network address");
    }
    thisUDPsock.sin_port=htons(UDP_PORT);

    if ((UDP_fd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Socket not created");
    }

    // set socket options to enable broadcast
    if((setsockopt(UDP_fd,SOL_SOCKET,SO_BROADCAST,
                             &broadcast,sizeof(broadcast))) == -1)
     {
             perror("setsockopt, BROADCAST");
             exit(1);
     }

    if(bind(UDP_fd, (struct sockaddr *) &thisUDPsock, sizeof(thisUDPsock)) < 0){
        perror("Socket not binded");
    }

    // END UDP socket setup

    //get_locIP_list();
    while (1) {
        server();
    }
    return 0;
}

int server(void){
    struct sockaddr_in clientUDP;
    socklen_t recvlen=0;
    memset(&buffer, 0, SRV_BUFF_LEN);



    // wait for message
    if(recvfrom(SrvUDPfd, &buffer, SRV_BUFF_LEN, 0,\
                (struct sockaddr *) &clientUDP, &recvlen) < 0){
        perror("Message recieve error");
    }

    printf("%s", &buffer);
    return 0;
}

void client(){
    char buff[UDP_BUFF_SIZE];
    const struct sockaddr_in peerAddr;
    ssize_t sentSize=0;

    memset($buff, '\0', UDP_BUFF_SIZE);
    memset($peerAddr, '\0', sizeof(peerAddr));

    peerAddr.sin_family=AF_INET;
    peerAddr.sin_port=htons(UDP_PORT);
    peerAddr.sin_addr.s_addr=Broadcast.bcast;

    sentSize=sendto(UDP_fd, &buff, UDP_BUFF_SIZE, 0, (struct sockaddr *) &peerAddr,\
                    sizeof(peerAddr));
    if(sentSize != sizeof(peerAddr)){
        perror("Udp datagram not transmitted!");
    }
}

struct ifaddrs* getIP4iflist(void){
    struct ifaddrs *ifList;

    // get struct list of local interfaces
    getifaddrs(&ifList);
    struct ifaddrs* interface = ifList;

    while (interface)
    {
        if (interface->ifa_addr && interface->ifa_addr->sa_family == AF_INET && )
        {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)interface->ifa_addr;
            printf("%s: %s\n", interface->ifa_name, inet_ntoa(pAddr->sin_addr));
        }

        interface = interface->ifa_next;
    }

    freeifaddrs(ifList);
    return ifList;
}



