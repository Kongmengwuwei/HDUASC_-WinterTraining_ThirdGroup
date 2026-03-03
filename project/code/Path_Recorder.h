#ifndef __PATH_RECORDER_H
#define __PATH_RECORDER_H

#include "zf_common_headfile.h"

// Flash 分区配置
#define PATH_FLASH_SECTOR_START     124
#define PATH_FLASH_SECTOR_COUNT     2
// 路径配置
#define MAX_PATH_POINTS             3000

// 控制参数
#define YAW_CORRECTION_KP           0.02f
#define FLASH_MAGIC_WORD            0x5AA5F00D

typedef struct {
    float left_speed;
    float right_speed;
    float yaw;
	float distance;
} PathPoint_t;

typedef struct {
    PathPoint_t points[MAX_PATH_POINTS];
    uint16_t count;
    uint16_t current_index;
} PathManager_t;

extern PathManager_t path_manager;
extern float Navigation_Speed, Navigation_Turn;
extern float Yaw_offset;
extern float Yaw_start_record;

void PathTracking_Init(void);
void PathRecording_Start(void);     // 新增
void PathTracking_Start(void);      // 新增
void Record_PathPoint(void);
void Navigation_Calculate(void);
void Path_SaveToFlash(void);
uint8_t Path_LoadFromFlash(void);

#endif