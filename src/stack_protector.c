#include <stdint.h>

uintptr_t __stack_chk_guard = 0xDEADC0DE;

__attribute__((noreturn)) void __stack_chk_fail(void)
{
    // Halt — stack smash detected
    for (;;)
        __asm__ volatile("hlt");
}

// GCC on i386 calls the local variant
__attribute__((noreturn)) void __stack_chk_fail_local(void)
{
    __stack_chk_fail();
}
