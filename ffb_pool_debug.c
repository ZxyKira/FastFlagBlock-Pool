/*****************************************************************************
 *			File Name		:ffb_debug.c
 *			Version			:V1.0
 *			Create Date	:2017-12-13
 *			Modufy Date	:
 *			Information :
 *****************************************************************************/
#include "ffb_pool.h" 
#include "ffb_pool_API.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ffb_pool_debug.h"
#include "time.h"
#include "string.h"
#include <windows.h>
#include <Mmsystem.h>





/*****************************************************************************
 * Private functions
 ****************************************************************************/
 
 /*----------------------------------------------------------------------------
   UI Function
*----------------------------------------------------------------------------*/
static void gotoxy(int x, int y)
{
    static HANDLE h = NULL;  
    if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };  
    SetConsoleCursorPosition(h,c);
}

static void cls(void){
	gotoxy(0,0);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void FFB_DEBUG_ValueShow(FFB_Pool_ID PoolID ,bool ShowFlag){
	ShowFlag=true;
	ffb_info_t *info_debug = (ffb_info_t*)PoolID;
	printf("\n");
	printu();
	printf("isBusy                        =");
	printv(info_debug->isBusy);
	printf("mod                           =");
	printv(info_debug->mod);
	printf("Buffer.pStartPoint            =");
	printx(info_debug->Buffer.pStartPoint);
	printf("Buffer.size                   =");
	printv(info_debug->Buffer.size);
	printf("Block.pFlagBlock              =");
	printx(info_debug->Block.pFlagBlock);
	printf("Block.size                    =");
	printv(info_debug->Block.size);
	printf("Block.Count.blockTotal        =");
	printv(info_debug->Block.Count.blockTotal);
	printf("Block.Count.flagTotal         =");
	printv(info_debug->Block.Count.flagTotal);
	printf("Block.Count.flagUse           =");
	printv(info_debug->Block.Count.flagUse);
	printf("Block.Count.flagStart         =");
	printv(info_debug->Block.Count.flagStart);
	printf("Block.Count.flagCount.value   =");
	printv(info_debug->Block.Count.flagCount.value);
	printf("Block.Count.flagCount.S.flag  =");
	printv(info_debug->Block.Count.flagCount.S.flag);
	printf("Block.Count.flagCount.S.block =");
	printv(info_debug->Block.Count.flagCount.S.block);
	printf("Block.Count.lastFlag.value    =");
	printv(info_debug->Block.Count.lastFlag.value);
	printf("Block.Count.lastFlag.S.flag   =");
	printv(info_debug->Block.Count.lastFlag.S.flag);
	printf("Block.Count.lastFlag.S.block  =");
	printv(info_debug->Block.Count.lastFlag.S.block);
	
	if(ShowFlag){
		int forMax = ((info_debug->Block.Count.blockTotal+3) / 4);
		for(int i=0; i<forMax ; i++){
			char tmp[16];
			for(int j=0; j<4; j++){
				if((forMax*j)+i<info_debug->Block.Count.blockTotal){
					itoa(info_debug->Block.pFlagBlock[(forMax*j)+i], tmp, 2);
					printf("FlagBlock[%02d] = %08s\t",(forMax*j)+i ,tmp);	
				}
			}
			printf("\n");
		}			
	}

	printu();
} 

void FFB_DEBUG_LoopTest(FFB_Pool_ID PoolID, uint32_t loopQuantity, uint32_t blockCount){
//system("mode con cols=130 lines=55");
	system("mode con cols=60 lines=30");
	void* ptr[blockCount];
	uint16_t randomNumb[loopQuantity];	
	printv(sizeof(ffb_info_t));
	memset(&ptr, 0x00, sizeof(ptr));

	
	
	uint16_t randomMax = FFB_POOL_API.getFreeCount(PoolID);
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;

	/*
	for(int x=0; x<(randomMax*0.2); x++){
		ptr[x] = FFB_POOL_API.Alloc();
	}
	
	for(int x=0; x<(randomMax*0.73); x++){
		FFB_POOL_API.Alloc();
	}
	/**/
	

	FFB_DEBUG_ValueShow(PoolID, false);
	system("pause");
	system("cls");
	
	uint32_t count_Alloc,	count_Free, count_Error;
	while(1){
		count_Alloc = 0;
		count_Free 	= 0;
		count_Error = 0;
		for(int x=0; x<loopQuantity; x++)
			randomNumb[x] =	(rand()%randomMax);
			
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBeginTime);
		 
		for(int x=0; x<loopQuantity; x++){
			if(ptr[randomNumb[x]] == 0){ 
				ptr[randomNumb[x]] = FFB_POOL_API.alloc(PoolID);
				(ptr[randomNumb[x]] != 0)?count_Alloc+=1:count_Error+=1;
				
			}
			else{
				FFB_POOL_API.free(PoolID, ptr[randomNumb[x]]);
				ptr[randomNumb[x]] = 0;
				count_Free+=1;
			}
		}
		
		QueryPerformanceCounter(&nEndTime);
		time=(double)((nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart)*1000;
		cls();
		printf("Total time:%14fms\n",time);
		printf("Quantity:%16d\n",loopQuantity);
		time = (time*1000*1000)/loopQuantity;
		printf("One time:%16fns\n",time);
		printf("Different:%6d\n",(count_Alloc-count_Free));
		printf("Error    :%6d\n",count_Error);
		float usin = FFB_POOL_API.getUseCount(PoolID);
		usin = (usin / FFB_POOL_API.getTotalCount(PoolID))*100;
		printf("Using    :%4f%%\n", usin);
		FFB_DEBUG_ValueShow(PoolID, false);
		//system("pause");
	}
}



