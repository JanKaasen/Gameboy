#include "../include/memory.h"
#include "../include/common.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

bool memory_init(memory_system_t *mem_sys)
{
	if (mem_sys == NULL) {
		printf("Cannot initialize NULL memory system"); 
		return false;
	}

	memset(mem_sys->rom, 0x00, ROM_SIZE);
	memset(mem_sys->vram, 0x00, VRAM_SIZE);
	memset(mem_sys->wram, 0x00, WRAM_SIZE);

	mem_sys->rom_loaded = false;

	return true;
}

void memory_cleanup(memory_system_t *mem_sys)
{
	if (mem_sys == NULL) {
		printf("ERROR: CANNOT CLEANUP NULL MEMORY SYSTEM"); 
		return;
	}

	memset(mem_sys->rom, 0x00, ROM_SIZE);
	memset(mem_sys->vram, 0x00, VRAM_SIZE);
	memset(mem_sys->wram, 0x00, WRAM_SIZE);

	mem_sys->rom_loaded = false;
	printf("Memory cleaned up successfully\n");
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

word memory_read_word(memory_system_t *mem_sys, address addr)
{
	byte low_byte = memory_read_byte(mem_sys, addr);
	byte high_byte = memory_read_byte(mem_sys, addr + 1);

	return low_byte | (high_byte << 8);
}

void memory_write_word(memory_system_t *mem_sys, address addr, word value)
{
	byte low_byte = value & 0xFF;
	byte high_byte = (value >> 8) & 0xFF;

	memory_write_byte(mem_sys, addr, low_byte);
	memory_write_byte(mem_sys, addr + 1,  high_byte);
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

void memory_dump_region(memory_system_t *mem_sys, address start, address end)
{
	if (mem_sys == NULL) {
    	    printf("Error: Cannot dump memory with NULL memory system\n");
    	    return;
    	}
    	
    	if (start > end) {
    	    printf("Error: Start address (0x%04X) > end address (0x%04X)\n", start, end);
    	    return;
    	}
    	
    	printf("Memory dump from 0x%04X to 0x%04X (%s):\n", 
    	       start, end, memory_get_region_name(start));
    	printf("Address  : 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | ASCII\n");
    	printf("---------|------------------------------------------------|----------------\n");
    	
    	for (address addr = start; addr <= end; addr += 16) {
    	    printf("0x%04X : ", addr);
    	    
    	    for (int offset = 0; offset < 16 && (addr + offset) <= end; offset++) {
    	        byte value = memory_read_byte(mem_sys, addr + offset);
    	        printf("%02X ", value);
    	    }
    	    
    	    int bytes_in_row = (end >= addr + 15) ? 16 : (end - addr + 1);
    	    for (int i = bytes_in_row; i < 16; i++) {
    	        printf("   ");
    	    }
    	    
    	    printf("| ");
    	    
    	    for (int offset = 0; offset < 16 && (addr + offset) <= end; offset++) {
    	        byte value = memory_read_byte(mem_sys, addr + offset);
    	        if (value >= 32 && value <= 126) {
    	            printf("%c", value);
    	        } else {
    	            printf(".");
    	        }
    	    }
    	    
    	    printf("\n");
    	}
    	printf("\n");
}

/**
 * @brief [TODO:description]
 *
 * @param mem_sys [TODO:parameter]
 * @param filename [TODO:parameter]
 * @return [TODO:return]
 */
bool memory_load_rom(memory_system_t *mem_sys, const char *filename)
{
	if (mem_sys == NULL || filename == NULL) {
		printf("ERROR: INVALID PARAMETERS FOR LOADING ROM");
		return false;
	}

	FILE *rom_file = fopen(filename, "rb");
	if (rom_file == NULL) {
		printf("ERROR: COULD NOT READ ROM FILE '%s' \n", filename);
		return false;
	}
	
	size_t bytes_read = fread(mem_sys->rom, 1, ROM_SIZE, rom_file);
	fclose(rom_file);

	if (bytes_read == 0) {
		printf("ERROR: FAILED TO READ ROM DATA");
		return false;
	}
	
	mem_sys->rom_loaded = true;
	printf("ROM LOADED SUCCESSFULLY");
	return true;
}
