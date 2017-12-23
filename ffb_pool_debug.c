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


ffb_info_t *info_debug = 0x00000000;

void MGMGR_DebugInit(void* addr){
	info_debug = (ffb_info_t*)addr;
}

void MGMGR_ValueShow(void){
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
	/*
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
	*/
	printu();
} 

