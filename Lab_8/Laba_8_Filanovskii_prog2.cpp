#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <mqueue.h>
#include <cstring>

using namespace std;

bool flag;
pthread_t id1;
mqd_t mqd;
mq_attr mqd_atr;

void *Foo(void *param)
{
    char buf[128];
    while (flag)
    {
        buf[0] = '\0';

        long res = mq_receive(mqd, buf, mqd_atr.mq_msgsize+1, nullptr);

        if ( res > 0 )
        {
            cout << "Prog2: nmq_receive:result=" << res << " " << buf << endl;
        }
        else if ( res == -1 )
            perror("mq_receive");

        sleep(1);
    }
    pthread_exit(nullptr);
}

int main()
{
    flag = true;

    cout << "Prog2\n" << flush;
    if ((mqd = mq_open("/myqueue", O_CREAT | O_RDONLY | O_NONBLOCK, 0644, NULL)) == -1 )
        perror("mq_open");

    mq_getattr(mqd, &mqd_atr);

    pthread_create(&id1, nullptr, Foo, nullptr);

    cin.get();

    flag = false;

    pthread_join(id1, nullptr);

    mq_close(mqd);
    mq_unlink("/myqueue");

    cout << "\nProg2 stopped\n";

    return 0;
}
