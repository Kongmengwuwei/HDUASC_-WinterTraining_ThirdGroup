#include "flash.h"
#include "menu.h"

/*Flash上传*/
void Flash_Upload(void)
{
	flash_buffer_clear();
	
	uint8_t index = 0;													//按索引读取
	
	for(uint8_t mode = 0; mode < MODE_COUNT; mode++)
	{
		for(uint8_t num = 0; num < PID_COUNT; num++)
		{
			flash_union_buffer[index].float_type = pidnum[mode][num];
			index++;
		}
	}
	
	flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
	flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
}

/*Flash下载*/
void Flash_Download(void)
{
	flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
	
	uint8_t index = 0;													//按索引赋值
	
for(uint8_t mode = 0; mode < MODE_COUNT; mode++)
    {
        for(uint8_t num = 0; num < PID_COUNT; num++)
        {
            pidnum[mode][num] = flash_union_buffer[index].float_type;
            
            if(isnan(pidnum[mode][num]) || isinf(pidnum[mode][num]))	//检验函数（数值异常时才用，一般用不到）
            {
                pidnum[mode][num] = 0.0f;
            }
            index++;
        }
    }
}
