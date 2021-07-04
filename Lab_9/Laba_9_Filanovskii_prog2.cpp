#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

bool flag;
pthread_t id1;
pthread_t id2;
pthread_mutex_t queueMutex;

int mySocket;
sockaddr_in mySockAddr;
sockaddr_in serverSockAddr;
socklen_t clientAddrLen;

struct bufInfo
{
    int count;
    char info[256];

    bufInfo()
    {
        count =0;
        info[0]='\0';
    }
};

//Get info from server
void *FooGet(void *param)
{
    bufInfo rcvbuf;

    while (flag)
    {
        memset(&rcvbuf.info, 0, sizeof(rcvbuf.info));

        long recvcount = recvfrom(mySocket, &rcvbuf, sizeof(rcvbuf)+1, 0, (sockaddr *) &mySockAddr, &clientAddrLen);

        if ( recvcount == -1 )
        {
            perror("recv");
            sleep(1);
        }
        else
        {
            cout << rcvbuf.count<<")Client get from server " << rcvbuf.info << endl;
        }

        
    }
    pthread_exit(nullptr);
}

//Send info to server
void *FooSend(void *param)
{
    bufInfo buf;
    int count = 1;
    while (flag)
    {
        int len = sprintf(buf.info, "Request");
        buf.count=count;

        long sendCount = sendto(mySocket, &buf, sizeof(buf)+1, 0, (sockaddr *) &serverSockAddr, clientAddrLen);
        if ( sendCount == -1 )
            perror("Send");
        else
        {
            cout << buf.count <<")Client sent to server " << buf.info << endl;
            count++;
        }

        sleep(1);
    }
    pthread_exit(nullptr);
}

int main()
{
    flag = true;

    cout << "Prog2(Client)\n" << flush;

    mySocket = socket(AF_INET, SOCK_DGRAM, 0);
    if ( mySocket == -1 )
        perror("SocketCreation");

    if ( fcntl(mySocket, F_SETFL, O_NONBLOCK) == -1 )
        perror("FCNTL");

    int optval = 1;
    setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    //Set values for sockaddr_in for client(me)
    mySockAddr.sin_family = AF_INET;
    mySockAddr.sin_port = htons(7000);
    mySockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( bind(mySocket, (sockaddr *) &mySockAddr, sizeof(mySockAddr)) == -1 )
        perror("Socket bind");

    memset(&serverSockAddr, 0, sizeof(serverSockAddr));

    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(8000);
    serverSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    clientAddrLen = sizeof(serverSockAddr);

    pthread_mutex_init(&queueMutex, NULL);

    pthread_create(&id1, nullptr, FooGet, nullptr);
    pthread_create(&id2, nullptr, FooSend, nullptr);

    cin.get();

    flag = false;

    pthread_join(id1, nullptr);
    pthread_join(id2, nullptr);
    pthread_mutex_destroy(&queueMutex);

    close(mySocket);

    cout << "\nProg2 stopped\n";

    return 0;
}
