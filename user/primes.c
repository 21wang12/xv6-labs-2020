#include "kernel/types.h"
#include "user/user.h"

#define BEGIN 2
#define END 35

/** 
 * Prime Seive:
	p = get a number from left neighbor
	print p
	loop:
		n = get a number from left neighbor
		if (p does not divide n)
			send n to right neighbor
 */

void seive(int* nums,int size);

int main(int argc,char* argv[]){
	int neighbour_pipe[2],nums[END-BEGIN];
	int nums_idx,i;

	pipe(neighbour_pipe);
	if(fork()){
		//parent
		close(neighbour_pipe[0]); //close read end
		printf("prime %d\n",BEGIN);
		for(i=3; i<35; i++){
			if(i%2 != 0){
				write(neighbour_pipe[1],&i,sizeof(i));//write to right
			}
		}
		close(neighbour_pipe[1]); //close write end
		wait(0);// wait until the entire pipeline terminates
	}else{
		//child
		close(neighbour_pipe[1]); //close write end
		nums_idx=0;
		while(read(neighbour_pipe[0],&i,sizeof(i))){
			nums[nums_idx]=i;//read from left
			nums_idx++;
		}
		close(neighbour_pipe[0]); //close read end
		seive(nums,nums_idx);
	}

	exit(0);
}

void seive(int* nums,int size){
	int neighbour_pipe[2];
	int nums_idx,i;

	if(size <=0){
		exit(0);
	}
	
	pipe(neighbour_pipe);

	if(fork()){
		//parent
		close(neighbour_pipe[0]);
		printf("prime %d\n",nums[0]);
		nums_idx=1;
		while(nums_idx < size){
			if(nums[nums_idx]%nums[0] != 0){
				write(neighbour_pipe[1],&nums[nums_idx],sizeof(nums[nums_idx]));
			}
			nums_idx++;
		}
		close(neighbour_pipe[1]);
		wait(0);// wait until the entire pipeline terminates
	}else{
		//child
		close(neighbour_pipe[1]);
		nums_idx=0;
		while(read(neighbour_pipe[0],&i,sizeof(i))){
			nums[nums_idx]=i;
			nums_idx++;
		}
		close(neighbour_pipe[0]);
		seive(nums,nums_idx);
	}
	exit(0);
}