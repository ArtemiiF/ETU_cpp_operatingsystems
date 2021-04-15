#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;
struct thread_params
{
private:

    bool flag;
    char symb;

    public:
    thread_params()
    {
        flag = true;
        symb ='x';
    }
    void SetSymb(char ch)
    {
        symb = ch;
    }
    void SetFlag(bool fl)
    {
        flag = fl;
    }
    char GetSymb()
    {
        return symb;
    }
    bool GetFlag()
    {
        return flag;
    }
};


//Функция потока 1
static void *func1(void *param)
{
    struct thread_params *params = (struct thread_params *)param;
    while (params->GetFlag())
    {
        cout << params->GetSymb()<<flush;
        sleep(1);
    }
    pthread_exit((void *) 1);
}

//Функция потока 2
static void *func2(void *param)
{
    struct thread_params *params = (struct thread_params *)param;
    while (params->GetFlag())
    {
        cout << params->GetSymb()<<flush;
        sleep(1);
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

    //Ввод символов для потоков
    cout<<"Input symbol for threads\n";
    cout<<"1 thread:";

    //Проверка на верный ввод
    string c1;
    do
    {
        cin>>c1;
        if (c1.length()!=1)
            cout<<"\nInput SINGLE symbol!!!!\n1 thread:"<<flush;
    } while (c1.length()!=1);

    params1.SetSymb(c1[0]);

    //Проверка на верный ввод
    cout<<"2 thread:";
    string c2;
    do
    {
        cin>>c2;
        if (c2.length()!=1)
            cout<<"Input SINGLE symbol!!!!\n2 thread:"<<flush;
    } while (c2.length()!=1);

    params2.SetSymb(c2[0]);

    cout << "\nThreads started\nPress ENTER for stop\n";
    //Создаем потоки
    pthread_create(&id1, nullptr, func1, &params1);
    pthread_create(&id2, nullptr, func2, &params2);

    //Очищаем буфер потока
    cin.clear();
    cin.ignore();

    cin.get();
    cout << "Threads stopped\n";
    params1.SetFlag(false);
    params2.SetFlag(false);

    //threadcode 1 и 2 - переменные для хранения кода завершения потока
    int threadcode1, threadcode2;
    pthread_join(id1, (void **) &threadcode1);
    pthread_join(id2, (void **) &threadcode2);

    //Вывод кодов завершения
    cout << "1 thread code:" << threadcode1 << endl << "2 thread code:" << threadcode2 << endl;
    return 0;
}
