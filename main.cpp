#include "stdio.h"
#include "stdlib.h"
#include "ffb_pool_API.h"
#include "ffb_pool_debug.h"
#include "string.h"


#define blockSize 8
#define blockQuantity 0x18



static uint8_t	buf[blockQuantity * blockSize];

uint8_t testSrc[blockSize];
void* ptr[blockQuantity];
int cnt, cntB;


int main(int argc, char** argv) {
	for(cnt=0; cnt<(blockSize * blockSize); cnt++){
		testSrc[cnt]=cnt;
	}
	
	for(cnt=0; cnt<(blockSize * blockSize); cnt++)
		printf("0x%02X ",testSrc[cnt]);
	
	/* Declarations memory pool ID */
	FFB_Pool_ID PoolID;
	PoolID = FFB_POOL_API.init(&buf, sizeof(buf), blockSize);
	
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
	}	
	
	//FFB_DEBUG_LoopTest(PoolID, 500000, blockQuantity);
	return 0;
}

