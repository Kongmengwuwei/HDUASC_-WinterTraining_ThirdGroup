#include "flash.h"
#include "menu.h"

/*Flash上传*/
void Flash_Upload(void)
{
	flash_buffer_clear();
	
	uint8_t index = 0;													//按索引读取
	
	for(uint8_t mode = 0; mode < 5; mode++)
	{
		for(uint8_t row = 0; row < 3; row++)
		{
			flash_union_buffer[index].float_type = parameter[mode][row];
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
	
for(uint8_t mode = 0; mode < 5; mode++)
	{
		for(uint8_t row = 0; row < 3;row++)
		{
			parameter[mode][row] = flash_union_buffer[index].float_type;
			
			if(isnan(parameter[mode][row]) || isinf(parameter[mode][row]))	//检验函数（数值异常时才用，一般用不到）
			{
				parameter[mode][row] = 0.0f;
			}
			index++;
			}
	}
}
