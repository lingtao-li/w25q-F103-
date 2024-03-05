#pragma once
#include <stdint.h>

#pragma pack(push, 1)
class ETX_OTA_COMMAND_
{
public:
	uint8_t	  sof;
	uint8_t   packet_type;
	uint16_t  data_len;
	uint8_t   cmd;
	uint32_t  crc;
	uint8_t   eof;
};
#pragma pack(pop)