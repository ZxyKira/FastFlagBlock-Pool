/* *****************************************************************************************
 *    File Name   :tool_block_pool.c
 *    Create Date :2017-12-04
 *    Modufy Date :2021-04-30
 *    Information :
 */

/* *****************************************************************************************
 *    Include
 */ 
#include <string.h>
#include <stdbool.h>

#include "tool_block_pool.h"



/* *****************************************************************************************
 *    Macro
 */ 

/*----------------------------------------
 *  IS_BLOCK_FLAG_FREE
 *----------------------------------------*/
#define IS_BLOCK_FLAG_FREE(_this ,blockNum, flagNum) (((_this->block.flagBlock[blockNum]&(1<<flagNum)))?false:true)



/*----------------------------------------
 *  IS_BLOCK_FLAG_FREE
 *----------------------------------------*/
#define IS_BLOCK_FULL(_this, blockNum) ((_this->block.flagBlock[blockNum] == 0xFF)?true:false)



/*----------------------------------------
 *  SET_FLAG
 *----------------------------------------*/
#define SET_FLAG(_this, blockNum, flagNum, enable)     \
((enable)?(_this->block.flagBlock[blockNum] |=  (1<<flagNum)):(_this->block.flagBlock[blockNum] &= ~(1<<flagNum)))



/*----------------------------------------
 *  GET_POLL_BLOCK_ADDR
 *----------------------------------------*/
#define GET_POLL_BLOCK_ADDR(_this, value) (void*)((uint32_t)_this + (_this->block.size*value))



/*----------------------------------------
 *  GET_MUTEX
 *----------------------------------------*/
#define GET_MUTEX(_this) ((_this->isBusy == 0)?true:false)



/*----------------------------------------
 *  SET_MUTEX
 *----------------------------------------*/
#define SET_MUTEX(_this, enable) _this->isBusy = enable



/*----------------------------------------
 *  FLAG_ADD
 *----------------------------------------*/
#define FLAG_ADD(_this)                                                             \
if(_this->block.count.flagCount.value >= (_this->block.count.flagTotal.value-1))    \
  _this->block.count.flagCount.value = _this->block.count.flagStart;                \
else                                                                                \
  _this->block.count.flagCount.value++



/*----------------------------------------
 *  BLOCK_FLAG_ADD
 *----------------------------------------*/
#define BLOCK_FLAG_ADD(_this)                                                   \
if(_this->block.count.flagCount.s.block >= (_this->block.count.blockTotal-1))   \
  _this->block.count.flagCount.value = _this->block.count.flagStart;            \
else{                                                                           \
  _this->block.count.flagCount.s.block++;                                       \
  _this->block.count.flagCount.s.flag = 0;                                      \
}  



/*----------------------------------------
 *  BLOCK_FLAG_ADD
 *----------------------------------------*/
#define GET_POOL_BLOCK_FLAG(_this, ptr)    \
((_this->mod == 0)?(((uint32_t)ptr - (uint32_t)_this)/_this->block.size):(((uint32_t)ptr - (uint32_t)_this) >> _this->mod))



/* *****************************************************************************************
 *    Type/Structure
 */ 



/* *****************************************************************************************
 *    Extern Function/Variable
 */

/* *****************************************************************************************
 *    Public Variable
 */

/* *****************************************************************************************
 *    Private Variable
 */

/* *****************************************************************************************
 *    Inline Function
 */
 
/* *****************************************************************************************
 *    Private Function
 */ 

/* *****************************************************************************************
 *    Public Function
 */

/*----------------------------------------
 *  tool_block_reload
 *----------------------------------------*/
tool_block_pool_id_t tool_block_reload(void *pBuffer){
  tool_block_pool_handle_t *pTmp = (tool_block_pool_handle_t*)pBuffer;
  
  if(pTmp->buffer.startPoint == pBuffer){
    return (tool_block_pool_id_t)pBuffer;
  }
  else
    return (void*)0x00000000;
}



/*----------------------------------------
 *  tool_block_init
 *----------------------------------------*/
tool_block_pool_id_t tool_block_init(void *pBuffer,  uint32_t bufferSize,  uint32_t blockSize){
  int32_t temp, cnt;
  tool_block_pool_handle_t *info = (tool_block_pool_handle_t*)pBuffer;
  
  memset(pBuffer, 0x00, sizeof(tool_block_pool_handle_t));
  info->buffer.startPoint   = pBuffer;
  info->buffer.size         =  bufferSize;
  info->block.size          = blockSize;
    
  /*  block  */
  info->block.count.flagTotal.value   = (bufferSize / blockSize);
  info->block.flagBlock = (uint8_t*)((uint32_t)pBuffer + sizeof(tool_block_pool_handle_t));
  info->block.count.blockTotal = ((info->block.count.flagTotal.value + 7) / 8);
  info->block.count.flagStart = ((sizeof(tool_block_pool_handle_t) + info->block.count.blockTotal + blockSize - 1) / blockSize);  
  info->block.count.lastFlag.value = 0;
  memset(info->block.flagBlock, 0x00, info->block.count.blockTotal);

  for(cnt = 0; cnt < info->block.count.flagStart; cnt++){
    uint16_t cntA = (cnt/8);
    uint16_t cntB = (cnt%8);
    SET_FLAG(info, cntA, cntB, true);
  }

  info->block.count.flagUse = info->block.count.flagStart;
  info->block.count.flagCount.value = (info->block.count.flagStart - 1);
    
  if(info->block.count.flagTotal.s.flag != 0){
    for(cnt = info->block.count.flagTotal.s.flag; cnt < 8; cnt++)
      SET_FLAG(info, info->block.count.flagTotal.s.block, cnt, true);
  }
    
  for(cnt=0, temp=0; cnt<16; cnt++){
    if(((blockSize>>cnt)&1) == 1){
      temp++;
    }
      
    if(temp>1)break;
  }
    
  if(temp == 1){
    for(cnt = 1; cnt<16; cnt++){
      if(((blockSize>>cnt)&1) == 1){
        info->mod = cnt;
        break;
      }
    }
  }
  
  return (tool_block_pool_id_t)pBuffer;
}



/*----------------------------------------
 *  tool_block_getUsedCount
 *----------------------------------------*/
uint16_t tool_block_getUsedCount(tool_block_pool_id_t poolID){
  return ((tool_block_pool_handle_t*)poolID)->block.count.flagUse;
}



/*----------------------------------------
 *  tool_block_getFreeCount
 *----------------------------------------*/
uint16_t tool_block_getFreeCount(tool_block_pool_id_t poolID){
  return (((tool_block_pool_handle_t*)poolID)->block.count.flagTotal.value - 
          ((tool_block_pool_handle_t*)poolID)->block.count.flagUse);
}



/*----------------------------------------
 *  tool_block_getFreeCount
 *----------------------------------------*/
uint16_t tool_block_getTotalCount(tool_block_pool_id_t poolID){
  return ((tool_block_pool_handle_t*)poolID)->block.count.flagTotal.value;
}



/*----------------------------------------
 *  tool_block_getFreeCount
 *----------------------------------------*/
void* tool_block_alloc(tool_block_pool_id_t poolID){
  tool_block_pool_handle_t *info = (tool_block_pool_handle_t*)poolID;
  void* result;
  int i, cnt;
  
  
  if(!GET_MUTEX(info))
    return 0;
    
  SET_MUTEX(info, true);
  
  if(info->block.count.flagUse == info->block.count.flagTotal.value) {
    SET_MUTEX(info, false);
    return 0;
  }
    
  
    
  //If Last free flag is not empty.
  if(info->block.count.lastFlag.value != 0){
    result = GET_POLL_BLOCK_ADDR(info, info->block.count.lastFlag.value);
    info->block.count.flagUse+=1;
    SET_FLAG(info, info->block.count.lastFlag.s.block, info->block.count.lastFlag.s.flag, true);
    info->block.count.lastFlag.value = 0;
    SET_MUTEX(info, false);
    return result;
  }
  
  //If next space is free will run into here.
  FLAG_ADD(info);
  if(IS_BLOCK_FLAG_FREE(info ,info->block.count.flagCount.s.block, info->block.count.flagCount.s.flag)){
    result = GET_POLL_BLOCK_ADDR(info, info->block.count.flagCount.value);
    info->block.count.flagUse+=1;
    SET_FLAG(info, info->block.count.flagCount.s.block, info->block.count.flagCount.s.flag, true);
    SET_MUTEX(info, false);
    return result;
  }
  
  //If next space is using will run into here.
  for(i=0; i<info->block.count.blockTotal; i+=1){
    if(!IS_BLOCK_FULL(info, info->block.count.flagCount.s.block)){
      for(cnt=0; cnt<8; cnt++){
        if(IS_BLOCK_FLAG_FREE(info ,info->block.count.flagCount.s.block, cnt)){
          info->block.count.flagCount.s.flag = cnt;
          break;
        }
      }
      
      result = GET_POLL_BLOCK_ADDR(info, info->block.count.flagCount.value);
      info->block.count.flagUse+=1;
      SET_FLAG(info, info->block.count.flagCount.s.block, info->block.count.flagCount.s.flag, true);
      SET_MUTEX(info, false);
      return result;
    }
    BLOCK_FLAG_ADD(info);
  }
  
  /* 
  for(i=0; i < info->block.count.flagTotal.value; i++){
    info->block.count.flagCount.value = i;
    if(IS_BLOCK_FLAG_FREE(info->block.count.flagCount.s.block, info->block.count.flagCount.s.flag)){
      result = GET_POLL_BLOCK_ADDR(info, info->block.count.flagCount.value);
      info->block.count.flagUse+=1;
      SET_FLAG(info, info->block.count.flagCount.s.block, info->block.count.flagCount.s.flag, true);
      FFB_POOL_Mutex(false);
      return result;
    }
  } */ 
  //Never run in here
  SET_MUTEX(info, false);
  return result;
}



/*----------------------------------------
 *  tool_block_free
 *----------------------------------------*/
bool tool_block_free(tool_block_pool_id_t poolID, void* addr){
  tool_block_pool_handle_t *info = (tool_block_pool_handle_t*)poolID;
  
  if(!GET_MUTEX(info))
    return false;
    
  SET_MUTEX(info, true);
  
  if(((uint32_t)addr < (uint32_t)info) | ((uint32_t)addr > ((uint32_t)info + info->buffer.size))){
    SET_MUTEX(info, false);
    return false;
  }
  
  info->block.count.lastFlag.value = GET_POOL_BLOCK_FLAG(info, addr);
  
  if(IS_BLOCK_FLAG_FREE(info ,info->block.count.lastFlag.s.block, info->block.count.lastFlag.s.flag)){
    SET_MUTEX(info, false);
    return false;
  }
  
  info->block.count.flagUse-=1;
  SET_FLAG(info, info->block.count.lastFlag.s.block, info->block.count.lastFlag.s.flag, false);
  SET_MUTEX(info, false);
  return true;
}

/* *****************************************************************************************
 *    API Link
 */

/*----------------------------------------
 *  tool_block_pool_api
 *----------------------------------------*/
const struct tool_block_pool_api_t tool_block_pool_api = {
  .reload            =  tool_block_reload,
  .init              =  tool_block_init,
  .getUsedCount      =  tool_block_getUsedCount,
  .getFreeCount      =  tool_block_getFreeCount,
  .getTotalCount     =  tool_block_getTotalCount,
  .alloc             =  tool_block_alloc,
  .free              =  tool_block_free
};

/*****************************************************************************
 * End of file
 ****************************************************************************/
