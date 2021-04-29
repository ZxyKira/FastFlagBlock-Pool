#ifndef H_FFB_POOL_DEBUG
#define H_FFB_POOL_DEBUG

#define printx(value) printf("ADDR :0x%08X\n", value)
#define printv(value) printf("VALUE:%6d\n", value)
#define printu()			printf("---------------------------------\n")
#define pdbg(value)		printf("%d\n", value);system("pause")

extern void FFB_DEBUG_ValueShow(FFB_Pool_ID PoolID ,bool ShowFlag);
extern void FFB_DEBUG_LoopTest(FFB_Pool_ID PoolID, uint32_t loopQuantity, uint32_t blockCount);

#endif
