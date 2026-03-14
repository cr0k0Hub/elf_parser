#include "utils.h"
#include <string.h>

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
    if (bytes_read < 4) {
        perror("[-] failed to read bytes from file");
        goto CLEANUP;
    }

    if (magic[0] == 0x7f && 
        magic[1] == 0x45 && 
        magic[2] == 0x4c && 
        magic[3] == 0x46) {
        printf("[+] it's an elf!\n");
    } else {
        printf("[-] it's not an elf\n");
        goto CLEANUP;
    }

    fseek(f, 4, SEEK_SET);
    if (fgetc(f) == ELF_CLASS_32) {
        ELF_32_HEADER elf = elf_32_init(f);
        
        printf("==========[ INFO ]==========\n");
        printf("  TYPE:    0x%x\n", elf.type);
        printf("  MACHINE: 0x%x\n", elf.machine);
        printf("  VERSION: 0x%x\n", elf.version);
        printf("  ENTRY:   0x%x\n", elf.entry);
        printf("  SHNUM:   0x%x\n", elf.shnum);
        printf("============================\n");
    } else {
        ELF_64_HEADER elf = elf_64_init(f);

        printf("==========[ INFO ]==========\n");
        printf("  TYPE:    0x%x\n", elf.type);
        printf("  MACHINE: 0x%x\n", elf.machine);
        printf("  VERSION: 0x%x\n", elf.version);
        printf("  ENTRY:   0x%x\n", elf.entry);
        printf("  SHNUM:   0x%x\n", elf.shnum);
        printf("  SHSTRND: 0x%x\n", elf.shstrndx);
        printf("============================\n");

        if (elf.phnum > 0) {
            fseek(f, elf.phoff, SEEK_SET);
            for (size_t i = 0; i < elf.phnum; i++) {
                ELF_64_PHDR phdr = get_64_prog_header(f);
                printf("  SEGMENT: %ld\n", i);
                printf("  TYPE:    0x%x\n", phdr.p_type);
                printf("  VADDR:   0x%lx\n", phdr.p_vaddr);
                // print_bytes(f, elf.phentsize);
                printf("============================\n");
            }

        }   

        if (elf.shnum > 0) {
            ELF_64_SHDR *shdrs = malloc(elf.shnum * sizeof(ELF_64_SHDR));
            if (!shdrs) {
                perror("[-] (main) failed to allocate memory");
                goto CLEANUP;
            }
            
            fseek(f, elf.shoff, SEEK_SET);
            printf("========[ SECTIONS ]========\n");
            for (size_t i = 0; i < elf.shnum; i++) {
                ELF_64_SHDR sh = get_64_sec_header(f);
                shdrs[i] = sh;
            }

            ELF_64_SHDR *strtab_hdr = &shdrs[elf.shstrndx];
            fseek(f, strtab_hdr->sh_offset, SEEK_SET);

            char *shstrtab = malloc(strtab_hdr->sh_size);
            fread(shstrtab, 1, strtab_hdr->sh_size, f);
            for (size_t i = 0; i < elf.shnum; i++) {
                const char *name = (i == elf.shstrndx) ? ".shstrtab" : shstrtab + shdrs[i].sh_name;
                printf("  INDEX: %d\n", i);
                printf("  NAME:  %s\n", name);
                printf("  TYPE:  0x%x\n", shdrs[i].sh_type);
                printf("============================\n");
            }

            free(shstrtab);
            free(shdrs);
        }
    }

    printf("\n");

    fseek(f, 0, SEEK_SET);

CLEANUP:
    fclose(f);
 
    return EXIT_SUCCESS;
}
