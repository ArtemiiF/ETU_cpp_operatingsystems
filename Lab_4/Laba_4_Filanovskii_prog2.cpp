#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <cstring>

using namespace std;

int main()
{
    pid_t pid = fork();

    int status = -1;

    //Дочерний процесс
    if ( pid == 0 )
    {
        cout << "Prog2 child id:" << getpid() << endl;
        char *arr[] = {"1", "2", "3", NULL};

        char *path = getenv("PATH");
        char *envp[] = {path, NULL};

        cout<<"Parent's env:"<<envp[0]<<endl;
        int execcode;
        if ((execcode = execvpe("Laba_4_Filanovskii_prog1", arr, envp)) == -1 )
        {
            perror("execvpe");
        }
    }
    else if ( pid == -1 )
    {
        perror("Process error");
    }
    else
    {
        int w;

        cout << "Prog2 id:" << getpid() << endl;
        cout << "Prog2 parent id:" << getppid() << endl;

        while ((w = waitpid(pid, &status, WNOHANG)) == 0)
        {
            usleep(500000);
        }
        if ( w == -1 )
        {
            perror("Waitpid error");
        }
        else
        {
            cout << "Child process ended with code:" << WEXITSTATUS(status);
        }

    }

    return 0;
}
