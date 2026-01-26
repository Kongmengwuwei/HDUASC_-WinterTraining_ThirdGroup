#ifndef __FLASH_H
#define __FLASH_H

#include "zf_common_headfile.h"
#include "menu.h"

#define FLASH_SECTION_INDEX			(127)					// 存储数据用的扇区 倒数第一个扇区
#define FLASH_PAGE_INDEX			(FLASH_PAGE_3)	// 存储数据用的页码 倒数第一个页码

void Flash_Upload(void);
void Flash_Download(void);

#endif
