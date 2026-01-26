#include "flash.h"
#include "math.h"

void Flash_Upload(void)
{
	flash_buffer_clear();
	flash_union_buffer[0].float_type = num[0];
	flash_union_buffer[1].float_type = num[1];
	flash_union_buffer[2].float_type = num[2];
	flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
	flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
}

void Flash_Download(void)
{
	flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
	num[0] = flash_union_buffer[0].float_type;
	num[1] = flash_union_buffer[1].float_type;
	num[2] = flash_union_buffer[2].float_type;
	
	if (isnan(num[0]) || isinf(num[0])) num[0] = 0.0f;
	if (isnan(num[1]) || isinf(num[1])) num[1] = 0.0f;
	if (isnan(num[2]) || isinf(num[2])) num[2] = 0.0f;
}
