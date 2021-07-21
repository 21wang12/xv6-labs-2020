#include "kernel/types.h"
#include "kernel/fs.h"
/**
    #define DIRSIZ 14

    struct dirent {
        ushort inum;
        char name[DIRSIZ];
    };
 */
#include "kernel/stat.h"
/**
    struct stat {
        int dev;     // File system's disk device
        uint ino;    // Inode number
        short type;  // Type of file
        short nlink; // Number of links to file
        uint64 size; // Size of file in bytes
    };
 */
#include "kernel/fcntl.h"
#include "user/user.h"

#define BUF_LEN 512

void find(char* directory,char* target);

int main(int argc,char* argv[]){

    char* directory = argv[1]; // directory
    char* target = argv[2]; //target file

    find(directory,target);//recursive find

    exit(0);
}

void find(char* directory,char* target){
    struct stat st;
    struct dirent de;
    int fd,fd2;
    char buf[BUF_LEN],*d;

    //open directory
    fd = open(directory,O_RDONLY);
    if(fd<0){
        fprintf(2,"find: can not open %s\n",directory);
        exit(0);
    }
    
    // Directory is a file containing a sequence of dirent structures.
    while (read(fd,&de,sizeof(de)) == sizeof(de)){
        memset(buf,0,BUF_LEN);
        //Don't recurse into "." and "..". Skip dirent.inum=0 .
        if(strcmp(".",de.name)==0 || strcmp("..",de.name)==0 || de.inum==0){
            continue;
        }
        //buf = directory+"/"+de.name
        strcpy(buf,directory);
        d = buf+strlen(buf);
        *d++ = '/';
        strcpy(&buf[strlen(buf)],de.name);
        fd2 = open(buf,O_RDONLY);//open(directory+"/"+de.name)
        if(fd2<0){
            fprintf(2,"find: can not open %s\n",de.name);
            exit(0);
        }

        fstat(fd2,&st);
        if(st.type == T_DIR){
            //find(directory+"/"+de.name,target);
            find(buf,target);
        }else if (st.type == T_FILE && strcmp(de.name,target) == 0)
        {
            //print directory+"/"+de.name
            printf("%s\n",buf);
        }
        close(fd2);
    }
    close(fd);
}
