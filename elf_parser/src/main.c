#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

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

ELF_64_HEADER elf_64_init(FILE *file_ptr) {
    ELF_64_HEADER elf = { 0 };

    fseek(file_ptr, ELF_TYPE_SHIFT_64, SEEK_SET);
    fread(&elf.type, 1, sizeof(elf.type), file_ptr);

    fseek(file_ptr, ELF_MACHINE_SHIFT_64, SEEK_SET);
    fread(&elf.machine, 1, sizeof(elf.machine), file_ptr);

    fseek(file_ptr, ELF_VERSION_SHIFT_64, SEEK_SET);
    fread(&elf.version, 1, sizeof(elf.version), file_ptr);

    fseek(file_ptr, ELF_ENTRY_SHIFT_64, SEEK_SET);
    fread(&elf.entry, 1, sizeof(elf.entry), file_ptr);

    fseek(file_ptr, ELF_PHOFF_SHIFT_64, SEEK_SET);
    fread(&elf.phoff, 1, sizeof(elf.phoff), file_ptr);

    fseek(file_ptr, ELF_SHOFF_SHIFT_64, SEEK_SET);
    fread(&elf.shoff, 1, sizeof(elf.shoff), file_ptr);

    fseek(file_ptr, ELF_FLAGS_SHIFT_64, SEEK_SET);
    fread(&elf.flags, 1, sizeof(elf.flags), file_ptr);

    fseek(file_ptr, ELF_EHSIZE_SHIFT_64, SEEK_SET);
    fread(&elf.ehsize, 1, sizeof(elf.ehsize), file_ptr);

    fseek(file_ptr, ELF_PHENTSIZE_SHIFT_64, SEEK_SET);
    fread(&elf.phentsize, 1, sizeof(elf.phentsize), file_ptr);

    fseek(file_ptr, ELF_PHNUM_SHIFT_64, SEEK_SET);
    fread(&elf.phnum, 1, sizeof(elf.phnum), file_ptr);

    fseek(file_ptr, ELF_SHENTSIZE_SHIFT_64, SEEK_SET);
    fread(&elf.shentsize, 1, sizeof(elf.shentsize), file_ptr);

    fseek(file_ptr, ELF_SHNUM_SHIFT_64, SEEK_SET);
    fread(&elf.shnum, 1, sizeof(elf.shnum), file_ptr);

    fseek(file_ptr, ELF_SHSTRNDX_SHIFT_64, SEEK_SET);
    fread(&elf.shstrndx, 1, sizeof(elf.shstrndx), file_ptr);

    return elf;
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
        perror("[-] failed to open file");
        return EXIT_FAILURE;
    }

    fseek(f, 0, SEEK_SET);
    unsigned char magic[4];
    size_t bytes_read = fread(magic, sizeof(*magic), 4, f);
    if (magic[0] == 0x7f && 
        magic[1] == 0x45 && 
        magic[2] == 0x4c && 
        magic[3] == 0x46) {
        printf("[+] it's an elf!\n");
    } else {
        printf("[-] it's not an elf\n");
        goto CLEANUP;
    }

    // fseek(f, 0, SEEK_SET);
    // print_bytes(f, 32);
    // fseek(f, 0, SEEK_SET);
    ELF_64_HEADER elf = elf_64_init(f);

    printf("TYPE:    0x%x\n", elf.type);
    printf("MACHINE: 0x%x\n", elf.machine);
    printf("VERSION: 0x%x\n", elf.version);
    printf("ENTRY:   0x%x\n", elf.entry);
    printf("SHNUM:   0x%x\n", elf.shnum);

    printf("\n");

    fseek(f, 0, SEEK_SET);

CLEANUP:
    fclose(f);
    
    return EXIT_SUCCESS;
}
