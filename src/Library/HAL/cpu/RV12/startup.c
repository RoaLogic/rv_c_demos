#include <stdio.h>
#include <string.h>

// Generic C function pointer.
typedef void (*function_t)();

// These symbols are defined by the linker script.
extern uint8_t bss_target_start;
extern uint8_t bss_target_end;
extern uint8_t tbss_target_start;
extern uint8_t tbss_target_end;
extern uint8_t data_source_start;
extern uint8_t data_target_start;
extern uint8_t data_target_end;
extern uint8_t tdata_source_start;
extern uint8_t tdata_target_start;
extern uint8_t tdata_target_end;

#ifdef __cplusplus
extern "C" function_t __init_array_start[];
extern "C" function_t __init_array_end[];
#endif
// This function will be placed by the linker script according to the section
void _reset(void)  __attribute__ ((naked, section(".text.init.reset")));

// Define the symbols with "C" linkage as they are used by the assembler
// we need to define it as used, since otherwise GCC cant see that we are using it, for reference see:
// https://stackoverflow.com/questions/31637626/whats-the-usecase-of-gccs-used-attribute
void _start(void) __attribute__ ((noreturn)) __attribute__((__used__)) ;
void _Exit(int exit_code) __attribute__ ((noreturn));

// Standard entry point, no arguments.
extern int main(void);
#ifdef __cplusplus
extern "C" {
#endif

extern void trapHandler();
#define TRAP_HANDLER trapHandler

#ifdef __cplusplus
}
#endif

// The linker script will place this at the reset entry point.
// It will be 'called' with no stack or C runtime configuration.
// NOTE - this only supports a single hart.
// tp will not be initialized
void _reset(void)   
{
    // Setup SP and GP
    // The locations are defined in the linker script
    __asm__ volatile  (
                      "la t0, trapHandler;"
                      "csrw mtvec,t0;"
                      "li    x1, 0;"
                      "li    x2, 0;"
                      "li    x3, 0;"
                      "li    x4, 0;"
                      "li    x5, 0;"
                      "li    x6, 0;"
                      "li    x7, 0;"
                      "li    x8, 0;"
                      "li    x9, 0;"
                      "li    x10, 0;"
                      "li    x11, 0;"
                      "li    x12, 0;"
                      "li    x13, 0;"
                      "li    x14, 0;"
                      "li    x15, 0;"
                      "li    x16, 0;"
                      "li    x17, 0;"
                      "li    x18, 0;"
                      "li    x19, 0;"
                      "li    x20, 0;"
                      "li    x21, 0;"
                      "li    x22, 0;"
                      "li    x23, 0;"
                      "li    x24, 0;"
                      "li    x25, 0;"
                      "li    x26, 0;"
                      "li    x27, 0;"
                      "li    x28, 0;"
                      "li    x29, 0;"
                      "li    x30, 0;"
                      "li    x31, 0;"
                      ".option push;"
                      ".option norelax;"
                      "la    gp, __global_pointer$;"
                      ".option pop;"
                      "la    sp, __stack;"
                      "jal   zero, _start;"
                      :  /* output: none %0 */
                      : "g"(TRAP_HANDLER)/* input: TRAP_HANDLER */
                      : /* clobbers: none */); 
    // This point will not be executed, _start() will be called with no return.
}

// At this point we have a stack and global poiner, but no access to global variables.
void _start(void) 
{
    // Init memory regions
    // Clear the .bss section (global variables with no initial values)
    memset(&bss_target_start , 0U , (&bss_target_end  - &bss_target_start)); 
    memset(&tbss_target_start, 0U , (&tbss_target_end - &tbss_target_start));

    // Initialize the .data section (global variables with initial values)
    memcpy(&data_target_start , &data_source_start , (&data_target_end - &data_target_start));
    memcpy(&tdata_target_start, &tdata_source_start, (&tdata_target_end - &tdata_target_start));

    #ifdef __cplusplus
    // Call constructors
    std::for_each( __init_array_start,
                   __init_array_end, 
                   [](const function_t pf) {pf();});
    #endif
    
    // Jump to main
    int rc = main();
    // Don't expect to return, if so busy loop in the exit function.
    _Exit(rc);
}

// This should never be called. Busy loop with the CPU in idle state.
void _Exit(int exit_code) 
{ 
    // Halt
    while (1) {
        __asm__ volatile ("wfi");
    }
}
