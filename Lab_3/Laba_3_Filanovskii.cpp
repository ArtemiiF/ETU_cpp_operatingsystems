#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "fcntl.h"
#include <cstring>

using namespace std;

struct thread_params
{
private:

    bool flag;

public:
    thread_params()
    {
        flag = true;

    }

    void SetFlag(bool fl)
    {
        flag = fl;
    }

    bool GetFlag()
    {
        return flag;
    }
};

int fd[2], bufsize;

//Функция потока записи
static void *func1(void *param)
{
    struct thread_params *params = (struct thread_params *) param;

    char buf[256];
    while (params->GetFlag())
    {
        fcntl(fd[1], F_SETFL, O_NONBLOCK);
        string str = to_string(getegid());
        strcpy(buf, str.c_str());
        bufsize = str.length();

        //Проверяем что записалось и выводим ошибку если нет
        if ( write(fd[1], buf, bufsize) == -1 )
            perror("Write");

        sleep(1);
    }
    pthread_exit((void *) 1);
}

//Функция потока чтения
static void *func2(void *param)
{
    struct thread_params *params = (struct thread_params *) param;

    char buf[256];
    while (params->GetFlag())
    {
        fcntl(fd[0], F_SETFL, O_NONBLOCK);
        buf[0] = '\0';
        int nr = read(fd[0], buf, bufsize);

        if ( nr == -1 )
        {
            perror("Pipe");
            sleep(1);
        }
        else if ( nr == 0 )
        {
            cout<<"End of file, writing channel closed\n";
            sleep(1);
        }
        else if ( nr > 0 )
            cout << buf << endl;
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

    int rv = pipe(fd);

    cout << "Pipe code:" << rv << "\nPress ENTER for start";
    cin.get();
    cout << "\nThreads started\nPress ENTER for stop\n";
    //Создаем потоки
    pthread_create(&id1, nullptr, func1, &params1);
    pthread_create(&id2, nullptr, func2, &params2);

    cin.get();
    cout << "Threads stopped\n";
    params1.SetFlag(false);
    params2.SetFlag(false);

    //threadcode 1 и 2 - переменные для хранения кода завершения потока
    int threadcode1, threadcode2;
    pthread_join(id1, (void **) &threadcode1);
    pthread_join(id2, (void **) &threadcode2);

    //Закрываем дескрипторы пайпы
    close(fd[1]);
    close(fd[0]);

    //Вывод кодов завершения
    cout << "1 thread code:" << threadcode1 << endl << "2 thread code:" << threadcode2 << endl;
    return 0;
}
