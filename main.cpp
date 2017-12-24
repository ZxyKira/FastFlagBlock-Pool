#include "stdio.h"
#include "ffb_pool_API.h"
#include "ffb_pool_debug.h"


#define blockSize 4
#define blockQuantity 0x100


static uint8_t	buf[blockQuantity * blockSize];



int main(int argc, char** argv) {
	/* Declarations memory pool ID */
	FFB_Pool_ID PoolID;
	PoolID = FFB_POOL_API.Init(&buf,	sizeof(buf), blockSize);
	
	
	FFB_DEBUG_LoopTest(PoolID, 500000, blockQuantity);
	return 0;
}

