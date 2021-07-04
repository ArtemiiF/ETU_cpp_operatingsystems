#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

bool flag;
pthread_t id1;
ofstream file;
sem_t *sem;
char *semName = "/awsem";

void* Foo(void *param)
{
    while(flag)
    {
        sem_wait(sem);

        for ( int i = 0; i < 5; i++ )
        {
            
            cout<<1<<flush;
            file<<'1'<<flush;
            sleep(1);
        }
        sem_post(sem);
        sleep(1);
    }
    pthread_exit((void*)1);
}

int main()
{
    flag = true;

    sem = sem_open(semName,O_CREAT,0644,1);
    if(sem==SEM_FAILED)
        perror("Semaphore:");


    file.open("Laba_5_Filanovskii_text.txt",ios::in|ios::out|ios::app);

    pthread_create(&id1, nullptr,Foo, nullptr);
    cout<<"Prog1\n"<<flush;

    cin.get();

    flag=false;

    pthread_join(id1, nullptr);

    file.close();
    sem_close(sem);
    sem_unlink(semName);

    cout<<"\nProg1 stopped\n";
    return 0;
}
