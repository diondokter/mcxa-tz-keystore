MEMORY
{
    FLASH : ORIGIN = 0x000F8000, LENGTH = 32K
    RAM : ORIGIN = 0x2001E000, LENGTH = 8K
}

/* # Sections */
SECTIONS
{
  PROVIDE(_ram_start = ORIGIN(RAM));
  PROVIDE(_ram_end = ORIGIN(RAM) + LENGTH(RAM));

  /* The alignment of .text is the maximum alignment required by its input sections, which
     can be more than 4 (e.g. `.p2align` directives inside inline assembly). Since the
     start address of .text is set explicitly, the linker won't align it automatically, so do it
     here. */
  PROVIDE(_stext = ALIGN(ORIGIN(FLASH), ALIGNOF(.text)));

  /* ### .text */
  .text _stext :
  {
    __stext = .;
    *(.Reset);

    *(.text .text.*);

    . = ALIGN(4); /* Pad .text to the alignment to workaround overlapping load section bug in old lld */
    __etext = .;
  } > FLASH

  /* ### .rodata */
  .rodata : ALIGN(4)
  {
    . = ALIGN(4);
    __srodata = .;
    *(.rodata .rodata.*);

    /* 4-byte align the end (VMA) of this section.
       This is required by LLD to ensure the LMA of the following .data
       section will have the correct alignment. */
    . = ALIGN(4);
    __erodata = .;
  } > FLASH

  /* ## Sections in RAM */
  /* ### .data */
  .data : ALIGN(4)
  {
    . = ALIGN(4);
    __sdata = .;
    *(.data .data.*);
    . = ALIGN(4); /* 4-byte align the end (VMA) of this section */
  } > RAM AT>FLASH
  /* Allow sections from user `memory.x` injected using `INSERT AFTER .data` to
   * use the .data loading mechanism by pushing __edata. Note: do not change
   * output region or load region in those user sections! */
  . = ALIGN(4);
  __edata = .;

  /* LMA of .data */
  __sidata = LOADADDR(.data);

  /* ### .gnu.sgstubs
     This section contains the TrustZone-M veneers put there by the Arm GNU linker. */
  /* Security Attribution Unit blocks must be 32 bytes aligned. */
  /* Note that this pads the FLASH usage to 32 byte alignment. */
  .gnu.sgstubs : ALIGN(32)
  {
    . = ALIGN(32);
    __veneer_base = .;
    *(.gnu.sgstubs*)
    . = ALIGN(32);
  } > FLASH
  /* Place `__veneer_limit` outside the `.gnu.sgstubs` section because veneers are
   * always inserted last in the section, which would otherwise be _after_ the `__veneer_limit` symbol.
   */
  . = ALIGN(32);
  __veneer_limit = .;

  /* ### .bss */
  .bss (NOLOAD) : ALIGN(4)
  {
    . = ALIGN(4);
    __sbss = .;
    *(.bss .bss.*);
    *(COMMON); /* Uninitialized C statics */
    . = ALIGN(4); /* 4-byte align the end (VMA) of this section */
  } > RAM
  /* Allow sections from user `memory.x` injected using `INSERT AFTER .bss` to
   * use the .bss zeroing mechanism by pushing __ebss. Note: do not change
   * output region or load region in those user sections! */
  . = ALIGN(4);
  __ebss = .;

  /* ### .uninit */
  .uninit (NOLOAD) : ALIGN(4)
  {
    . = ALIGN(4);
    __suninit = .;
    *(.uninit .uninit.*);
    . = ALIGN(4);
    __euninit = .;
  } > RAM

  /* Align `__sheap` and `_stack_end` pointers to 4 bytes */
  . = ALIGN(4);

  /* ## .got */
  /* Dynamic relocations are unsupported. This section is only used to detect relocatable code in
     the input files and raise an error if relocatable code is found */
  .got (NOLOAD) :
  {
    KEEP(*(.got .got.*));
  }

  /* ## Discarded sections */
  /DISCARD/ :
  {
    /* Unused exception related info that only wastes space */
    *(.ARM.exidx);
    *(.ARM.exidx.*);
    *(.ARM.extab.*);
  }
}

/* Do not exceed this mark in the error messages below                                    | */
/* # Alignment checks */
ASSERT(ORIGIN(FLASH) % 4 == 0, "
ERROR(cortex-m-rt): the start of the FLASH region must be 4-byte aligned");

ASSERT(ORIGIN(RAM) % 4 == 0, "
ERROR(cortex-m-rt): the start of the RAM region must be 4-byte aligned");

/* # Other checks */
ASSERT(SIZEOF(.got) == 0, "
ERROR(cortex-m-rt): .got section detected in the input object files
Dynamic relocations are not supported. If you are linking to C code compiled using
the 'cc' crate then modify your build script to compile the C code _without_
the -fPIC flag. See the documentation of the `cc::Build.pic` method for details.");

ASSERT(SIZEOF(.data) == 0, ".data symbols not supported");
ASSERT(SIZEOF(.bss) == 0, ".bss symbols not supported");
/* Do not exceed this mark in the error messages above                                    | */
