/* *****************************************************************************************
 *    File Name   :tool_block_pool.h
 *    Create Date :2017-12-06
 *    Modufy Date :2021-04-30
 *    Information :
 */

#ifndef TOOL_BLOCK_POOL_VERSION
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* *****************************************************************************************
 *    Include
 */ 
#include <stdint.h>
#include <stdbool.h>

#include "version.h"



/* *****************************************************************************************
 *    Macro
 */ 

/*----------------------------------------
 *  TOOL_BLOCK_POOL_VERSION
 *----------------------------------------*/
#define TOOL_BLOCK_POOL_VERSION VERSION_DEFINE(1, 0, 0)



/* *****************************************************************************************
 *    Typedef List
 */ 




/* *****************************************************************************************
 *    Typedef Function
 */ 
typedef struct tool_block_pool_handle_t* tool_block_pool_id;
/* *****************************************************************************************
 *    Struct/Union/Enum
 */ 

/*----------------------------------------
 *  tool_block_pool_api_t
 *----------------------------------------*/
struct tool_block_pool_api_t{
	tool_block_pool_id	  (*reload)        (void *pBuffer);
	tool_block_pool_id	  (*init)          (void *pBuffer, uint32_t bufferSize, uint32_t blockSize);
	uint16_t		          (*getUsedCount)  (tool_block_pool_id poolID);
	uint16_t		          (*getFreeCount)  (tool_block_pool_id poolID);
	uint16_t		          (*getTotalCount) (tool_block_pool_id poolID);
	void* 			          (*alloc)         (tool_block_pool_id poolID);
	bool      	          (*free)          (tool_block_pool_id poolID, void* addr);
};



/*----------------------------------------
 *  tool_block_pool_flag_t
 *----------------------------------------*/
union tool_block_pool_flag_t{
	uint16_t value;
	struct{
		uint16_t	flag  :3;
	  uint16_t	block :13;
  }s;
};



/*----------------------------------------
 *  tool_block_pool_handle_t
 *----------------------------------------*/
typedef struct{
	uint16_t	isBusy;
	uint16_t	mod;
	
	struct{
		void			*startPoint;
		uint32_t	size;
	}buffer;
	
	struct{
		uint32_t	size;
		uint8_t		*flagBlock;
		void			*startPoint;
		
		struct{
			uint16_t	blockTotal;
			uint16_t	flagUse;
			uint16_t	flagStart;
			union tool_block_pool_flag_t flagTotal;
			union tool_block_pool_flag_t flagCount;
			union tool_block_pool_flag_t lastFlag;
		}count;
	}block;
}tool_block_pool_handle_t;


/* *****************************************************************************************
 *    Typedef Struct/Union/Enum
 */ 

/* *****************************************************************************************
 *    Inline Function
 */ 

/* *****************************************************************************************
 *    Extern
 */ 
extern const struct tool_block_pool_api_t tool_block_pool_api;



#ifdef __cplusplus
}
#endif //__cplusplus
#endif //TOOL_BLOCK_POOL_VERSION
/* *****************************************************************************************
 *  End of file
 */

