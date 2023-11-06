/*//////////////////////////////////////////////////////////////////
//   ,------.                    ,--.                ,--.         //
//   |  .--. ' ,---.  ,--,--.    |  |    ,---. ,---. `--' ,---.   //
//   |  '--'.'| .-. |' ,-.  |    |  |   | .-. | .-. |,--.| .--'   //
//   |  |\  \ ' '-' '\ '-'  |    |  '--.' '-' ' '-' ||  |\ `--.   //
//   `--' '--' `---'  `--`--'    `-----' `---' `-   /`--' `---'   //
/                                             `---'               //
//                                                                //
/      RV linker file                                             //
//	Linker file for the Roa Logic RVL SoC application             //
//                                                                //
////////////////////////////////////////////////////////////////////
//                                                                //
//     Copyright (C) 2016-2019 ROA Logic BV                       //
//     www.roalogic.com                                           //
//                                                                //
//     This source file may be used and distributed without       //
//   restrictions, provided that this copyright statement is      //
//   not removed from the file and that any derivative work       //
//   contains the original copyright notice and the associated    //
//   disclaimer.                                                  //
//                                                                //
//     This source file is free software; you can redistribute    //
//   it and/or modify it under the terms of the GNU General       //
//   Public License as published by the Free Software             //
//   Foundation, either version 3 of the License, or (at your     //
//   option) any later versions.                                  //
//   The current text of the License can be found at:             //
//   http://www.gnu.org/licenses/gpl.html                         //
//                                                                //
//     This source file is distributed in the hope that it will   //
//   be useful, but WITHOUT ANY WARRANTY; without even the        //
//   implied warranty of MERCHANTABILITY or FITTNESS FOR A        //
//   PARTICULAR PURPOSE. See the GNU General Public License for   //
//   more details.                                                //
//                                                                //
//////////////////////////////////////////////////////////////////*/

/* Specify memory areas */
MEMORY
{
	INT_RAM  (rwx)    : ORIGIN = 0x00110000, LENGTH = 4K
    ROM  (rwx)        : ORIGIN = 0x80000000, LENGTH = 64K
	RAM  (rwx)		  : ORIGIN = 0x80100000, LENGTH = 128K
}

OUTPUT_ARCH("riscv")

/* entry point */
ENTRY( _reset)

__heap_size = 0x200;

SECTIONS
{

    /* Default heap size (no heap) can be overriden at build-time by
     * adding the following to CFLAGS:
	 *
	 * -Xlinker --defsym=__heap_size=<address>
	 *
	 * Alternatively, heap can be defined to occupy all remaining RAM
     * by adding this to CFLAGS:
	 *
	 * -Xlinker --defsym=__heap_max=1
	 */
    __heap_size = DEFINED(__heap_size) ? __heap_size : 0x0;


	/*startup address for rvl*/
	. = 0x80000000;

	/*This section holds executable instructions that contribute to the 
	  process initialization code. That is, when a program starts to run 
	  the system arranges to execute the code in this section before the 
	  main program entry point (called main in C programs).*/
	.init ALIGN(4) : 
	{
		KEEP (*(.text.init.reset))
		KEEP (*(.text.trapHandler))
      	KEEP(*(SORT_NONE(.init)))
 	}	>ROM

  	/** This section holds executable instructions that contribute to the 
		process termination code. That is, when a program exits normally, 
		the system arranges to execute the code in this section. */
  	.fini           : 
	{ 
		KEEP(*(SORT_NONE(.fini))) 
	} >ROM

	/*--------------------------------------------------------------------*/
  	/* Global constructor/destructor segement                             */
  	/*--------------------------------------------------------------------*/
  	.preinit_array  : ALIGN(8)
  	{
    		PROVIDE_HIDDEN (__preinit_array_start = .);
    		KEEP (*(.preinit_array))
    		PROVIDE_HIDDEN (__preinit_array_end = .);
  	} >ROM

  	/** Initialisation */
  	.init_array     :
  	{
		PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
  	} >ROM

  	.fini_array     :
  	{
		PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
  	} >ROM

	.rodata : {
        *(.rdata)
        *(.rodata .rodata.*)
        *(.gnu.linkonce.r.*)
        . = ALIGN(8);
        *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata .srodata.*)
    } >ROM
	

	/* TEXT section */
	.text ALIGN(4) :
	{
	  *(.text )	/* text sections */
	  *(.text*)
	  *(.gnu.linkonce.t.*)
	  _etext = .;   /* global signal end of text */
	} >ROM		/* placement in ROM */

	.data : ALIGN(8) 
	{
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        PROVIDE( __global_pointer$ = . + 0x800 );
        *(.sdata .sdata.* .sdata2.*)
        *(.gnu.linkonce.s.*)
    } >RAM

	PROVIDE( data_source_start = LOADADDR(.data) );
    PROVIDE( data_target_start = ADDR(.data) );
    PROVIDE( data_target_end = ADDR(.data) + SIZEOF(.data) );

	.tdata : ALIGN(8) 
    { 
        *(.tdata .tdata.* .gnu.linkonce.td.*)
    } >RAM

	PROVIDE( __tdata_source = LOADADDR(.tdata) );
    PROVIDE( __tdata_size = SIZEOF(.tdata) );

	    /* Not really needed as our startup is never from ROM. */
    PROVIDE( tdata_source_start = LOADADDR(.tdata) );
    PROVIDE( tdata_target_start = ADDR(.tdata) );
    PROVIDE( tdata_target_end = ADDR(.tdata) + SIZEOF(.tdata) );

	.bss (NOLOAD): ALIGN(8) 
	{
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
    } >RAM

	PROVIDE( bss_source_start = LOADADDR(.bss) );
    PROVIDE( bss_target_start = ADDR(.bss) );
    PROVIDE( bss_target_end = ADDR(.bss) + SIZEOF(.bss) );

	.tbss : ALIGN(8) 
    {
        *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon)
    } > RAM
    PROVIDE( __tbss_size = SIZEOF(.tbss) );
    PROVIDE( __tls_size = __tls_end - __tls_base );

    .tbss_space : ALIGN(8) 
    {
        . = . + __tbss_size;
    } >RAM

	PROVIDE( tbss_source_start = LOADADDR(.tbss) );
    PROVIDE( tbss_target_start = ADDR(.tbss) );
    PROVIDE( tbss_target_end = ADDR(.tbss) + SIZEOF(.tbss) );

	_end = .;
	end = _end;
	_sp = .;
  	__stack = ORIGIN(RAM) + LENGTH(RAM);

	__freertos_irq_stack_top = ORIGIN(INT_RAM) + LENGTH (INT_RAM) - 4;
}
