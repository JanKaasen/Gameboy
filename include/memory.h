#ifndef MEMORY_H

#define MEMORY_H

#include "common.h"
#include <stdbool.h>
#include <stdint.h>

#define ROM_START 0x0000
#define ROM_END 0x7FFF
#define VRAM_START 0x8000
#define VRAM_END 0x9FFF
#define WRAM_START 0xC000
#define WRAM_END 0xDFFF
#define IO_REGISTERS_START 0xFF00
#define IO_REGISTERS_END 0xFF7F
#define HRAM_START 0xFF80
#define HRAM_END 0xFFFE

#define ROM_SIZE (ROM_END - ROM_START + 1)
#define VRAM_SIZE (VRAM_END - VRAM_START + 1)
#define WRAM_SIZE (WRAM_END - WRAM_START + 1)
#define IO_REGISTERS_SIZE (IO_REGISTERS_END - IO_REGISTERS_START + 1)
#define HRAM_SIZE (HRAM_END - HRAM_START + 1)

typedef struct memory_system memory_system_t;

struct memory_system {
	byte rom[ROM_SIZE];
	byte vram[VRAM_SIZE];
	byte wram[WRAM_SIZE];
	bool rom_loaded;
};

bool memory_init(memory_system_t *mem_sys);
void memory_cleanup(memory_system_t *mem_sys);

byte memory_read_byte(memory_system_t *mem_sys, address addr);
void memory_write_byte(memory_system_t *mem_sys, address addr, byte value);

word memory_read_word(memory_system_t *mem_sys, address addr);
void memory_write_word(memory_system_t *mem_sys, address addr, word value);

bool memory_is_valid_address(address addr);
const char *memory_get_region_name(address addr);
void memory_dump_region(memory_system_t *mem_sys, address start, address end);

bool memory_load_rom(memory_system_t *mem_sys, const char *filename);

#endif
