#ifndef __FLASH_H
#define __FLASH_H

#include "zf_common_headfile.h"
#include "math.h"

#define FLASH_SECTION_INDEX			(127)
#define FLASH_PAGE_INDEX			(FLASH_PAGE_3)

void Flash_Upload(void);
void Flash_Download(void);

#endif
