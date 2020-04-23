#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#define NUM_CHILD 5

#define WITH_SIGNALS

#ifdef WITH_SIGNALS

int b=0;
int c=0;
int d=0;

pid_t table[NUM_CHILD];
pid_t a;



void sig_handler1()
{
printf("\nReceived keyboard interrupt\n");
c++;
for(int i=0;i<b;i++)
{
d=getpid()+i+1;
printf("\n[child] %d gets killed early because of interrupt\n", d);
kill(d,SIGKILL);
}
}



void ChildProcess()
{
printf("[child] pid %d from [parent] pid %d\n",getpid(),getppid()); 
sleep(10);
printf("[child] pid %d completed execution\n", getpid());
exit(0);
}

int main()
{
struct sigaction act;
act.sa_handler = SIG_IGN;
for(int j = 0; j < NSIG; ++j){
   sigaction(j, &act, NULL);
}

signal(SIGINT,sig_handler1);
pid_t id;




for(int i=0;i<NUM_CHILD;i++)
{
id=fork();
b=b+1;
if(id ==0)
{
signal(SIGINT, SIG_IGN);
table[i]=getpid();
ChildProcess();
}


if(id<0)
{
printf("Some child processes were not properly created so terminating them all and exiting with status 1\n");
for(int i=0;i<b;i++)
{
a=table[i];
kill(a,SIGTERM);
}
exit(1);
}


sleep(1);
}

 
for(int i=0;i<NUM_CHILD;i++)
{ 
    wait(NULL); 

}

printf("All child processes finished. There were %d child processes.\n", b);

act.sa_handler = SIG_DFL;
for(int j = 0; j < NSIG; j++){
   sigaction(j,&act,0);
}



#else




void ChildProcess()
{
printf("[child] pid %d from [parent] pid %d\n",getpid(),getppid()); 
sleep(10);
printf("[child] pid %d completed execution\n", getpid());
exit(0);
}


int main()
{
pid_t id;
pid_t a;
pid_t table[NUM_CHILD];

for(int i=0;i<NUM_CHILD;i++)
{
id=fork();
if(id ==0)
{
table[i]=getpid();
ChildProcess();
}



if(id<0)
{
printf("Some child processes were not properly created so terminating them all and exiting with status 1\n");
for(int i=0;i<NUM_CHILD;i++)
{
a=table[i];
kill(a,SIGTERM);
}
exit(1);
}


sleep(1);
}

 
for(int i=0;i<NUM_CHILD;i++) 
    wait(NULL); 

printf("All child processes finished. There were %d child processes.\n", NUM_CHILD);


  

 #endif
    return 0;
}
