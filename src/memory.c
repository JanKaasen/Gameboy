#include "../include/memory.h"
#include "../include/common.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

bool memory_init(memory_system_t *mem_sys)
{
	if (mem_sys == NULL) {
		printf("Cannot initialize NULL memory system"); 
		return false;
	}

	memset(mem_sys->rom, 0x00, ROM_SIZE);
	memset(mem_sys->vram, 0x00, ROM_SIZE);
	memset(mem_sys->wram, 0x00, ROM_SIZE);

	mem_sys->rom_loaded = false;

	return true;
}

static int memory_calculate_offset(address addr)
{
	if (addr > 0xFFFF) {
		return -1;
	}

	if (addr >= ROM_START && addr <= ROM_END) {
		return addr - ROM_START;
	}

	if (addr >= VRAM_START && addr <= VRAM_END) {
		return addr - VRAM_START;
	}

	if (addr >= WRAM_START && addr <= WRAM_END) {
		return addr - WRAM_START;
	}

	
	return -1;
}

byte memory_read_byte(memory_system_t *mem_sys, address addr)
{
	assert(mem_sys != NULL);

	int offset = memory_calculate_offset(addr);
	if (offset < 0) {
		return 0xFF;
	}
	
	byte result;
	if (addr >= ROM_START && addr <= ROM_END) {
		result = mem_sys->rom[offset];
	}
	else if (addr >= VRAM_START && addr <= VRAM_END) {
		result = mem_sys->vram[offset];
	}
	else if (addr >= WRAM_START && addr <= WRAM_END) {
		result = mem_sys->wram[offset];
	}
	
	return result;
}

void memory_write_byte(memory_system_t *mem_sys, address addr, byte value)
{
	assert(mem_sys != NULL);

	int offset = memory_calculate_offset(addr);
	if (offset < 0) {
		return;
	}

	if (addr >= ROM_START && addr <= ROM_END) {
		printf("ERROR: tried writing to ROM address");
		return; 
	}

	if (addr >= VRAM_START && addr <= VRAM_END) {
		mem_sys->vram[offset] = value;
	}

	else if (addr >= WRAM_START && addr <= WRAM_END) {
		mem_sys->wram[offset] = value;
	}
}

bool memory_is_valid_address(address addr)
{
	if (addr > 0xFFFF) {
		return false;
	}
	
	if (addr >= ROM_START && addr <= ROM_END) {
		return true;
	}
	
	if (addr >= VRAM_START && addr <= VRAM_END) {
		return true;
	}
	
	if (addr >= WRAM_START && addr <= WRAM_END) {
		return true;
	}

	return false;
}

const char *memory_get_region_name(address addr)
{
    if (addr >= ROM_START && addr <= ROM_END) {
        return "ROM";
    }
    
    if (addr >= VRAM_START && addr <= VRAM_END) {
        return "VRAM";
    }
    
    if (addr >= WRAM_START && addr <= WRAM_END) {
        return "WRAM";
    }
    
    if (addr > 0xFFFF) {
        return "Invalid";
    }
    
    return "Unmapped";
}
