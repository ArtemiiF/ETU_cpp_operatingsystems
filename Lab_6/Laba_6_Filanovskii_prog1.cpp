#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

bool flag;
pthread_t id1;
sem_t *semW;
sem_t *semR;
char *semNameW = "/awsemW";
char *semNameR = "/awsemR";
const void *shmaddres;      //Memory addres

unsigned int *ptr;

void *Foo(void *param)
{
    while (flag)
    {
        cout << endl << "Prog1:" << getgid();

        unsigned int *temp;
        temp = ptr;
        *temp = (unsigned int)getgid();


        sem_post(semW);
        sem_wait(semR);

        sleep(1);
    }
    pthread_exit((void *) 1);
}

int main()
{
    flag = true;

    //Semaphore
    semW = sem_open(semNameW, O_CREAT, 0644, 1);
    if ( semW == SEM_FAILED)
        perror("Semaphore:");

    semR = sem_open(semNameR, O_CREAT, 0644, 1);
    if ( semR == SEM_FAILED)
        perror("Semaphore:");

    //Shared memory
    key_t memKey = ftok("/tmp", 14);
    if ( memKey == -1 )
        perror("Memkey");

    cout<<"Memkey:"<<memKey<<endl;

    int shmid;
    if ((shmid = shmget(memKey, sizeof(int), 0644 | IPC_CREAT )) < 0 )
        perror("Shared memory get");

    if ((ptr = (unsigned int *) shmat(shmid, shmaddres, 0)) == (unsigned int *) -1 )
        perror("Shmat");


    pthread_create(&id1, nullptr, Foo, nullptr);

    cout << "Prog1\n" << flush;

    cin.get();

    flag = false;

    pthread_join(id1, nullptr);

    shmdt(shmaddres);
    shmctl(memKey, IPC_RMID, nullptr);

    sem_close(semW);
    sem_close(semR);
    sem_unlink(semNameW);
    sem_unlink(semNameR);

    cout << "\nProg1 stopped\n";
    return 0;
}
