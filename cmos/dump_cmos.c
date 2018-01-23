#include<stdio.h>

#include<stdlib.h>
#include<sys/io.h>

#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

#include<unistd.h>

unsigned char cmos_read(int offset)
{
     unsigned char value;

     ioperm(0x70, 0x2, 1);
     ioperm(0x80, 0x1, 1);

     outb(offset, 0x70);
     outb(0, 0x80);  /* Small delay */
     value = inb(0x71);

     ioperm(0x80, 0x1, 0);
     ioperm(0x70, 0x2, 0);

     return value;
}

int main(int argc, char *argv[]) {
	unsigned char *v = (unsigned char*)malloc(256 * sizeof(unsigned char));
	
	for(int i=0; i<256; i++) {
		v[i] = cmos_read(i);
	}

/*
	for(int i=0; i<256; i++) {
		printf("%02X=%02X\n", i, v[i]);
	}
*/	
	char *default_file_name = "cmos.bin";
	char *output_file_name = (argc > 1) ? argv[1] : default_file_name;

	int h = open(output_file_name, O_CREAT | O_APPEND | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	int cnt = write(h, v, 256 * sizeof(unsigned char));
	if(cnt != 256 * sizeof(unsigned char)) {
		printf("Error writing to '%s' \n", output_file_name);
	}
	close(h);
		
	free(v);

	return 0;
}

