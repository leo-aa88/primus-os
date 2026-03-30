#ifndef _ATA_H
#define _ATA_H

#include "stdint.h"

#define ATA_PRIMARY_DATA 0x1F0
#define ATA_PRIMARY_ERR 0x1F1
#define ATA_PRIMARY_SECTOR_COUNT 0x1F2
#define ATA_PRIMARY_LBA_LO 0x1F3
#define ATA_PRIMARY_LBA_MID 0x1F4
#define ATA_PRIMARY_LBA_HI 0x1F5
#define ATA_PRIMARY_DRIVE_HEAD 0x1F6
#define ATA_PRIMARY_STATUS 0x1F7
#define ATA_PRIMARY_COMMAND 0x1F7

#define ATA_CMD_READ 0x20
#define ATA_CMD_WRITE 0x30

#define ATA_SR_BSY 0x80
#define ATA_SR_DRQ 0x08
#define ATA_SR_ERR 0x01

void ata_read_sector(uint32_t lba, uint8_t *buffer);
void ata_write_sector(uint32_t lba, uint8_t *buffer);

#endif
