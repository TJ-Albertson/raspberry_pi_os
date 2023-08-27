//#include "mailbox.h"
#include "3d.h"
#include "printf.h"

#define GPU_QPUS 1

#define GPU_MEM_FLG 0xC // cached=0xC; direct=0x4
#define GPU_MEM_MAP 0x0 // cached=0x0; direct=0x20000000

// VBO from opengl
#define vpm_layout(row_step, element_stride, offset) (offset | 0xa00 | row_step << 12 | element_stride << 20)

#define vw_setup0(rows, units) (2 << 30 | (units) << 23 | (rows<<16))

#define vw_setup1(x,y) ( 3 << 30 | (x) << 16 | (y))


#define BUS_TO_PHYS(addr) (((addr)) & ~0xC0000000)

unsigned int program[] = {
#include "qpu-02.hex"
};

#define as_gpu_address(x) (unsigned int)gpu_pointer + ((void *)x - arm_pointer)

int main(int argc, char *argv[]) {

	/* Enable QPU for execution */
	int qpu_enabled = !qpu_enable(1);
	if (!qpu_enabled) {
		printf("Unable to enable QPU.  Check your firmware is latest.");
		goto cleanup;
	}

	/* Allocate GPU memory and map it into ARM address space */
	unsigned int size = 4096;
	unsigned int align = 4096;

    unsigned int handle = allocate_memory(size, align, GPU_MEM_FLG);
	
	if (!handle) {
		printf("Failed to allocate GPU memory.");
		goto cleanup;
	}

	void *gpu_pointer = (void *)lock_memory(handle);
	void *arm_pointer = mapmem(BUS_TO_PHYS((unsigned)gpu_pointer + GPU_MEM_MAP), size);

	/* Fill result buffer with 0x55 */
	memset(arm_pointer, 0x55, size);

	unsigned int *p = (unsigned int*)arm_pointer;

	/*
	   +---------------+ <----+
	   |  QPU Code     |      |
	   |  ...          |      |
	   +---------------+ <--+ |
	   |  Uniforms     |    | |
	   +---------------+    | |
	   |  QPU0 Uniform -----+ |
	   |  QPU0 Start   -------+
	   +---------------+
	   ...
	   +---------------+
	   | Dest for VPM  |
	   | ...           |
	   +---------------+
	*/

	/* Copy QPU program into GPU memory */
	unsigned *qpu_code = p;
	memcpy(p, program, sizeof program);
	p += (sizeof program)/(sizeof program[0]);

	/* Build Uniforms */
	unsigned *qpu_uniform = p;
	int i;
	for (i = 0; i < GPU_QPUS; ++i) {
	  *p++ = 1;
	  *p++ = (unsigned int)(gpu_pointer + 2048 + i * 16 * 6 * 4);
	}
	
	/* Build QPU Launch messages */
	unsigned *qpu_msg = p;
	for (i = 0; i < GPU_QPUS; ++i) {
	  *p++ = as_gpu_address(qpu_uniform + i * 4 * 2);
	  *p++ = as_gpu_address(qpu_code);
	}

	// Test buffer
	printf("before:");
	for (i=0; i<size/4; i++) {
		if ((i%8)==0) printf("\n%08x:", gpu_pointer+i*4);
		printf(" %08x", ((unsigned *)arm_pointer)[i]);
	}
	printf("\n");

	/* Launch QPU program and block till its done */
    	unsigned r = execute_qpu(mb, GPU_QPUS, as_gpu_address(qpu_msg), 1, 10000);
	printf("qpu exec %08x returns %d\n", gpu_pointer + ((void *)qpu_msg - arm_pointer), r);

	// Test buffer
	printf("after:");
	for (i=0; i<size/4; i++) {
		if ((i%8)==0) printf("\n%08x:", gpu_pointer+i*4);
		printf(" %08x", ((unsigned *)arm_pointer)[i]);
	}
	printf("\n");

cleanup:
	/* Release GPU memory */
	if (arm_pointer) {
		unmapmem(arm_pointer, size);
	}
    	if (handle) {
		Unlock_Memory(handle);
    		Release_Memory(handle);
	}
	
	/* Release QPU */
	if (qpu_enabled) {
	    	qpu_enable(mb, 0);
	}
	
	/* Release mailbox */
	// mbox_close(mb);
}

