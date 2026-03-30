#include "../include/fat32.h"
#include "../include/ata.h"
#include "../include/string.h"
#include "../include/tty.h"

static fat32_bpb_t bpb;
static uint32_t fat_start;
static uint32_t data_start;
static uint32_t root_cluster;

static uint32_t cluster_to_lba(uint32_t cluster)
{
    return data_start + (cluster - 2) * bpb.sectors_per_cluster;
}

static uint32_t fat_next_cluster(uint32_t cluster)
{
    uint8_t sector[512];
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = fat_start + (fat_offset / 512);
    uint32_t offset = fat_offset % 512;
    ata_read_sector(fat_sector, sector);
    return *((uint32_t *)&sector[offset]) & 0x0FFFFFFF;
}

void fat32_init(void)
{
    uint8_t sector[512];
    ata_read_sector(0, sector);
    uint8_t *p = sector;
    // copy BPB
    for (int i = 0; i < (int)sizeof(fat32_bpb_t); i++)
        ((uint8_t *)&bpb)[i] = p[i];

    fat_start = bpb.reserved_sectors;
    data_start = fat_start + (bpb.num_fats * bpb.fat_size_32);
    root_cluster = bpb.root_cluster;
    printk("\nFAT32 initialized\n");
}

// convert "filename.ext" to FAT32 8.3 format "FILENAMEEXT"
static void to_83(const char *name, char *out)
{
    int i, j;
    for (i = 0; i < 11; i++)
        out[i] = ' ';
    for (i = 0, j = 0; name[i] && name[i] != '.' && j < 8; i++, j++)
    {
        out[j] = (name[i] >= 'a' && name[i] <= 'z') ? name[i] - 32 : name[i];
    }
    while (name[i] && name[i] != '.')
        i++;
    if (name[i] == '.')
    {
        i++;
        for (j = 8; name[i] && j < 11; i++, j++)
        {
            out[j] = (name[i] >= 'a' && name[i] <= 'z') ? name[i] - 32 : name[i];
        }
    }
}

int fat32_read_file(const char *name, uint8_t *buffer, uint32_t max_size)
{
    char fname[11];
    to_83(name, fname);

    uint32_t cluster = root_cluster;
    uint8_t sector[512];

    while (cluster < 0x0FFFFFF8)
    {
        uint32_t lba = cluster_to_lba(cluster);
        for (uint32_t s = 0; s < bpb.sectors_per_cluster; s++)
        {
            ata_read_sector(lba + s, sector);
            fat32_entry_t *entry = (fat32_entry_t *)sector;
            for (int i = 0; i < 512 / 32; i++)
            {
                if (entry[i].name[0] == 0x00)
                    return -1; // end of dir
                if (entry[i].name[0] == 0xE5)
                    continue; // deleted
                if (entry[i].attributes & 0x0F)
                    continue; // LFN
                if (memcmp(entry[i].name, fname, 11) == 0)
                {
                    // found — read data
                    uint32_t file_cluster = ((uint32_t)entry[i].cluster_high << 16) | entry[i].cluster_low;
                    uint32_t remaining = max_size;
                    uint32_t offset = 0;
                    while (file_cluster < 0x0FFFFFF8 && remaining > 0)
                    {
                        uint32_t flba = cluster_to_lba(file_cluster);
                        for (uint32_t fs = 0; fs < bpb.sectors_per_cluster && remaining > 0; fs++)
                        {
                            ata_read_sector(flba + fs, sector);
                            uint32_t to_copy = remaining > 512 ? 512 : remaining;
                            for (uint32_t b = 0; b < to_copy; b++)
                                buffer[offset++] = sector[b];
                            remaining -= to_copy;
                        }
                        file_cluster = fat_next_cluster(file_cluster);
                    }
                    return offset;
                }
            }
        }
        cluster = fat_next_cluster(cluster);
    }
    return -1; // not found
}

int fat32_write_file(const char *name, uint8_t *buffer, uint32_t size)
{
    // for now: find existing file and overwrite first cluster only
    // full allocation requires a free cluster scanner — coming next
    char fname[11];
    to_83(name, fname);

    uint32_t cluster = root_cluster;
    uint8_t sector[512];

    while (cluster < 0x0FFFFFF8)
    {
        uint32_t lba = cluster_to_lba(cluster);
        for (uint32_t s = 0; s < bpb.sectors_per_cluster; s++)
        {
            ata_read_sector(lba + s, sector);
            fat32_entry_t *entry = (fat32_entry_t *)sector;
            for (int i = 0; i < 512 / 32; i++)
            {
                if (entry[i].name[0] == 0x00)
                    return -1;
                if (entry[i].name[0] == 0xE5)
                    continue;
                if (entry[i].attributes & 0x0F)
                    continue;
                if (memcmp(entry[i].name, fname, 11) == 0)
                {
                    uint32_t file_cluster = ((uint32_t)entry[i].cluster_high << 16) | entry[i].cluster_low;
                    uint32_t flba = cluster_to_lba(file_cluster);
                    uint8_t write_sector[512];
                    uint32_t offset = 0;
                    while (file_cluster < 0x0FFFFFF8 && offset < size)
                    {
                        for (uint32_t fs = 0; fs < bpb.sectors_per_cluster && offset < size; fs++)
                        {
                            for (int b = 0; b < 512; b++)
                                write_sector[b] = (offset < size) ? buffer[offset++] : 0;
                            ata_write_sector(flba + fs, write_sector);
                        }
                        file_cluster = fat_next_cluster(file_cluster);
                        if (file_cluster < 0x0FFFFFF8)
                            flba = cluster_to_lba(file_cluster);
                    }
                    return offset;
                }
            }
        }
        cluster = fat_next_cluster(cluster);
    }
    return -1;
}
