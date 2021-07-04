#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

bool flag;
pthread_t id1;
int fd;

void *Foo(void *param)
{
    cout << endl << "Foo" << endl;

    unsigned int buf;

    while (flag)
    {
        long temp = read(fd, &buf, sizeof(unsigned int));

        if ( temp == -1 )
            perror("readFifo");
        else if ( temp == 0 )
            cout << "End of file\n";
        else
            cout << "Prog2:" << buf << endl;

        sleep(1);
    }
    pthread_exit(nullptr);
}

int main()
{
    flag = true;

    //Fifo
    if ( mkfifo("/tmp/my_named_pipe", 0644) == -1 )
        perror("mkfifo");

    fd = open("/tmp/my_named_pipe", O_CREAT | O_RDONLY | O_NONBLOCK);

    if ( fd == -1 )
        perror("openfifo");
    else
        pthread_create(&id1, nullptr, Foo, nullptr);

    cout << "Prog2\n" << flush;

    cin.get();

    flag = false;

    pthread_join(id1, nullptr);

    close(fd);
    unlink("/tmp/my_named_pipe");

    cout << "\nProg2 stopped\n";

    return 0;
}
