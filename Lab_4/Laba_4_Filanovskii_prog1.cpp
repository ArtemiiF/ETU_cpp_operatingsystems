#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[],char *envp[])
{
    char *env = getenv("PWD");

    if (env==NULL)
    {
        perror("Child getenv");
        //printf("%s",env);
    }
    else
    {
        cout << "Prog1 env:" << env << endl;
    }
    cout << "Prog1 id:" << getpid() << endl;
    cout << "Prog1 parent id:" << getppid() << endl;

    for ( int i = 0; i < argc; i++ )
    {
        cout << argv[i] << endl;
        sleep(1);
    }

   exit(10);

}

