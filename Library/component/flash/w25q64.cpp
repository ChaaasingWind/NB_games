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
    HAL_OSPI_Command(_hospi, &sCommand, W25Q64_TIMEOUT);

    sConfig.Match         = 0x00;
    sConfig.Mask          = 0x01;   // 只看 BUSY 位
    sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval      = 0x10;
    sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
    HAL_OSPI_AutoPolling(_hospi, &sConfig, W25Q64_TIMEOUT);
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