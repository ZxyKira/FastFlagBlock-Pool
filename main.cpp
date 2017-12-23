#include "stdio.h"
#include "stdlib.h"
#include "ffb_pool_API.h"
#include "ffb_pool_debug.h"
#include "ffb_pool.h"
#include "time.h"
#include "string.h"
#include <windows.h>
#include <Mmsystem.h>


#define loopQuantity 500000
#define blockSize 4
#define testQ 0x100

uint16_t randomNumb[loopQuantity];
static uint8_t	buf[testQ * blockSize];
static void* ptr[testQ];
FFB_Pool_ID PoolID;

void gotoxy(int x, int y)
{
    static HANDLE h = NULL;  
    if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };  
    SetConsoleCursorPosition(h,c);
}

void cls(void){
	gotoxy(0,0);
}




void* ptrNew(){
	static uint16_t value = 0;
	ffbStatus result;
	ptr[value] = FFB_POOL_API.Alloc(PoolID);
	printx(ptr[value]);
	value++;
	return ptr[value-1];
}

ffbStatus ptrFree(uint16_t value){
	return FFB_POOL_API.Free(PoolID, ptr[value]);
}


int main(int argc, char** argv) {
	//system("mode con cols=130 lines=55");
	system("mode con cols=60 lines=30");

	printv(sizeof(ffb_info_t));
	memset(&ptr, 0x00, sizeof(ptr));
	ffbStatus result;


	PoolID = FFB_POOL_API.Init(&buf,	sizeof(buf), blockSize);
	MGMGR_DebugInit(&buf);
	
	
	uint16_t randomMax = FFB_POOL_API.GetFreeCount(PoolID);
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
	

	MGMGR_ValueShow();
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
				ptr[randomNumb[x]] = FFB_POOL_API.Alloc(PoolID);
				(ptr[randomNumb[x]] != 0)?count_Alloc+=1:count_Error+=1;
				
			}
			else{
				FFB_POOL_API.Free(PoolID, ptr[randomNumb[x]]);
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
		float usin = FFB_POOL_API.GetUseCount(PoolID);
		usin = (usin / FFB_POOL_API.GetTotalCount(PoolID))*100;
		printf("Using    :%4f%%\n", usin);
		MGMGR_ValueShow();
		//system("pause");
	}


	return 0;
}
