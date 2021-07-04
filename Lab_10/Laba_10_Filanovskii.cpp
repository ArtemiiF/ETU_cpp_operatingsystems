#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <cstring>

#define STACK_SIZE (1024*1024)

using namespace std;

static int childFoo(void *arg)
{
    cout<<"(child)Child id:"<<getpid()<<endl;
    cout<<"(child)Parent's id:"<<getppid()<<endl;

    return 0;
}


int main()
{
    char *stack;
    char *stackTop;

    stack = (char *) malloc(STACK_SIZE);
    stackTop = stack + STACK_SIZE;

    cout<<"(parent)Child id:"<<getpid()<<endl;
    cout<<"(parent)Parent's id:"<<getppid()<<endl;

    cout<<"Unshared:"<<endl;
    int child_pid1;
    child_pid1 = clone(childFoo,stackTop,CLONE_NEWUTS|CLONE_NEWPID|SIGCHLD,NULL);
    if(child_pid1==-1)
    {
        perror("Child_pid");
    }

    int status1;
    waitpid(child_pid1,&status1,0);

    cout<<"Shared:"<<endl;
    int child_pid2;
    child_pid2 = clone(childFoo,stackTop,SIGCHLD,NULL);
    if(child_pid2==-1)
    {
        perror("Child_pid");
    }

    int status2;
    waitpid(child_pid2,&status2,0);

    cout<<"The end.\n";
    return 0;
}
