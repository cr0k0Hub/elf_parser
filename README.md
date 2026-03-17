# Simple ELF Parser

A lightweight ELF (Executable and Linkable Format) parser written in C. It detects whether a file is an ELF, reads its header (32/64-bit), and displays key information such as entry point, section headers, program headers, and optionally dumps the .text section.

This tool was created as a learning exercise to understand the internal structure of ELF binaries and to practice low-level file parsing in C.

## Features

  - Validates ELF magic bytes (0x7F ELF).

  - Supports both 32-bit and 64-bit ELF files.

  - Displays essential ELF header fields:
    - Type (e.g., EXEC, DYN)
    - Machine architecture
    - Version
    - Entry point address
    - Number of section headers

    For 64-bit ELF:

      - Lists program headers (segments) with type and virtual address.

      - Lists all section headers with names, types, and indexes.

      - Prints a hex dump of the .text section (if present).

## Building

Make sure you have gcc and the standard C library installed.

```bash
git clone https://github.com/cr0k0Hub/elf_parser.git
cd elf_parser
cd src
gcc  main.c -o elf_parser
```

### Usage

```bash
./elf_parser <path-to-elf-file>
```
### Example

Parsing a simple 64-bit executable:
```bash
$ ./elf_parser /bin/ls
[+] it's an elf!
==========[ INFO ]==========
  TYPE:    0x2
  MACHINE: 0x3e
  VERSION: 0x1
  ENTRY:   0x4048c0
  SHNUM:   0x1d
  SHSTRND: 0x1c
============================
  SEGMENT: 0
  TYPE:    0x1
  VADDR:   0x400000
============================
...
========[ SECTIONS ]========
	.text
<hex dump of .text section>
	##########################
  INDEX: 0
  NAME:  
  TYPE:  0x0
============================
  INDEX: 1
  NAME:  .interp
  TYPE:  0x1
============================
...
```
## Limitations

  - Only basic error checking is performed (e.g., missing file, read errors).

  - The parser assumes the ELF is well-formed; malformed files may cause undefined behavior.

  - Section name string table is assumed to be present (for 64-bit).

  - The .text dump is printed unconditionally if the section is found.

## Dependencies

  - Standard C library (stdio.h, stdlib.h, string.h, etc.)

  - Custom utils.h (provided separately) – defines ELF structures and helper functions.
