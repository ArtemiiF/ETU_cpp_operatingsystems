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
int fd;

void sig_handler(int signo)
{
    cout << "get SIGPIPE\n";
}

void *Foo(void *param)
{
    cout << endl << "Foo" << endl;
    unsigned int buf;

    while (flag)
    {
        buf =  getgid();
        cout << endl << "Prog1:" << getgid();

        if ( write(fd, &buf, sizeof (unsigned int)) == -1 )
            perror("writeFifo");

        sleep(1);
    }
    pthread_exit(nullptr);
}

void *FooOpen(void *param)
{
    cout << endl << "FooOpen" << endl;

    while (flag)
    {
        fd = open("/tmp/my_named_pipe", O_CREAT | O_WRONLY | O_NONBLOCK);

        if ( fd == -1 )
        {
            perror("openfifo");
            sleep(1);
        }
        else
        {
            pthread_create(&id2, nullptr, Foo, nullptr);
            pthread_exit(nullptr);
        }
    }
}


int main()
{
    signal(SIGPIPE, sig_handler);
    
    flag = true;

    //Fifo
    if ( mkfifo("/tmp/my_named_pipe", 0644) == -1 )
        perror("mkfifo");

    pthread_create(&id1, nullptr, FooOpen, nullptr);

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
