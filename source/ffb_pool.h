/*****************************************************************************
 *			File Name		:ffb_pool.h
 *			Version			:V1.0
 *			Create Date	:2017-12-06
 *			Modufy Date	:
 *			Information :
 *****************************************************************************/
#ifndef H_FFB_POOL
#define H_FFB_POOL
#include "stdint.h"

typedef struct{
	uint16_t	isBusy;
	uint16_t	mod;
	struct{
		void			*pStartPoint;
		uint32_t	size;
	}Buffer;
	
	struct{
		uint32_t	size;
		uint8_t		*pFlagBlock;
		void			*pStartPoint;
		
		struct{
			uint16_t	blockTotal;
			union{
				uint16_t value;
				struct{
					uint16_t	flag  :3;
					uint16_t	block :13;
				}S;
			}flagTotal;
			uint16_t	flagUse;
			uint16_t	flagStart;
			union{
				uint16_t value;
				struct{
					uint16_t	flag  :3;
					uint16_t	block :13;
				}S;
			}flagCount;
			union{
				uint16_t value;
				struct{
					uint16_t	flag  :3;
					uint16_t	block :13;
				}S;
			}lastFlag;
		}Count;
	}Block;
}ffb_info_t;
#endif
/*****************************************************************************
 * End of file
 ****************************************************************************/

