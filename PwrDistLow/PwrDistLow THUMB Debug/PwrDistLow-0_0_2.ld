MEMORY
{
  UNPLACED_SECTIONS (wx) : ORIGIN = 0x100000000, LENGTH = 0
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 0x00080000
  CCMSRAM (wx) : ORIGIN = 0x10000000, LENGTH = 0x00004000
  SYSTEM_BANK1 (wx) : ORIGIN = 0x1fff0000, LENGTH = 0x00007000
  OTP (wx) : ORIGIN = 0x1fff7000, LENGTH = 0x00000400
  SYSTEM_BANK2 (wx) : ORIGIN = 0x1fff8000, LENGTH = 0x00007000
  OPTION_BANK1 (wx) : ORIGIN = 0x1fff7800, LENGTH = 0x00000030
  OPTION_BANK2 (wx) : ORIGIN = 0x1ffff800, LENGTH = 0x00000030
  RAM (wx) : ORIGIN = 0x20000000, LENGTH = 0x00018000
  SRAM1 (wx) : ORIGIN = 0x20000000, LENGTH = 0x00014000
  SRAM2 (wx) : ORIGIN = 0x20014000, LENGTH = 0x00004000
  ABP1 (wx) : ORIGIN = 0x40000000, LENGTH = 0x00009800
  ABP2 (wx) : ORIGIN = 0x40010000, LENGTH = 0x00006400
  AHB1 (wx) : ORIGIN = 0x40020000, LENGTH = 0x00004400
  AHB2 (wx) : ORIGIN = 0x48000000, LENGTH = 0x08060c00
  NOR_PSRAM1 (wx) : ORIGIN = 0x60000000, LENGTH = 0x00000000
  NOR_PSRAM2 (wx) : ORIGIN = 0x64000000, LENGTH = 0x00000000
  NOR_PSRAM3 (wx) : ORIGIN = 0x68000000, LENGTH = 0x00000000
  NOR_PSRAM4 (wx) : ORIGIN = 0x6c000000, LENGTH = 0x00000000
  NAND (rx) : ORIGIN = 0x80000000, LENGTH = 0x00000000
  QUADSPI_FLASH (rx) : ORIGIN = 0x90000000, LENGTH = 0x00000000
  FMC (wx) : ORIGIN = 0xa0000000, LENGTH = 0x00001000
  QUADSPI (wx) : ORIGIN = 0xa0001000, LENGTH = 0x00000400
  PPB (wx) : ORIGIN = 0xe0000000, LENGTH = 0x00100000
}

SECTIONS
{
  __FLASH_segment_start__ = 0x08000000;
  __FLASH_segment_end__ = 0x08080000;
  __FLASH_segment_size__ = 0x00080000;
  __CCMSRAM_segment_start__ = 0x10000000;
  __CCMSRAM_segment_end__ = 0x10004000;
  __CCMSRAM_segment_size__ = 0x00004000;
  __SYSTEM_BANK1_segment_start__ = 0x1fff0000;
  __SYSTEM_BANK1_segment_end__ = 0x1fff7000;
  __SYSTEM_BANK1_segment_size__ = 0x00007000;
  __OTP_segment_start__ = 0x1fff7000;
  __OTP_segment_end__ = 0x1fff7400;
  __OTP_segment_size__ = 0x00000400;
  __SYSTEM_BANK2_segment_start__ = 0x1fff8000;
  __SYSTEM_BANK2_segment_end__ = 0x1ffff000;
  __SYSTEM_BANK2_segment_size__ = 0x00007000;
  __OPTION_BANK1_segment_start__ = 0x1fff7800;
  __OPTION_BANK1_segment_end__ = 0x1fff7830;
  __OPTION_BANK1_segment_size__ = 0x00000030;
  __OPTION_BANK2_segment_start__ = 0x1ffff800;
  __OPTION_BANK2_segment_end__ = 0x1ffff830;
  __OPTION_BANK2_segment_size__ = 0x00000030;
  __RAM_segment_start__ = 0x20000000;
  __RAM_segment_end__ = 0x20018000;
  __RAM_segment_size__ = 0x00018000;
  __SRAM1_segment_start__ = 0x20000000;
  __SRAM1_segment_end__ = 0x20014000;
  __SRAM1_segment_size__ = 0x00014000;
  __SRAM2_segment_start__ = 0x20014000;
  __SRAM2_segment_end__ = 0x20018000;
  __SRAM2_segment_size__ = 0x00004000;
  __ABP1_segment_start__ = 0x40000000;
  __ABP1_segment_end__ = 0x40009800;
  __ABP1_segment_size__ = 0x00009800;
  __ABP2_segment_start__ = 0x40010000;
  __ABP2_segment_end__ = 0x40016400;
  __ABP2_segment_size__ = 0x00006400;
  __AHB1_segment_start__ = 0x40020000;
  __AHB1_segment_end__ = 0x40024400;
  __AHB1_segment_size__ = 0x00004400;
  __AHB2_segment_start__ = 0x48000000;
  __AHB2_segment_end__ = 0x50060c00;
  __AHB2_segment_size__ = 0x08060c00;
  __NOR_PSRAM1_segment_start__ = 0x60000000;
  __NOR_PSRAM1_segment_end__ = 0x60000000;
  __NOR_PSRAM1_segment_size__ = 0x00000000;
  __NOR_PSRAM2_segment_start__ = 0x64000000;
  __NOR_PSRAM2_segment_end__ = 0x64000000;
  __NOR_PSRAM2_segment_size__ = 0x00000000;
  __NOR_PSRAM3_segment_start__ = 0x68000000;
  __NOR_PSRAM3_segment_end__ = 0x68000000;
  __NOR_PSRAM3_segment_size__ = 0x00000000;
  __NOR_PSRAM4_segment_start__ = 0x6c000000;
  __NOR_PSRAM4_segment_end__ = 0x6c000000;
  __NOR_PSRAM4_segment_size__ = 0x00000000;
  __NAND_segment_start__ = 0x80000000;
  __NAND_segment_end__ = 0x80000000;
  __NAND_segment_size__ = 0x00000000;
  __QUADSPI_FLASH_segment_start__ = 0x90000000;
  __QUADSPI_FLASH_segment_end__ = 0x90000000;
  __QUADSPI_FLASH_segment_size__ = 0x00000000;
  __FMC_segment_start__ = 0xa0000000;
  __FMC_segment_end__ = 0xa0001000;
  __FMC_segment_size__ = 0x00001000;
  __QUADSPI_segment_start__ = 0xa0001000;
  __QUADSPI_segment_end__ = 0xa0001400;
  __QUADSPI_segment_size__ = 0x00000400;
  __PPB_segment_start__ = 0xe0000000;
  __PPB_segment_end__ = 0xe0100000;
  __PPB_segment_size__ = 0x00100000;

  __HEAPSIZE__ = 1024;
  __STACKSIZE_ABT__ = 0;
  __STACKSIZE_FIQ__ = 0;
  __STACKSIZE_IRQ__ = 0;
  __STACKSIZE_PROCESS__ = 0;
  __STACKSIZE_SVC__ = 0;
  __STACKSIZE_UND__ = 0;
  __STACKSIZE__ = 4096;
  __do_debug_operation = __do_debug_operation_mempoll;
  __vfprintf = __vfprintf_float_long_long;
  __vfscanf = __vfscanf_float_long_long_cc;

  __qspi_load_start__ = ALIGN(__QUADSPI_FLASH_segment_start__ , 4);
  .qspi ALIGN(__QUADSPI_FLASH_segment_start__ , 4) : AT(ALIGN(__QUADSPI_FLASH_segment_start__ , 4))
  {
    __qspi_start__ = .;
    *(.qspi .qspi.*)
  }
  __qspi_end__ = __qspi_start__ + SIZEOF(.qspi);
  __qspi_size__ = SIZEOF(.qspi);
  __qspi_load_end__ = __qspi_end__;

  __QUADSPI_FLASH_segment_used_end__ = ALIGN(__QUADSPI_FLASH_segment_start__ , 4) + SIZEOF(.qspi);
  __QUADSPI_FLASH_segment_used_size__ = __QUADSPI_FLASH_segment_used_end__ - __QUADSPI_FLASH_segment_start__;

  . = ASSERT(__qspi_start__ == __qspi_end__ || (__qspi_end__ - __QUADSPI_FLASH_segment_start__) <= __QUADSPI_FLASH_segment_size__ , "error: .qspi is too large to fit in QUADSPI_FLASH memory segment");

  __vectors_ram_load_start__ = ALIGN(__RAM_segment_start__ , 256);
  .vectors_ram ALIGN(__RAM_segment_start__ , 256) (NOLOAD) : AT(ALIGN(__RAM_segment_start__ , 256))
  {
    __vectors_ram_start__ = .;
    *(.vectors_ram .vectors_ram.*)
  }
  __vectors_ram_end__ = __vectors_ram_start__ + SIZEOF(.vectors_ram);
  __vectors_ram_size__ = SIZEOF(.vectors_ram);
  __vectors_ram_load_end__ = __vectors_ram_end__;

  . = ASSERT(__vectors_ram_start__ == __vectors_ram_end__ || (__vectors_ram_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .vectors_ram is too large to fit in RAM memory segment");

  __vectors_load_start__ = ALIGN(__FLASH_segment_start__ , 256);
  .vectors ALIGN(__FLASH_segment_start__ , 256) : AT(ALIGN(__FLASH_segment_start__ , 256))
  {
    __vectors_start__ = .;
    *(.vectors .vectors.*)
  }
  __vectors_end__ = __vectors_start__ + SIZEOF(.vectors);
  __vectors_size__ = SIZEOF(.vectors);
  __vectors_load_end__ = __vectors_end__;

  . = ASSERT(__vectors_start__ == __vectors_end__ || (__vectors_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .vectors is too large to fit in FLASH memory segment");

  __init_load_start__ = ALIGN(__vectors_end__ , 4);
  .init ALIGN(__vectors_end__ , 4) : AT(ALIGN(__vectors_end__ , 4))
  {
    __init_start__ = .;
    *(.init .init.*)
  }
  __init_end__ = __init_start__ + SIZEOF(.init);
  __init_size__ = SIZEOF(.init);
  __init_load_end__ = __init_end__;

  . = ASSERT(__init_start__ == __init_end__ || (__init_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .init is too large to fit in FLASH memory segment");

  __init_rodata_load_start__ = ALIGN(__init_end__ , 4);
  .init_rodata ALIGN(__init_end__ , 4) : AT(ALIGN(__init_end__ , 4))
  {
    __init_rodata_start__ = .;
    *(.init_rodata .init_rodata.*)
  }
  __init_rodata_end__ = __init_rodata_start__ + SIZEOF(.init_rodata);
  __init_rodata_size__ = SIZEOF(.init_rodata);
  __init_rodata_load_end__ = __init_rodata_end__;

  . = ASSERT(__init_rodata_start__ == __init_rodata_end__ || (__init_rodata_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .init_rodata is too large to fit in FLASH memory segment");

  __text_load_start__ = ALIGN(__init_rodata_end__ , 4);
  .text ALIGN(__init_rodata_end__ , 4) : AT(ALIGN(__init_rodata_end__ , 4))
  {
    __text_start__ = .;
    *(.text .text.* .gnu.linkonce.t.* .glue_7t .glue_7 .gcc_except_table .ARM.extab* .gnu.linkonce.armextab.*)
  }
  __text_end__ = __text_start__ + SIZEOF(.text);
  __text_size__ = SIZEOF(.text);
  __text_load_end__ = __text_end__;

  . = ASSERT(__text_start__ == __text_end__ || (__text_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .text is too large to fit in FLASH memory segment");

  __dtors_load_start__ = ALIGN(__text_end__ , 4);
  .dtors ALIGN(__text_end__ , 4) : AT(ALIGN(__text_end__ , 4))
  {
    __dtors_start__ = .;
    KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors)) KEEP (*(.fini_array)) KEEP (*(SORT(.fini_array.*)))
  }
  __dtors_end__ = __dtors_start__ + SIZEOF(.dtors);
  __dtors_size__ = SIZEOF(.dtors);
  __dtors_load_end__ = __dtors_end__;

  . = ASSERT(__dtors_start__ == __dtors_end__ || (__dtors_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .dtors is too large to fit in FLASH memory segment");

  __ctors_load_start__ = ALIGN(__dtors_end__ , 4);
  .ctors ALIGN(__dtors_end__ , 4) : AT(ALIGN(__dtors_end__ , 4))
  {
    __ctors_start__ = .;
    KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors)) KEEP (*(.init_array)) KEEP (*(SORT(.init_array.*)))
  }
  __ctors_end__ = __ctors_start__ + SIZEOF(.ctors);
  __ctors_size__ = SIZEOF(.ctors);
  __ctors_load_end__ = __ctors_end__;

  . = ASSERT(__ctors_start__ == __ctors_end__ || (__ctors_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .ctors is too large to fit in FLASH memory segment");

  __rodata_load_start__ = ALIGN(__ctors_end__ , 4);
  .rodata ALIGN(__ctors_end__ , 4) : AT(ALIGN(__ctors_end__ , 4))
  {
    __rodata_start__ = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  __rodata_end__ = __rodata_start__ + SIZEOF(.rodata);
  __rodata_size__ = SIZEOF(.rodata);
  __rodata_load_end__ = __rodata_end__;

  . = ASSERT(__rodata_start__ == __rodata_end__ || (__rodata_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .rodata is too large to fit in FLASH memory segment");

  __ARM.exidx_load_start__ = ALIGN(__rodata_end__ , 4);
  .ARM.exidx ALIGN(__rodata_end__ , 4) : AT(ALIGN(__rodata_end__ , 4))
  {
    __ARM.exidx_start__ = .;
    __exidx_start =   __ARM.exidx_start__;
    *(.ARM.exidx .ARM.exidx.*)
  }
  __ARM.exidx_end__ = __ARM.exidx_start__ + SIZEOF(.ARM.exidx);
  __ARM.exidx_size__ = SIZEOF(.ARM.exidx);
  __exidx_end =   __ARM.exidx_end__;
  __ARM.exidx_load_end__ = __ARM.exidx_end__;

  . = ASSERT(__ARM.exidx_start__ == __ARM.exidx_end__ || (__ARM.exidx_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .ARM.exidx is too large to fit in FLASH memory segment");

  __fast_load_start__ = ALIGN(__ARM.exidx_end__ , 4);
  .fast ALIGN(__vectors_ram_end__ , 4) : AT(ALIGN(__ARM.exidx_end__ , 4))
  {
    __fast_start__ = .;
    *(.fast .fast.*)
  }
  __fast_end__ = __fast_start__ + SIZEOF(.fast);
  __fast_size__ = SIZEOF(.fast);
  __fast_load_end__ = __fast_load_start__ + SIZEOF(.fast);

  . = ASSERT(__fast_load_start__ == __fast_load_end__ || (__fast_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .fast is too large to fit in FLASH memory segment");

  .fast_run ALIGN(__vectors_ram_end__ , 4) (NOLOAD) :
  {
    __fast_run_start__ = .;
  }
  __fast_run_end__ = __fast_run_start__ + SIZEOF(.fast);
  __fast_run_size__ = __fast_run_end__ - __fast_run_start__;
  __fast_run_load_end__ = __fast_run_end__;

  . = ASSERT(__fast_run_start__ == __fast_run_end__ || (__fast_run_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .fast_run is too large to fit in RAM memory segment");

  __data_load_start__ = ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4);
  .data ALIGN(__fast_run_end__ , 4) : AT(ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4))
  {
    __data_start__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
  }
  __data_end__ = __data_start__ + SIZEOF(.data);
  __data_size__ = SIZEOF(.data);
  __data_load_end__ = __data_load_start__ + SIZEOF(.data);

  . = ASSERT(__data_load_start__ == __data_load_end__ || (__data_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .data is too large to fit in FLASH memory segment");

  .data_run ALIGN(__fast_run_end__ , 4) (NOLOAD) :
  {
    __data_run_start__ = .;
  }
  __data_run_end__ = __data_run_start__ + SIZEOF(.data);
  __data_run_size__ = __data_run_end__ - __data_run_start__;
  __data_run_load_end__ = __data_run_end__;

  . = ASSERT(__data_run_start__ == __data_run_end__ || (__data_run_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .data_run is too large to fit in RAM memory segment");

  __bss_load_start__ = ALIGN(__data_run_end__ , 4);
  .bss ALIGN(__data_run_end__ , 4) (NOLOAD) : AT(ALIGN(__data_run_end__ , 4))
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __bss_end__ = __bss_start__ + SIZEOF(.bss);
  __bss_size__ = SIZEOF(.bss);
  __bss_load_end__ = __bss_end__;

  . = ASSERT(__bss_start__ == __bss_end__ || (__bss_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .bss is too large to fit in RAM memory segment");

  __tbss_load_start__ = ALIGN(__bss_end__ , 4);
  .tbss ALIGN(__bss_end__ , 4) (NOLOAD) : AT(ALIGN(__bss_end__ , 4))
  {
    __tbss_start__ = .;
    *(.tbss .tbss.*)
  }
  __tbss_end__ = __tbss_start__ + SIZEOF(.tbss);
  __tbss_size__ = SIZEOF(.tbss);
  __tbss_load_end__ = __tbss_end__;

  . = ASSERT(__tbss_start__ == __tbss_end__ || (__tbss_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .tbss is too large to fit in RAM memory segment");

  __tdata_load_start__ = ALIGN(__data_load_start__ + SIZEOF(.data) , 4);
  .tdata ALIGN(__tbss_end__ , 4) : AT(ALIGN(__data_load_start__ + SIZEOF(.data) , 4))
  {
    __tdata_start__ = .;
    *(.tdata .tdata.*)
  }
  __tdata_end__ = __tdata_start__ + SIZEOF(.tdata);
  __tdata_size__ = SIZEOF(.tdata);
  __tdata_load_end__ = __tdata_load_start__ + SIZEOF(.tdata);

  __FLASH_segment_used_end__ = ALIGN(__data_load_start__ + SIZEOF(.data) , 4) + SIZEOF(.tdata);
  __FLASH_segment_used_size__ = __FLASH_segment_used_end__ - __FLASH_segment_start__;

  . = ASSERT(__tdata_load_start__ == __tdata_load_end__ || (__tdata_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .tdata is too large to fit in FLASH memory segment");

  .tdata_run ALIGN(__tbss_end__ , 4) (NOLOAD) :
  {
    __tdata_run_start__ = .;
  }
  __tdata_run_end__ = __tdata_run_start__ + SIZEOF(.tdata);
  __tdata_run_size__ = __tdata_run_end__ - __tdata_run_start__;
  __tdata_run_load_end__ = __tdata_run_end__;

  . = ASSERT(__tdata_run_start__ == __tdata_run_end__ || (__tdata_run_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .tdata_run is too large to fit in RAM memory segment");

  __non_init_load_start__ = ALIGN(__tdata_run_end__ , 4);
  .non_init ALIGN(__tdata_run_end__ , 4) (NOLOAD) : AT(ALIGN(__tdata_run_end__ , 4))
  {
    __non_init_start__ = .;
    *(.non_init .non_init.*)
  }
  __non_init_end__ = __non_init_start__ + SIZEOF(.non_init);
  __non_init_size__ = SIZEOF(.non_init);
  __non_init_load_end__ = __non_init_end__;

  . = ASSERT(__non_init_start__ == __non_init_end__ || (__non_init_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .non_init is too large to fit in RAM memory segment");

  __heap_load_start__ = ALIGN(__non_init_end__ , 8);
  .heap ALIGN(__non_init_end__ , 8) (NOLOAD) : AT(ALIGN(__non_init_end__ , 8))
  {
    __heap_start__ = .;
    *(.heap .heap.*)
    . = ALIGN(MAX(__heap_start__ + __HEAPSIZE__ , .), 8);
  }
  __heap_end__ = __heap_start__ + SIZEOF(.heap);
  __heap_size__ = SIZEOF(.heap);
  __heap_load_end__ = __heap_end__;

  . = ASSERT(__heap_start__ == __heap_end__ || (__heap_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .heap is too large to fit in RAM memory segment");

  . = ASSERT(__HEAPSIZE__  == ALIGN(__HEAPSIZE__, 8), "error: .heap section size does not match alignment");

  . = ASSERT(__heap_size__ <= __HEAPSIZE__ , "error: .heap section is larger than specified size");

  __stack_load_start__ = ALIGN(__heap_end__ , 8);
  .stack ALIGN(__heap_end__ , 8) (NOLOAD) : AT(ALIGN(__heap_end__ , 8))
  {
    __stack_start__ = .;
    *(.stack .stack.*)
    . = ALIGN(MAX(__stack_start__ + __STACKSIZE__ , .), 8);
  }
  __stack_end__ = __stack_start__ + SIZEOF(.stack);
  __stack_size__ = SIZEOF(.stack);
  __stack_load_end__ = __stack_end__;

  . = ASSERT(__stack_start__ == __stack_end__ || (__stack_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .stack is too large to fit in RAM memory segment");

  . = ASSERT(__STACKSIZE__  == ALIGN(__STACKSIZE__, 8), "error: .stack section size does not match alignment");

  . = ASSERT(__stack_size__ <= __STACKSIZE__ , "error: .stack section is larger than specified size");

  __stack_process_load_start__ = ALIGN(__stack_end__ , 8);
  .stack_process ALIGN(__stack_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_end__ , 8))
  {
    __stack_process_start__ = .;
    *(.stack_process .stack_process.*)
    . = ALIGN(MAX(__stack_process_start__ + __STACKSIZE_PROCESS__ , .), 8);
  }
  __stack_process_end__ = __stack_process_start__ + SIZEOF(.stack_process);
  __stack_process_size__ = SIZEOF(.stack_process);
  __stack_process_load_end__ = __stack_process_end__;

  __RAM_segment_used_end__ = ALIGN(__stack_end__ , 8) + SIZEOF(.stack_process);
  __RAM_segment_used_size__ = __RAM_segment_used_end__ - __RAM_segment_start__;

  . = ASSERT(__stack_process_start__ == __stack_process_end__ || (__stack_process_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .stack_process is too large to fit in RAM memory segment");

  . = ASSERT(__STACKSIZE_PROCESS__  == ALIGN(__STACKSIZE_PROCESS__, 8), "error: .stack_process section size does not match alignment");

  . = ASSERT(__stack_process_size__ <= __STACKSIZE_PROCESS__ , "error: .stack_process section is larger than specified size");

}

