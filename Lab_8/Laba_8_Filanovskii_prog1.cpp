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
    while (flag)
    {
        string str = to_string(getgid());
        char buf[str.length()];
        strcpy(buf, str.c_str());

        if ( mq_send(mqd, buf, sizeof(buf), 0) == -1 )
        {
            perror("mq_send");
        }

        cout << "Prog1:" << buf << " " << sizeof(buf) << endl;

        sleep(1);
    }
    pthread_exit(nullptr);
}

int main()
{
    flag = true;

    pthread_create(&id1, nullptr, Foo, nullptr);

    cout << "Prog1\n" << flush;

    mq_getattr(mqd, &mqd_atr);
    mqd_atr.mq_maxmsg = 20;
    mqd_atr.mq_msgsize = 8192;

    if ((mqd = mq_open("/myqueue", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &mqd_atr)) == -1 )
        perror("mq_open");

    cin.get();

    flag = false;

    mq_close(mqd);
    mq_unlink("/myqueue");

    pthread_join(id1, nullptr);

    cout << "\nProg1 stopped\n";

    return 0;
}
