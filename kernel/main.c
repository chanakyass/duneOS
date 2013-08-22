#include "util.h"
#include "screen.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "mem.h"
#include "thread.h"
#include "tss.h"
#include "kb.h"
#include "rtc.h"
#include "timer.h"

extern uintptr_t g_start, g_code, g_data, g_bss, g_end;

void main(struct multiboot_info *mbinfo, multiboot_uint32_t mboot_magic)
{
    /* interrupts are disabled */

    kcls();
    kprintf("Welcome to DuneOS...\n\n");
    if (mboot_magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        kprintf("Multiboot Successful\n");
    } else {
        kprintf("Bad multiboot\n");
        return;
    }

    bss_init();     /* zero all static data */
    gdt_install();
    kprintf("GDT installed\n");
    idt_install();
    kprintf("IDT and ISRs installed\n");
    irq_install();
    kprintf("IRQ handlers installed\n");
    tss_init();
    mem_init(mbinfo);
    kprintf("Memory manager and Heap initialized\n");

    /* re-enable interrupts */
    sti();

    scheduler_init();
    kprintf("Scheduler initialized\n");
    dump_all_threads_list();

    /* paging_install(&g_end); */
    /* kprintf("Paging enabled\n"); */

    char *tmp = "Hello World!\n";
    char *new = malloc(strlen(tmp) + 1);
    memcpy(new, tmp, strlen(tmp));
    new[strlen(tmp)] = '\0';
    kprintf("%s\n", new);
    free(new);

    timer_install();
    keyboard_install();
    rtc_install();

    /* uint32_t* page_fault = 0xFFFFF0000; */
    /* kprintf("page fault? 0x%x\n", *page_fault); */
    /* kprintf("page fault? %u\n", *page_fault); */

    /*
    kprintf("Kernel size in bytes: %u\n", &g_end - &g_start);
    kprintf("Start: 0x%x\n", &g_start);
    kprintf("Code: 0x%x\n", &g_code);
    kprintf("Data: 0x%x\n", &g_data);
    kprintf("BSS: 0x%x\n", &g_bss);
    kprintf("End: 0x%x\n", &g_end);
    */

    struct tm dt;
    while (1) {
        delay(500);
        datetime(&dt);
        kset_cursor(0, 58);
        kprintf("%02u:%02u:%02u %s %02u, %04u\n", dt.hour, dt.min, dt.sec,
                month_name(dt.month), dt.mday, dt.year);
    }

    /* playing around */
    /*
    kprintf("%u\n", 1 / 0);

    beep(2);
    delay(5000);
    kreboot();
    */
}
