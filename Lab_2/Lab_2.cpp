#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


using namespace std;

sem_t sem;

struct thread_params
{
private:
    bool flag;
    char symb;
    int mode;

public:
    thread_params()
    {
        flag = true;
        symb = 'x';
        mode = 0;
    }

    void SetSymb(char ch)
    {
        symb = ch;
    }

    void SetFlag(bool fl)
    {
        flag = fl;
    }

    void SetMode(int md)
    {
        mode = md;
    }

    char GetSymb()
    {
        return symb;
    }

    bool GetFlag()
    {
        return flag;
    }

    int GetMode()
    {
        return mode;
    }

};

//Функция потока 1
static void *func1(void *param)
{
    struct thread_params *params = (struct thread_params *) param;

    switch (params->GetMode())
    {
        //sem_wait
        case 1:
        {
            while (params->GetFlag())
            {
                sem_wait(&sem);
                for ( int i = 0; i < 4; ++i )
                {
                    cout << params->GetSymb() << flush;
                    sleep(1);
                }
                sem_post(&sem);
                sleep(1);
            }
            break;
        }

            //sem_trywait
        case 2:
        {


            while (params->GetFlag())
            {
                int tw = sem_trywait(&sem);
                if ( tw != -1 )
                {
                    for ( int i = 0; i < 3; ++i )
                    {
                        cout << params->GetSymb() << flush;
                        sleep(1);
                    }
                    sem_post(&sem);
                    sleep(2);
                }
                else
                {
                    perror("Resource 1 busy");
                    sleep(1);
                }

            }

            break;
        }

            //sem_timedwait
        case 3:
        {
            //Для счетчика
            timespec tm;

            while (params->GetFlag())
            {
                clock_gettime(CLOCK_REALTIME, &tm);
                tm.tv_sec += 1;
                int tw = sem_timedwait(&sem, &tm);

                if ( tw != -1 )
                {
                    for ( int i = 0; i < 4; ++i )
                    {
                        cout << params->GetSymb() << flush;
                        sleep(1);
                    }

                    sem_post(&sem);
                    sleep(1);
                }
            }
            break;
        }
    }

    pthread_exit((void *) 1);
}

//Функция потока 2
static void *func2(void *param)
{
    struct thread_params *params = (struct thread_params *) param;

    switch (params->GetMode())
    {
        //sem_wait
        case 1:
        {
            while (params->GetFlag())
            {
                sem_wait(&sem);
                for ( int i = 0; i < 4; ++i )
                {
                    cout << params->GetSymb() << flush;
                    sleep(1);
                }
                sem_post(&sem);
                sleep(1);
            }
            break;
        }

            //sem_trywait
        case 2:
        {


            while (params->GetFlag())
            {
                int tw = sem_trywait(&sem);
                if ( tw != -1 )
                {
                    for ( int i = 0; i < 3; ++i )
                    {
                        cout << params->GetSymb() << flush;
                        sleep(1);
                    }
                    sem_post(&sem);
                    sleep(2);
                }
                else
                {
                    perror("Resource 2 busy");
                    sleep(1);
                }

            }

            break;
        }

            //sem_timedwait
        case 3:
        {
            //Для счетчика
            timespec tm;

            while (params->GetFlag())
            {
                clock_gettime(CLOCK_REALTIME, &tm);
                tm.tv_sec += 1;

                int tw = sem_timedwait(&sem, &tm);

                if ( tw != -1 )
                {
                    for ( int i = 0; i < 4; ++i )
                    {
                        cout << params->GetSymb() << flush;
                        sleep(1);
                    }

                    sem_post(&sem);
                    sleep(1);
                }
            }
            break;
        }
    }

    pthread_exit((void *) 2);
}

int main()
{
    //Структуры с аргументами для потоков
    thread_params params1;
    thread_params params2;

    //Идентификаторы потоков
    pthread_t id1;
    pthread_t id2;

    sem_init(&sem, 0, 1);

    //Ввод символов для потоков
    cout << "Input symbol for threads\n";
    cout << "1 thread:";

    //Проверка на верный ввод
    string c1;
    do
    {
        cin >> c1;
        if ( c1.length() != 1 )
            cout << "\nInput SINGLE symbol!!!!\n1 thread:" << flush;
    } while (c1.length() != 1);

    params1.SetSymb(c1[0]);

    //Проверка на верный ввод
    cout << "2 thread:";
    string c2;
    do
    {
        cin >> c2;
        if ( c2.length() != 1 )
            cout << "Input SINGLE symbol!!!!\n2 thread:" << flush;
    } while (c2.length() != 1);

    params2.SetSymb(c2[0]);

    cout << "Choose mode.\n1- sem_wait\n2- sem_trywait \n3- sem_timedwait(1 sec)\nMode:";

    int mode;
    cin >> mode;
    params1.SetMode(mode);
    params2.SetMode(mode);

    cout << "\nThreads started\nPress ENTER for stop\n";

    //Создаем потоки
    pthread_create(&id1, nullptr, func1, &params1);
    pthread_create(&id2, nullptr, func2, &params2);

    //Очищаем буфер потока
    cin.clear();
    cin.ignore();

    cin.get();

    params1.SetFlag(false);
    params2.SetFlag(false);
    cout << "Threads stopped\n";


    //threadcode 1 и 2 - переменные для хранения кода завершения потока
    int threadcode1, threadcode2;
    pthread_join(id1, (void **) &threadcode1);
    pthread_join(id2, (void **) &threadcode2);
    sem_destroy(&sem);

    //Вывод кодов завершения
    cout << "\n1 thread code:" << threadcode1 << endl << "2 thread code:" << threadcode2 << endl;
    return 0;
}
