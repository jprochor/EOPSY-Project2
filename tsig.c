#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#define NUM_CHILD 5

#define WITH_SIGNALS
//two versions of a program, below version WITH_SIGNALS
#ifdef WITH_SIGNALS

int b=0; //counting children
int c=0; //marking interrupt occurance
int d=0; //intermediate variable to calculate child ID

pid_t table[NUM_CHILD];  
pid_t a;



void sig_handler1()  //my defined handler for SIGINT (CTRL+C on keyboard)
{
printf("\nReceived keyboard interrupt\n");
c++;
for(int i=0;i<b;i++)  //looping over already created children
{
d=getpid()+i+1;
printf("\n[child] %d gets killed early because of interrupt\n", d);
kill(d,SIGKILL);   //ending their dear life early
}
}



void ChildProcess()  // child procesure where they introduce their birth, sleep for 10 seconds and announce the end of their duty
{
printf("[child] pid %d from [parent] pid %d\n",getpid(),getppid()); 
sleep(10);
printf("[child] pid %d completed execution\n", getpid());
exit(0);
}

int main()
{
struct sigaction act;
act.sa_handler = SIG_IGN;   //setting ignoring of all other interrupts
for(int j = 0; j < NSIG; ++j){
   sigaction(j, &act, NULL);
}

signal(SIGINT,sig_handler1);  //defining a signal for SIGINT and the procedure
pid_t id;  




for(int i=0;i<NUM_CHILD;i++) // main loop for creating child processes with fork
{
id=fork();
b=b+1;
if(id ==0) //entering children area
{
signal(SIGINT, SIG_IGN);
table[i]=getpid();
ChildProcess();
}


if(id<0) //id<0 means something went horribly wrong
{
printf("Some child processes were not properly created so terminating them all and exiting with status 1\n");
for(int i=0;i<b;i++)
{
a=table[i];
kill(a,SIGTERM);
}
exit(1);  // send sigterm and exit with status code 1
}


sleep(1);  //delay between each child creation
}

 
for(int i=0;i<NUM_CHILD;i++) //to ensure that all child processes are allowed to finish before program ends
{ 
    wait(NULL); 

}

printf("All child processes finished. There were %d child processes.\n", b);

act.sa_handler = SIG_DFL; //getting rid of previous blockage of interrupts
for(int j = 0; j < NSIG; j++){
   sigaction(j,&act,0);
}



#else  // the first version without signals, all comments from prior part apply here since it's basically a simplified version




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
