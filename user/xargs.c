#include "kernel/types.h"
#include "kernel/param.h" //#define MAXARG       32  // max exec arguments
#include "user/user.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int main(int argc,char* argv[]){
    char* argument[MAXARG];
    char line[512];
    char* command;
    int argument_idx,line_idx;
    char c;
    int pid;
    if(argc<2){
        printf("xargs: too few argument(s)!");
        exit(0);
    }
    command = argv[1];
    
    //move program name and argument to arguments array
    /**
     * eg:
     * //shell   : xargs echo hello
     * //idx     :     0    1     2
     */
    for(argument_idx=0;argument_idx+1<argc && argument_idx<MAXARG;argument_idx++){
        argument[argument_idx] = argv[argument_idx+1];
    }
    line_idx = 0;

    //read fron stdin
    while (read(STDIN,&c,sizeof(c)) == sizeof(c)){
        if(c != '\n'){
            line[line_idx++] = c;
        }else{
            pid = fork();
            if(pid == 0){
                //child
                argument[argument_idx] = line;
                exec(command,argument);
            }else if(pid > 0){
                //parent
                wait(0);
            }
            line_idx=0;
            memset(line,0,512);
        }
    }

    exit(0);
}