#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>

#define DBFS_FILE_PATH  "/sys/kernel/debug/paddr/output"
#define PADDR   0xff000

struct packet {
        pid_t pid;
        unsigned long vaddr;
        unsigned long paddr;
};

struct vadd{
	char* name;
	unsigned long vad;
};

int uninit_global;
int init_global=0;

void read_only(){}


int main(void)
{	
	char *ptr = NULL;
	
	int a;
        int b;
        int c;

        //printf(" virtual address of read_only:       %p\n", &read_only);

        //printf(" virtual address of main:            %p\n", &main);
	//printf(" virtual address of init_global:     %p\n", &init_global);
        //printf(" virtual address of uninit_global:   %p\n", &uninit_global);

        ptr=(char*)malloc(100);
        //printf(" virtual address of ptr:             %p\n", ptr);

	//printf(" virtual address of malloc:          %p\n", &malloc);
	//printf(" virtual address of printf:          %p\n", &printf);

        //printf(" virtual address of a:               %p\n", &a);
        //printf(" virtual address of b:               %p\n", &b);
        //printf(" virtual address of c:               %p\n", &c);
	
	int fd, mem, ret;
        struct packet pckt;
	struct vadd temp[10];
	temp[0].vad = &read_only;
	temp[0].name= "read_only     ";
	temp[1].vad = &main;
	temp[1].name= "main          ";
	temp[2].vad = &init_global;
	temp[2].name= "init_global   ";
	temp[3].vad = &uninit_global;
	temp[3].name= "uninit_global ";
	temp[4].vad = ptr;
	temp[4].name= "ptr           ";
	temp[5].vad = &malloc;
	temp[5].name= "malloc        ";
	temp[6].vad = &printf;
	temp[6].name= "printf        ";
	temp[7].vad = &a;
	temp[7].name= "a             ";
	temp[8].vad = &b;
	temp[8].name= "b             ";
	temp[9].vad = &c;
	temp[9].name= "c             ";

        mem = open("/dev/mem", O_RDWR);

        pckt.pid = getpid();

	for(int i=0;i<10;i++){
	        pckt.vaddr = temp[i].vad;
		pckt.paddr = 0;

		fd = open(DBFS_FILE_PATH, O_RDWR);

        	if (fd < 0) {
                	printf("debugfs file open error\n");
                	exit(-1);
        	}


        	ret = read(fd, &pckt, sizeof(struct packet));

        	if (ret < 0) {
                	printf("debugfs input file write failed\n");
                	exit(-1);
        	}

        	printf("%s(virtual address / physical address) = %p / %p  \n",temp[i].name ,pckt.vaddr, pckt.paddr);
	
		close(fd);
	}
        
	while(1){
		sleep(1);	
	}
        munmap((void*)pckt.vaddr, sizeof(unsigned long));

        return 0;
}
