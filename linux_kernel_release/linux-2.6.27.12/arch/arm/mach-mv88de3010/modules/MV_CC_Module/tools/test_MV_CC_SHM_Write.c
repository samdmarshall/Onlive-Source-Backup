/*
 * mapper.c -- simple file that mmap()s a file region and prints it
 *
 * Copyright (C) 1998,2000,2001 Alessandro Rubini
 * 
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>

#include "MV_SHM_def.h"




int TestFramePrint(unsigned char *pFrameBuf,
					int FrameSize)
{
	int i,count = 0, column;

	printf("print from %08xh , size = %08xh\n", (unsigned long)pFrameBuf, FrameSize);

	printf("------------------------------------------------------------\n");	
	printf("Address   |");
	for (i=0; i<16; i++)
		printf(" %02X", i);		
	printf("\n");
	printf("------------------------------------------------------------\n");
		
	while (count < FrameSize) {
		
		printf("%08Xh |", count);
		
		for (column=0; column<16; column++) {
			
			printf(" %02X", pFrameBuf[count]);
			
			count++;
			if (count == (FrameSize) )
				break;
		}
		printf("\n");
	}	
	
	printf("------------------------------------------------------------\n");	
	return 0;		
}



int main(int argc, char **argv)
{
    char *fname;
    FILE *f;
    unsigned long offset, len, value;
    void *address;


    if (argc !=4
       || sscanf(argv[1],"%li", &offset) != 1
       || sscanf(argv[2],"%li", &len) != 1
       || sscanf(argv[3],"%li", &value) != 1) {
        fprintf(stderr, "%s: Usage \"%s <offset> <len> <Value>\"\n", argv[0],
                argv[0]);
        exit(1);
    }
    /* the offset might be big (e.g., PCI devices), but conversion trims it */
    if (offset == INT_MAX) {
	if (argv[1][1]=='x')
            sscanf(argv[1]+2, "%lx", &offset);
        else
            sscanf(argv[1], "%lu", &offset);
    }
    
   	printf("<offset = %d> <len= %d> <value= %02x>\n", offset, len, (unsigned char)value);

    fname = MV_SHM_DeviceName;

    if (!(f=fopen(fname,"r+"))) {
        fprintf(stderr, "%s: %s: %s\n", argv[0], fname, strerror(errno));
        exit(1);
    }

//    address = mmap(NULL, MV_SHM_FullSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(f), 0);

	address = mmap(0, MV_SHM_FullSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(f), 0);

    if (address == (void *)-1) {
        fprintf(stderr,"%s: mmap(): %s\n",argv[0],strerror(errno));
        exit(1);
    }
    
    fclose(f);
    
    fprintf(stderr, "mapped \"%s\" from %lu (0x%08lx) to %lu (0x%08lx)\n",
            fname, address, MV_SHM_Start_PhysAddr, address+MV_SHM_FullSize, MV_SHM_Start_PhysAddr+MV_SHM_FullSize);

	memset(address + offset, (unsigned char)value, len);

    //fwrite(address, 1, len, stdout);
    TestFramePrint((void *)(((unsigned long)address + offset) & 0xFFFFFF00), (len & 0xFFFFFF00) + 0x100 );
    
    return 0;
}
        
