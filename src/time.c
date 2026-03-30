#include "../include/time.h"
#include "../include/string.h"
#include "../include/io.h"

#include "stdint.h"

#define RTCaddress 0x0070
#define RTCdata 0x0071

// PORT_CMOS_INDEX nmi disable bit
#define NMI_DISABLE_BIT 0x80

// Standard BIOS RTC chip entries
#define CMOS_RTC_SECONDS 0x00
#define CMOS_RTC_SECONDS_ALARM 0x01
#define CMOS_RTC_MINUTES 0x02
#define CMOS_RTC_MINUTES_ALARM 0x03
#define CMOS_RTC_HOURS 0x04
#define CMOS_RTC_HOURS_ALARM 0x05
#define CMOS_RTC_DAY_WEEK 0x06
#define CMOS_RTC_DAY_MONTH 0x07
#define CMOS_RTC_MONTH 0x08
#define CMOS_RTC_YEAR 0x09
#define CMOS_STATUS_A 0x0a
#define CMOS_STATUS_B 0x0b
#define CMOS_STATUS_C 0x0c
#define CMOS_STATUS_D 0x0d
#define CMOS_RESET_CODE 0x0f

// Holds a complete snapshot of all RTC fields
typedef struct
{
    uint32_t seconds;
    uint32_t minutes;
    uint32_t hours;
    uint32_t day;
    uint32_t month;
    uint32_t year;
    uint32_t registerB;
} rtc_time_t;

uint8_t get_update_in_progress_flag(void)
{
    output_bytes(RTCaddress, NMI_DISABLE_BIT | CMOS_STATUS_A);
    return (input_bytes(RTCdata) & 0x80);
}

uint32_t get_RTC_register(uint8_t reg)
{
    output_bytes(RTCaddress, NMI_DISABLE_BIT | reg);
    return input_bytes(RTCdata);
}

/*
 * Read all RTC registers atomically using a double-read consistency check.
 *
 * The update-in-progress flag has a race window: the RTC can begin an update
 * after we check the flag but before we finish reading registers. The fix is
 * to read all registers twice and retry until both reads agree.
 */
static void read_rtc(rtc_time_t *t)
{
    rtc_time_t prev;

    do
    {
        // Wait for any in-progress update to finish before first read
        while (get_update_in_progress_flag())
        {
        }

        prev.seconds = get_RTC_register(CMOS_RTC_SECONDS);
        prev.minutes = get_RTC_register(CMOS_RTC_MINUTES);
        prev.hours = get_RTC_register(CMOS_RTC_HOURS);
        prev.day = get_RTC_register(CMOS_RTC_DAY_MONTH);
        prev.month = get_RTC_register(CMOS_RTC_MONTH);
        prev.year = get_RTC_register(CMOS_RTC_YEAR);
        prev.registerB = get_RTC_register(CMOS_STATUS_B);

        // Wait again and re-read
        while (get_update_in_progress_flag())
        {
        }

        t->seconds = get_RTC_register(CMOS_RTC_SECONDS);
        t->minutes = get_RTC_register(CMOS_RTC_MINUTES);
        t->hours = get_RTC_register(CMOS_RTC_HOURS);
        t->day = get_RTC_register(CMOS_RTC_DAY_MONTH);
        t->month = get_RTC_register(CMOS_RTC_MONTH);
        t->year = get_RTC_register(CMOS_RTC_YEAR);
        t->registerB = get_RTC_register(CMOS_STATUS_B);

    } while (
        prev.seconds != t->seconds ||
        prev.minutes != t->minutes ||
        prev.hours != t->hours ||
        prev.day != t->day ||
        prev.month != t->month ||
        prev.year != t->year);
}

/*
 * Convert raw RTC register values (BCD or binary) to plain binary,
 * and normalise hours to 24-hour format. Mutates *t in place.
 */
static void normalise_rtc(rtc_time_t *t)
{
    // Convert BCD to binary if the RTC is in BCD mode (bit 2 of status B clear)
    if (!(t->registerB & 0x04))
    {
        t->seconds = (t->seconds & 0x0F) + ((t->seconds >> 4) * 10);
        t->minutes = (t->minutes & 0x0F) + ((t->minutes >> 4) * 10);
        // Hours: bits 6:4 are the tens digit; bit 7 is the PM flag in 12h mode
        t->hours = ((t->hours & 0x0F) + (((t->hours & 0x70) >> 4) * 10)) | (t->hours & 0x80);
        t->day = (t->day & 0x0F) + ((t->day >> 4) * 10);
        t->month = (t->month & 0x0F) + ((t->month >> 4) * 10);
        t->year = (t->year & 0x0F) + ((t->year >> 4) * 10);
    }

    // Convert 12-hour to 24-hour if necessary (bit 1 of status B clear = 12h mode)
    if (!(t->registerB & 0x02) && (t->hours & 0x80))
        t->hours = ((t->hours & 0x7F) + 12) % 24;

    // Expand 2-digit year to full 4-digit year
    t->year += 2000;
}

void datetime(void)
{
    rtc_time_t t;
    char buf[64];

    read_rtc(&t);
    normalise_rtc(&t);

    // Zero-pad all fields for consistent, readable output
    sprintf(buf, "%02u:%02u:%02u - %02u/%02u/%u",
            t.hours, t.minutes, t.seconds,
            t.day, t.month, t.year);
    printk("%s\n", buf);
}

void date(void)
{
    rtc_time_t t;
    char buf[32];

    read_rtc(&t);
    normalise_rtc(&t);

    sprintf(buf, "%02u/%02u/%u", t.day, t.month, t.year);
    printk("%s\n", buf);
}

void clock(void)
{
    rtc_time_t t;
    char buf[32];

    read_rtc(&t);
    normalise_rtc(&t);

    sprintf(buf, "%02u:%02u:%02u", t.hours, t.minutes, t.seconds);
    printk("%s\n", buf);
}

uint32_t current_seconds(void)
{
    rtc_time_t t;
    read_rtc(&t);
    normalise_rtc(&t);
    return t.seconds;
}

uint32_t current_minutes(void)
{
    rtc_time_t t;
    read_rtc(&t);
    normalise_rtc(&t);
    return t.minutes;
}

uint32_t current_hour(void)
{
    rtc_time_t t;
    read_rtc(&t);
    normalise_rtc(&t);
    return t.hours;
}

uint32_t current_day(void)
{
    rtc_time_t t;
    read_rtc(&t);
    normalise_rtc(&t);
    return t.day;
}

uint32_t current_month(void)
{
    rtc_time_t t;
    read_rtc(&t);
    normalise_rtc(&t);
    return t.month;
}

uint32_t current_year(void)
{
    rtc_time_t t;
    read_rtc(&t);
    normalise_rtc(&t);
    return t.year;
}
