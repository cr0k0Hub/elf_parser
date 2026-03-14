#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ELF_CLASS_32 1
#define ELF_CLASS_64 2

#define ELF_TYPE_SHIFT_32 16
#define ELF_MACHINE_SHIFT_32 18
#define ELF_VERSION_SHIFT_32 20
#define ELF_ENTRY_SHIFT_32 24
#define ELF_PHOFF_SHIFT_32 28
#define ELF_SHOFF_SHIFT_32 32
#define ELF_FLAGS_SHIFT_32 36
#define ELF_EHSIZE_SHIFT_32 40
#define ELF_PHENTSIZE_SHIFT_32 42
#define ELF_PHNUM_SHIFT_32 44
#define ELF_SHENTSIZE_SHIFT_32 46
#define ELF_SHNUM_SHIFT_32 48
#define ELF_SHSTRNDX_SHIFT_32 50

#define ELF_TYPE_SHIFT_64 16
#define ELF_MACHINE_SHIFT_64 18
#define ELF_VERSION_SHIFT_64 20
#define ELF_ENTRY_SHIFT_64 24
#define ELF_PHOFF_SHIFT_64 32
#define ELF_SHOFF_SHIFT_64 40
#define ELF_FLAGS_SHIFT_64 48
#define ELF_EHSIZE_SHIFT_64 52
#define ELF_PHENTSIZE_SHIFT_64 54
#define ELF_PHNUM_SHIFT_64 56
#define ELF_SHENTSIZE_SHIFT_64 58
#define ELF_SHNUM_SHIFT_64 60
#define ELF_SHSTRNDX_SHIFT_64 62

typedef struct __attribute__((packed)) ELF_32_HEADER {
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} ELF_32_HEADER;

typedef struct __attribute__((packed)) ELF_64_HEADER {
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint64_t entry;
    uint64_t phoff;
    uint64_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} ELF_64_HEADER;

typedef struct __attribute__((packed)) ELF_32_PHDR {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} ELF_32_PHDR;

typedef struct __attribute__((packed)) ELF_64_PHDR {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} ELF_64_PHDR;

typedef struct __attribute__((packed)) ELF_32_SHDR {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} ELF_32_SHDR;

typedef struct __attribute__((packed)) ELF_64_SHDR {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} ELF_64_SHDR;

ELF_32_HEADER elf_32_init(FILE *file_ptr) {
    ELF_32_HEADER elf = { 0 };

    fseek(file_ptr, 16, SEEK_SET);
    size_t bytes_read = fread(&elf, 1, sizeof(ELF_32_HEADER), file_ptr);
    if (bytes_read != 52) {
        perror("[-] (elf_32_init) failed to read bytes");
    }

    return elf;
}


ELF_64_HEADER elf_64_init(FILE *file_ptr) {
    ELF_64_HEADER elf = { 0 };

    fseek(file_ptr, 16, SEEK_SET);
    size_t bytes_read = fread(&elf, 1, sizeof(ELF_64_HEADER), file_ptr);
    if (bytes_read != sizeof(ELF_64_HEADER)) {
        perror("[-] (elf_64_init) failed to read bytes");
    }
    return elf;
}

ELF_32_PHDR get_32_prog_header(FILE *file_ptr) {
    ELF_32_PHDR phdr = { 0 };
    
    size_t bytes_read = fread(&phdr, 1, sizeof(ELF_32_PHDR), file_ptr);
    if (bytes_read != sizeof(ELF_32_PHDR)) {
        perror("[-] (get_32_prog_header) failed to read bytes");
    }

    return phdr;
}

ELF_64_PHDR get_64_prog_header(FILE *file_ptr) {
    ELF_64_PHDR phdr = { 0 };
    
    size_t bytes_read = fread(&phdr, 1, sizeof(ELF_64_PHDR), file_ptr);
    if (bytes_read != sizeof(ELF_64_PHDR)) {
        perror("[-] (get_64_prog_header) failed to read bytes");
    }

    return phdr;
}

ELF_32_SHDR get_32_sec_header(FILE *file_ptr) {
    ELF_32_SHDR shdr = { 0 };

    size_t bytes_read = fread(&shdr, 1, sizeof(ELF_32_SHDR), file_ptr);
    if (bytes_read != sizeof(ELF_32_SHDR)) {
        perror("[-] (get_32_sec_header) failed to read bytes");
    }

    return shdr;
}

ELF_64_SHDR get_64_sec_header(FILE *file_ptr) {
    ELF_64_SHDR shdr = { 0 };

    size_t bytes_read = fread(&shdr, 1, sizeof(ELF_64_SHDR), file_ptr);
    if (bytes_read != sizeof(ELF_64_SHDR)) {
        perror("[-] (get_64_sec_header) failed to read bytes");
    }

    return shdr;
}

void print_bytes(FILE *file_ptr, size_t n) {
    if (!file_ptr) {
        perror("[-] (print_bytes) wrong pointer");
        return;
    }

    unsigned char *buffer = (unsigned char *)malloc(n);
    if (!buffer) {
        perror("[-] (print_bytes) failed to allocate memory");
        return;
    }

    size_t bytes_read = fread(buffer, 1, n, file_ptr);
    if (bytes_read == 0 && n > 0) {
        perror("[-] (print_bytes) failed to read bytes from file");
        free(buffer);
        return;
    }

    for (size_t i = 0; i < bytes_read; i++) {
        printf("%02x ", buffer[i]);

        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("\n");
    free(buffer);
}
