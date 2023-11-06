#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

#include "HAL_Timer.h"
#include "Interrupt.h"

#if( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 )

extern uint32_t const ullMachineTimerCompareRegisterBase;
extern volatile uint64_t * pullMachineTimerCompareRegister;

extern void freertos_risc_v_trap_handler();

/**
 * @brief Setup timer interrupt
 * We are overriding function defined in port.c, in port.c it is defined as weak
 * Only reason to override is that we need to write to prescaler to enable timer and we do that here
 * everything else is same
 *
 */
void vPortSetupTimerInterrupt( void )
{
    volatile uint32_t * const timerInterruptEnable = ( volatile uint32_t * const ) ( AHB3_LITE_TIMER_BASE_ADDRESS + 0xC );
    volatile uint32_t ulHartId;

    /**
     * Writing Pre-Scaler with 0 means we will use prescaller of 1
     * even though 0 is default value, we need to write it to start the timer
     *
     */
    __asm volatile( "csrr %0, mhartid" : "=r"( ulHartId ) );
    __asm volatile( "csrw mtvec, %0" : :"rK"(freertos_risc_v_trap_handler):);
    pullMachineTimerCompareRegister  = ( volatile uint64_t * ) ( ullMachineTimerCompareRegisterBase + ( ulHartId * sizeof( uint64_t ) ) );

    *timerInterruptEnable = 0x1;
}

#else

extern void freertos_risc_v_trap_handler();
extern void enableGlobalInterrupt();
extern void enableTimerInterrupt();

void timerTickInterrupt(void)
{
    /* vPortSetupTimerInterrupt() uses LPIT0 to generate the tick interrupt. */
    if( xTaskIncrementTick() != 0 )
    {
        vTaskSwitchContext();
    }

    HAL_TIMER_setTimeCmpCount(TIMERBASE, 1, 0);
}

void vPortSetupTimerInterrupt( void )
{
    __asm volatile( "csrw mtvec, %0" : :"rK"(freertos_risc_v_trap_handler):);

    HAL_TIMER_configureTickFrequency(TIMERBASE, 1000);
    HAL_TIMER_setTimeCmpCount(TIMERBASE, 1, 0);
    HAL_TIMER_enableInterrupt(TIMERBASE, 0);

    HAL_TIMER_setCallbackFunction(timerTickInterrupt, 0);

	enableGlobalInterrupt();	
    enableTimerInterrupt();
}

void freertos_risc_v_application_interrupt_handler(uint32_t mcause)
{
    interruptHandler(mcause);
}

#endif

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    __asm volatile( "ebreak" );
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    __asm volatile( "ebreak" );
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{

}
/*-----------------------------------------------------------*/

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static -- otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static -- otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
