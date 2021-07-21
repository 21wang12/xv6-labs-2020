#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char *argv[]){
	int time;
	if(argc < 2){
		write(1,"You forget to pass an argument\n",50);	
	}
	time = atoi(argv[1]);
	sleep(time);
	exit(0);
}
