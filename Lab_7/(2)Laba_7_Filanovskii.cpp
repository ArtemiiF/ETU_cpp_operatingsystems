#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

using namespace std;

bool flag;
pthread_t id1;
pthread_t id2;

unsigned int *ptr;

void *Foo(void *param)
{
    void * buf;

    while (flag)
    {
        cout << endl << "Prog1:" << getgid();
        buf = (void*)getgid();

        if ( write(*(int*)param, buf, sizeof(int) )==-1)
            perror("writeFifo");

        signal(SIGPIPE,sig_handler);
        sleep(1);
    }
    pthread_exit(nullptr);
}

void *FooOpen(void *param)
{
    while (flag)
    {
        //int *fdTemp = (int *) param;

        cout << endl << "Prog1:" << getgid();

        if ((*(int *) param=*(int *)open("/tmp/my_named_pipe",O_WRONLY|O_NONBLOCK))==-1 )
        {
            perror("openfifo");
            sleep(1);
        }
        else
        {
            pthread_create(&id2, nullptr,Foo,param);
            pthread_exit(nullptr);
        }
    }
}



int main()
{
    flag = true;

    //Fifo
    if(mkfifo("/tmp/my_named_pipe",0644)==-1)
        perror("mkfifo");

    int fd;
    pthread_create(&id1, nullptr, FooOpen, (void *)fd);


    cout << "Prog1\n" << flush;

    cin.get();

    flag = false;

    pthread_join(id1, nullptr);
    pthread_join(id2, nullptr);

    close(fd);
    unlink("/tmp/my_named_pipe");

    cout << "\nProg1 stopped\n";


    return 0;
}
