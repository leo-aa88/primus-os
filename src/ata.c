#include "../include/ata.h"
#include "../include/io.h"

static void ata_wait_bsy(void)
{
    while (input_bytes(ATA_PRIMARY_STATUS) & ATA_SR_BSY)
        ;
}

static void ata_wait_drq(void)
{
    while (!(input_bytes(ATA_PRIMARY_STATUS) & ATA_SR_DRQ))
        ;
}

void ata_read_sector(uint32_t lba, uint8_t *buffer)
{
    ata_wait_bsy();
    output_bytes(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    output_bytes(ATA_PRIMARY_SECTOR_COUNT, 1);
    output_bytes(ATA_PRIMARY_LBA_LO, (uint8_t)(lba));
    output_bytes(ATA_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));
    output_bytes(ATA_PRIMARY_LBA_HI, (uint8_t)(lba >> 16));
    output_bytes(ATA_PRIMARY_COMMAND, ATA_CMD_READ);
    ata_wait_bsy();
    ata_wait_drq();

    uint16_t *buf16 = (uint16_t *)buffer;
    for (int i = 0; i < 256; i++)
    {
        uint16_t data;
        __asm__ __volatile__("inw %1, %0" : "=a"(data) : "Nd"((uint16_t)ATA_PRIMARY_DATA));
        buf16[i] = data;
    }
}

void ata_write_sector(uint32_t lba, uint8_t *buffer)
{
    ata_wait_bsy();
    output_bytes(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    output_bytes(ATA_PRIMARY_SECTOR_COUNT, 1);
    output_bytes(ATA_PRIMARY_LBA_LO, (uint8_t)(lba));
    output_bytes(ATA_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));
    output_bytes(ATA_PRIMARY_LBA_HI, (uint8_t)(lba >> 16));
    output_bytes(ATA_PRIMARY_COMMAND, ATA_CMD_WRITE);
    ata_wait_bsy();
    ata_wait_drq();

    uint16_t *buf16 = (uint16_t *)buffer;
    for (int i = 0; i < 256; i++)
    {
        __asm__ __volatile__("outw %0, %1" : : "a"(buf16[i]), "Nd"((uint16_t)ATA_PRIMARY_DATA));
    }
}
