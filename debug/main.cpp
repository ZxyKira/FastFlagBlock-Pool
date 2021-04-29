#include "stdio.h"
#include "stdlib.h"
#include "tool_block_pool.h"
#include "ffb_pool_debug.h"
#include "string.h"


#define blockSize 8
#define blockQuantity 0x100

#define FFB_POOL_API tool_block_pool_api

static uint8_t	buf[blockQuantity * blockSize];

uint8_t testSrc[blockSize];
void* ptr[blockQuantity];
int cnt, cntB;

uint8_t poolBuffer[8192];

void thread_A(void){
	FFB_Pool_ID PoolID;
	PoolID = FFB_POOL_API.init(&poolBuffer[0], sizeof(poolBuffer), 64);
	
	void* pSpace = FFB_POOL_API.alloc(PoolID);

	if(pSpace==0x0){
		//memory alloc fail, pool full or mutex lock
		while(1);
	}
	
	memset(pSpace, 0xFF, 64);
	
	bool result = FFB_POOL_API.free(PoolID, pSpace);
	
	if(!result){
		//memory free fail, pool mutex lock
		while(1);
	}
	
	return;
}


int main(int argc, char** argv) {
	
	/* Declarations memory pool ID */
	FFB_Pool_ID PoolID;
	PoolID = FFB_POOL_API.init(&buf, sizeof(buf), blockSize);
	/*
	for(cnt=0; cnt<(blockQuantity * blockSize); cnt++){
		if((cnt!=0)&&(cnt%16==0))
			printf("\n--------------------------------------------------------------------------\n");
		printf("0x%02X ",buf[cnt]);	
	}
	system("pause");
	
	
	system("mode con cols=86 lines=60");
	FFB_DEBUG_ValueShow(PoolID, true);
	system("pause");
	system("cls");
	
	int forMax = FFB_POOL_API.getFreeCount(PoolID);
	
	for(cntB=0; cntB<forMax; cntB++){
		ptr[cntB] = FFB_POOL_API.alloc(PoolID);
		if(ptr[cntB]!=0)
			memset(ptr[cntB], 0xFF, blockSize);
		else{
			printf("\npointer error\n");
		}
		FFB_DEBUG_ValueShow(PoolID, true);
		
		for(cnt=0; cnt<(blockQuantity * blockSize); cnt++){
			if((cnt!=0)&&(cnt%16==0))
				printf("\n--------------------------------------------------------------------------\n");
			printf("0x%02X ",buf[cnt]);	
		}
		printf("\n");
		system("pause");
		system("cls");
	}
	
	ffbStatus result;
	
	for(cntB=0; cntB<forMax; cntB++){
		result=FFB_POOL_API.free(PoolID, ptr[cntB]);
		if(ptr[cntB]!=0)
			memset(ptr[cntB], 0x00, blockSize);
		else{
			printf("\npointer error\n");
		}
		FFB_DEBUG_ValueShow(PoolID, true);
		
		for(cnt=0; cnt<(blockQuantity * blockSize); cnt++){
			if((cnt!=0)&&(cnt%16==0))
				printf("\n--------------------------------------------------------------------------\n");
			printf("0x%02X ",buf[cnt]);	
		}
		printf("\n");
		system("pause");
		system("cls");
	}	*/
	
	FFB_DEBUG_LoopTest(PoolID, 500000, blockQuantity);
	return 0;
}


/*

*/

