#ifndef DATA_DEF_H
#define DATA_DEF_H

#include "stdint.h"


union uart_cmd
{
    __attribute__((packed)) struct 
    {
        uint8_t w  : 1  ;
        uint8_t a  : 1  ;
        uint8_t s  : 1  ;
        uint8_t d  : 1  ;
        uint8_t z  : 1  ;
        uint8_t x  : 1  ;
        uint8_t c  : 1  ;
    }msg;

    uint8_t buffer;
};

struct uart_connect_pack
{
    uint8_t head;
    uart_cmd msg;
    uint8_t check;
    uint8_t tail;
};











#endif