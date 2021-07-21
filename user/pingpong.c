#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char* argv[]){
	int ping[2];
	int pong[2];
	int pid;
	char c;
	
	pipe(ping);		//ping[0] for read, pong[1] for write
	pipe(pong);

	if(fork() == 0){
		//child
		close(ping[1]);
		close(pong[0]);
		read(ping[0],&c,1);
		close(ping[0]);
		pid=getpid();
		printf("%d: received ping\n",pid);
		write(pong[1]," ",1);
		close(pong[1]);
	}else{
		//parent
		close(ping[0]);
		close(pong[1]);
		write(ping[1]," ",1);
		close(ping[1]);
		read(pong[0],&c,1);
		close(pong[0]);
		pid=getpid();
		printf("%d: received pong\n",pid);
	}

	exit(0);
}
