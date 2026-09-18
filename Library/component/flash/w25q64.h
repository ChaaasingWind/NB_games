#ifndef W25Q64_H
#define W25Q64_H

#include "main.h"



constexpr int W25Q64_TIMEOUT = 1000;


struct song_flash_data
{
    // 事件数
    uint16_t voice_num[5];
    // 偏移量
    uint32_t voice_offset[5];
    //歌曲名
    char name[20];
};



class w25q64
{
  public:
    //初始化
    void w25q64_init();
    //读id
    uint32_t w25q64_read_id();
    //擦除区域
    void     w25q64_erase_sector(uint32_t addr);
    //写信息
    bool     w25q64_page_program(uint32_t addr, const uint8_t* data, uint16_t len);
    //读信息
    bool     w25q64_read_data(uint32_t addr, uint8_t* buffer, uint32_t len);
    //写使能
    void     w25q64_write_enable();
    //等待
    void     w25q64_wait_ready();
    //内存映射模式
    bool w25q64_enable_memory_mapped();
    bool w25q64_disable_memory_mapped();


    //自动完成分页操作的页编程
    uint32_t w25q64_write_data(uint32_t addr, const uint8_t* data, uint32_t len);


    w25q64(OSPI_HandleTypeDef* hospi);
    static w25q64& instance(OSPI_HandleTypeDef* hospi)
    {
        static w25q64 _instance(hospi);
        return _instance;
    }

    OSPI_HandleTypeDef* _hospi;
    OSPI_RegularCmdTypeDef sCommand;

};






#endif