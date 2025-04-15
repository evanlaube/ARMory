
int main(void);

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
    __asm__(
            "ldr sp, =_estack\n"  
           );

    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();             
    while(1) (void) 0;  
}

extern void _estack(void);  // Defined in link.ld

// 16 standard and 42 nRF-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 42])(void) = {
    _estack, _reset
};
