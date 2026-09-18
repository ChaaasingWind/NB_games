#include "w25q64.h"

w25q64::w25q64(OSPI_HandleTypeDef* hospi)
{
    _hospi = hospi;


    sCommand = {0};


    sCommand.OperationType          = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId                = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode        = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize        = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode     = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressMode            = HAL_OSPI_ADDRESS_NONE;
    sCommand.AddressSize            = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.AddressDtrMode         = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.AlternateBytesMode     = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode               = HAL_OSPI_DATA_1_LINE;
    sCommand.DataDtrMode            = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DummyCycles            = 0;
    sCommand.DQSMode                = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode               = HAL_OSPI_SIOO_INST_EVERY_CMD;

    sCommand.Instruction        = 0x00;
    sCommand.NbData             = 0;

}




void w25q64::w25q64_init()
{
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);  // HOLD
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);  // WP
}

uint32_t w25q64::w25q64_read_id()
{
    uint8_t id[3] = {0};
    sCommand.Instruction = 0x9F;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
    sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;
    sCommand.NbData      = 3;


    if (HAL_OSPI_Command(_hospi, &sCommand, W25Q64_TIMEOUT) != HAL_OK) {
        return 0xFFFFFFFF;
    }
    if (HAL_OSPI_Receive(_hospi, id, W25Q64_TIMEOUT) != HAL_OK) {
        return 0xFFFFFFFE;
    }

    return ((uint32_t)id[0] << 16) | ((uint32_t)id[1] << 8) | id[2];
}



void w25q64::w25q64_write_enable()
{
    sCommand.Instruction = 0x06;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
    sCommand.DataMode    = HAL_OSPI_DATA_NONE;
    sCommand.NbData      = 0;
    HAL_OSPI_Command(_hospi, &sCommand, W25Q64_TIMEOUT);
}

void w25q64::w25q64_wait_ready()
{
    OSPI_AutoPollingTypeDef sConfig = {0};

    sCommand.Instruction = 0x05;    // Read Status Register 1
    sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
    sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;
    sCommand.NbData      = 1;
    sCommand.DummyCycles = 0;
    HAL_OSPI_Command(_hospi, &sCommand, W25Q64_TIMEOUT);

    sConfig.Match         = 0x00;
    sConfig.Mask          = 0x01;   // 只看 BUSY 位
    sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval      = 0x10;
    sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;

    HAL_StatusTypeDef ret = HAL_OSPI_AutoPolling(_hospi, &sConfig, W25Q64_TIMEOUT);
    if (ret != HAL_OK) {
        // 打印错误，或者强制复位
        _hospi->State = HAL_OSPI_STATE_READY;
    }
}

void w25q64::w25q64_erase_sector(uint32_t addr)
{
    w25q64_write_enable();
    sCommand.Instruction = 0x20;    // Sector Erase (4KB)
    sCommand.Address     = addr;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
    sCommand.DataMode    = HAL_OSPI_DATA_NONE;
    sCommand.NbData      = 0;
    HAL_OSPI_Command(_hospi, &sCommand, W25Q64_TIMEOUT);
    w25q64_wait_ready();
}

bool w25q64::w25q64_page_program(uint32_t addr, const uint8_t* data, uint16_t len)
{
    if (len == 0 || len > 256) return false;

    w25q64_write_enable();
    sCommand.Instruction = 0x02;    // Page Program
    sCommand.Address     = addr;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
    sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;
    sCommand.NbData      = len;

    if (HAL_OSPI_Command(_hospi, &sCommand, W25Q64_TIMEOUT) != HAL_OK) return false;
    if (HAL_OSPI_Transmit(_hospi, (uint8_t*)data, W25Q64_TIMEOUT) != HAL_OK) return false;
    w25q64_wait_ready();
    return true;
}

bool w25q64::w25q64_read_data(uint32_t addr, uint8_t* buffer, uint32_t len)
{
    sCommand.Instruction = 0x03;    // Read Data
    sCommand.Address     = addr;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
    sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;
    sCommand.NbData      = len;

    if (HAL_OSPI_Command(_hospi, &sCommand, W25Q64_TIMEOUT) != HAL_OK) return false;
    return HAL_OSPI_Receive(_hospi, buffer, W25Q64_TIMEOUT) == HAL_OK;
}





// uint32_t w25q64::w25q64_write_data(uint32_t addr, const uint8_t* data, uint32_t len)
// {
//     uint32_t page_remaining = 0;
//     if(addr & 0xFF)
//     {
//         //计算剩余的字节数
//         page_remaining = 256 - (addr & 0xFF);
//     }
//     else 
//     {
//         //直接写满
//         page_remaining = 256;
//     }
//     //判断当前页是否能写完
//     if(len <= page_remaining)
//     {
//         w25q64_page_program(addr, data, len);
//         //返回下一次能直接写的地址
//         return addr + len;
//     }
//     else 
//     {
//         //写不完，先写满当前这一页
//         w25q64_page_program(addr, data, page_remaining);
//         //写到下一页
//         const uint8_t* _data = data + page_remaining;
//         uint32_t _addr = addr + page_remaining;
//         uint32_t _len = len - page_remaining;
//         //递归调用
//         return w25q64_write_data(_addr, _data, _len);
//     }
// }


uint32_t w25q64::w25q64_write_data(uint32_t addr, const uint8_t* data, uint32_t len)
{
    while (len > 0) {
        uint32_t page_remaining = 256 - (addr & 0xFF);
        uint32_t chunk = (len < page_remaining) ? len : page_remaining;

        w25q64_page_program(addr, data, chunk);

        addr += chunk;
        data += chunk;
        len  -= chunk;
    }
    return addr;
}

bool w25q64::w25q64_enable_memory_mapped()
{
    // ---------- 1. 配置"读取"命令 ----------
    sCommand.Instruction    = 0x0B;    // Fast Read
    sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.AddressMode    = HAL_OSPI_ADDRESS_1_LINE;
    sCommand.AddressSize    = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
    sCommand.DummyCycles    = 8;
    sCommand.NbData         = 0;

    // ---------- 2. 先发 Command，把 State 推到 CMD_CFG ----------
    if (HAL_OSPI_Command(_hospi, &sCommand, W25Q64_TIMEOUT) != HAL_OK) {
        return false;
    }

    // ---------- 3. 配置内存映射参数 ----------
    OSPI_MemoryMappedTypeDef sMemMappedCfg = {0};
    sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

    // ---------- 4. 进入内存映射 ----------
    if (HAL_OSPI_MemoryMapped(_hospi, &sMemMappedCfg) != HAL_OK) {
        return false;
    }
    return true;
}

bool w25q64::w25q64_disable_memory_mapped()
{
    return HAL_OSPI_Abort(_hospi) == HAL_OK;
}



