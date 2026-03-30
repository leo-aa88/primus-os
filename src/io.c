#include "../include/io.h"
#include "../include/tty.h"

void shutdown()
{
    outw(0xB004, 0x2000);
    outw(0x604, 0x2000);
    outw(0x4004, 0x3400);
}

void reboot()
{
    uint8_t temp;

    asm volatile("cli"); /* disable all interrupts */

    /* Clear all keyboard buffers (output and command buffers) */
    do
    {
        temp = input_bytes(KBRD_INTRFC); /* empty user data */
        if (check_flag(temp, KBRD_BIT_KDATA) != 0)
            input_bytes(KBRD_IO); /* empty keyboard data */
    } while (check_flag(temp, KBRD_BIT_UDATA) != 0);

    output_bytes(KBRD_INTRFC, KBRD_RESET); /* pulse CPU reset line */
loop:
    asm volatile("hlt"); /* if that didn't work, halt the CPU */
    goto loop;           /* if a NMI is received, halt again */
}

uint8_t input_bytes(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0"
                         : "= a"(ret)
                         : "Nd"(port));
    return ret;
}

void output_bytes(uint16_t port, uint8_t val)
{
    __asm__ __volatile__("outb %0, %1"
                         :
                         : "a"(val), "Nd"(port));
}

uint8_t inw(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__("in %1, %0"
                         : "= a"(ret)
                         : "d"(port));
    return ret;
}

void outw(uint16_t port, uint16_t data)
{
    __asm__ __volatile__("out %0, %1"
                         :
                         : "a"(data), "d"(port));
}

uint8_t scan(void)
{
    static uint8_t e0 = 0;
    uint8_t sc;

    while ((input_bytes(0x64) & 0x01) == 0)
        ;

    sc = input_bytes(0x60);

    if (sc == 0xE0)
    {
        e0 = 1;
        return 0;
    }

    if (e0)
    {
        e0 = 0;

        // extended break code
        if (sc & 0x80)
            return 0;

        // extended make code
        return sc | 0x80;
    }

    // normal break code
    if (sc & 0x80)
        return 0;

    // normal make code
    return sc;
}

void move_cursor(int row, int col)
{
    unsigned short pos = (row * VGA_WIDTH) + col;
    output_bytes(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    output_bytes(FB_DATA_PORT, (unsigned char)(pos & 0xFF));
    output_bytes(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    output_bytes(FB_DATA_PORT, (unsigned char)((pos >> 8) & 0xFF));
}

void print_prompt(void)
{
    printk("\n$ > ");
    move_cursor(get_terminal_row(), get_terminal_col());
}