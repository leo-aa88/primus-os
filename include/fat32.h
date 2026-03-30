#ifndef _FAT32_H
#define _FAT32_H

#include "stdint.h"

typedef struct
{
    uint8_t jump[3];
    uint8_t oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t num_fats;
    uint16_t root_entry_count; // 0 for FAT32
    uint16_t total_sectors_16;
    uint8_t media_type;
    uint16_t fat_size_16; // 0 for FAT32
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    // FAT32 extended
    uint32_t fat_size_32;
    uint16_t ext_flags;
    uint16_t fs_version;
    uint32_t root_cluster;
    uint16_t fs_info;
    uint16_t backup_boot;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t fs_type[8];
} __attribute__((packed)) fat32_bpb_t;

typedef struct
{
    uint8_t name[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t create_time_tenth;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t access_date;
    uint16_t cluster_high;
    uint16_t write_time;
    uint16_t write_date;
    uint16_t cluster_low;
    uint32_t file_size;
} __attribute__((packed)) fat32_entry_t;

#define FAT32_ATTR_DIRECTORY 0x10
#define FAT32_ATTR_ARCHIVE 0x20

void fat32_init(void);
int fat32_read_file(const char *name, uint8_t *buffer, uint32_t max_size);
int fat32_write_file(const char *name, uint8_t *buffer, uint32_t size);

#endif
