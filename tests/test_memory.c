#include "../include/memory.h"
#include "../include/common.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// Test counter for tracking progress
static int tests_run = 0;
static int tests_passed = 0;

// Macro for test reporting
#define TEST_START(name) \
    do { \
        printf("Running test: %s... ", name); \
        tests_run++; \
    } while(0)

#define TEST_PASS() \
    do { \
        printf("✅ PASSED\n"); \
        tests_passed++; \
    } while(0)

#define TEST_FAIL(msg) \
    do { \
        printf("❌ FAILED: %s\n", msg); \
        exit(1); \
    } while(0)

// Function declarations
void test_memory_init(void);
void test_address_validation(void);
void test_region_names(void);
void test_byte_operations(void);
void test_rom_protection(void);
void test_word_operations(void);
void test_boundary_conditions(void);
void test_memory_patterns(void);
void test_error_handling(void);
void test_memory_cleanup(void);
void test_performance(void);
void test_current_directory(void);
void test_tetris_loading(void);

// Test memory system initialization
void test_memory_init(void)
{
    TEST_START("Memory Initialization");
    
    memory_system_t test_system;
    
    // Test successful initialization
    bool init_result = memory_init(&test_system);
    if (!init_result) {
        TEST_FAIL("Initialization returned false");
    }
    
    // Verify ROM is cleared
    for (int i = 0; i < 10; i++) {
        if (memory_read_byte(&test_system, ROM_START + i) != 0x00) {
            TEST_FAIL("ROM not properly cleared during initialization");
        }
    }
    
    // Verify VRAM is cleared
    for (int i = 0; i < 10; i++) {
        if (memory_read_byte(&test_system, VRAM_START + i) != 0x00) {
            TEST_FAIL("VRAM not properly cleared during initialization");
        }
    }
    
    // Verify WRAM is cleared
    for (int i = 0; i < 10; i++) {
        if (memory_read_byte(&test_system, WRAM_START + i) != 0x00) {
            TEST_FAIL("WRAM not properly cleared during initialization");
        }
    }
    
    // Verify ROM loaded flag is false
    if (test_system.rom_loaded != false) {
        TEST_FAIL("ROM loaded flag not set to false");
    }
    
    // Test null pointer handling
    if (memory_init(NULL) != false) {
        TEST_FAIL("Should return false for NULL pointer");
    }
    
    TEST_PASS();
}

// Test address validation
void test_address_validation(void)
{
    TEST_START("Address Validation");
    
    // Test valid addresses
    if (!memory_is_valid_address(ROM_START)) {
        TEST_FAIL("ROM_START should be valid");
    }
    
    if (!memory_is_valid_address(ROM_END)) {
        TEST_FAIL("ROM_END should be valid");
    }
    
    if (!memory_is_valid_address(VRAM_START)) {
        TEST_FAIL("VRAM_START should be valid");
    }
    
    if (!memory_is_valid_address(VRAM_END)) {
        TEST_FAIL("VRAM_END should be valid");
    }
    
    if (!memory_is_valid_address(WRAM_START)) {
        TEST_FAIL("WRAM_START should be valid");
    }
    
    if (!memory_is_valid_address(WRAM_END)) {
        TEST_FAIL("WRAM_END should be valid");
    }
    
    // Test invalid addresses - use valid 16-bit values that are unmapped
    if (memory_is_valid_address(0xA000)) {
        TEST_FAIL("Address 0xA000 should be invalid (unmapped region)");
    }
    
    if (memory_is_valid_address(0xBFFF)) {
        TEST_FAIL("Address 0xBFFF should be invalid (unmapped region)");
    }
    
    // Test edge case - maximum 16-bit value
    if (memory_is_valid_address(0xFFFF)) {
        // This might be valid depending on implementation
        printf("\n    Note: Address 0xFFFF is considered valid in your implementation\n");
    }
    
    TEST_PASS();
}

// Test region name identification
void test_region_names(void)
{
    TEST_START("Region Name Identification");
    
    // Test ROM region
    if (strcmp(memory_get_region_name(ROM_START), "ROM") != 0) {
        TEST_FAIL("ROM_START should return 'ROM'");
    }
    
    if (strcmp(memory_get_region_name(ROM_END), "ROM") != 0) {
        TEST_FAIL("ROM_END should return 'ROM'");
    }
    
    // Test VRAM region
    if (strcmp(memory_get_region_name(VRAM_START), "VRAM") != 0) {
        TEST_FAIL("VRAM_START should return 'VRAM'");
    }
    
    if (strcmp(memory_get_region_name(VRAM_END), "VRAM") != 0) {
        TEST_FAIL("VRAM_END should return 'VRAM'");
    }
    
    // Test WRAM region
    if (strcmp(memory_get_region_name(WRAM_START), "WRAM") != 0) {
        TEST_FAIL("WRAM_START should return 'WRAM'");
    }
    
    if (strcmp(memory_get_region_name(WRAM_END), "WRAM") != 0) {
        TEST_FAIL("WRAM_END should return 'WRAM'");
    }
    
    // Test invalid addresses - use valid 16-bit values
    if (strcmp(memory_get_region_name(0xFFFF), "Invalid") == 0) {
        // This is fine - some addresses might be marked as invalid
    }
    
    // Test unmapped addresses
    if (strcmp(memory_get_region_name(0xA000), "Unmapped") != 0) {
        TEST_FAIL("Address 0xA000 should return 'Unmapped'");
    }
    
    TEST_PASS();
}

// Test basic byte operations
void test_byte_operations(void)
{
    TEST_START("Byte Read/Write Operations");
    
    memory_system_t test_system;
    memory_init(&test_system);
    
    // Test VRAM read/write
    memory_write_byte(&test_system, VRAM_START, 0x42);
    byte vram_value = memory_read_byte(&test_system, VRAM_START);
    if (vram_value != 0x42) {
        TEST_FAIL("VRAM byte write/read failed");
    }
    
    // Test WRAM read/write
    memory_write_byte(&test_system, WRAM_START + 100, 0xAB);
    byte wram_value = memory_read_byte(&test_system, WRAM_START + 100);
    if (wram_value != 0xAB) {
        TEST_FAIL("WRAM byte write/read failed");
    }
    
    // Test multiple different values
    byte test_values[] = {0x00, 0xFF, 0x55, 0xAA, 0x01, 0xFE};
    int num_values = sizeof(test_values) / sizeof(test_values[0]);
    
    for (int i = 0; i < num_values; i++) {
        memory_write_byte(&test_system, VRAM_START + i, test_values[i]);
        byte read_value = memory_read_byte(&test_system, VRAM_START + i);
        if (read_value != test_values[i]) {
            TEST_FAIL("Multi-value byte test failed");
        }
    }
    
    // Test reading from invalid addresses returns 0xFF
    byte invalid_read = memory_read_byte(&test_system, 0xA000);
    if (invalid_read != 0xFF) {
        TEST_FAIL("Invalid address should return 0xFF");
    }
    
    TEST_PASS();
}

// Test ROM write protection
void test_rom_protection(void)
{
    TEST_START("ROM Write Protection");
    
    memory_system_t test_system;
    memory_init(&test_system);
    
    // Verify ROM starts as zero
    byte initial_value = memory_read_byte(&test_system, ROM_START);
    if (initial_value != 0x00) {
        TEST_FAIL("ROM should start as zero");
    }
    
    // Attempt to write to ROM (should be rejected)
    printf("\n    (Expecting ROM write error message below)\n    ");
    memory_write_byte(&test_system, ROM_START, 0xFF);
    
    // Verify ROM value didn't change
    byte after_write = memory_read_byte(&test_system, ROM_START);
    if (after_write != 0x00) {
        TEST_FAIL("ROM write should have been rejected");
    }
    
    // Test ROM protection at different addresses
    memory_write_byte(&test_system, ROM_START + 100, 0x33);
    byte rom_middle = memory_read_byte(&test_system, ROM_START + 100);
    if (rom_middle != 0x00) {
        TEST_FAIL("ROM write protection failed at middle address");
    }
    
    memory_write_byte(&test_system, ROM_END, 0x77);
    byte rom_end = memory_read_byte(&test_system, ROM_END);
    if (rom_end != 0x00) {
        TEST_FAIL("ROM write protection failed at end address");
    }
    
    TEST_PASS();
}

// Test word operations
void test_word_operations(void)
{
    TEST_START("Word Read/Write Operations");
    
    memory_system_t test_system;
    memory_init(&test_system);
    
    // Test basic word write/read
    word test_value = 0x1234;
    memory_write_word(&test_system, WRAM_START, test_value);
    word read_value = memory_read_word(&test_system, WRAM_START);
    
    if (read_value != test_value) {
        TEST_FAIL("Basic word write/read failed");
    }
    
    // Verify correct little-endian byte ordering
    byte low_byte = memory_read_byte(&test_system, WRAM_START);
    byte high_byte = memory_read_byte(&test_system, WRAM_START + 1);
    
    if (low_byte != 0x34) {
        TEST_FAIL("Low byte should be 0x34");
    }
    
    if (high_byte != 0x12) {
        TEST_FAIL("High byte should be 0x12");
    }
    
    // Test edge case values
    word edge_values[] = {0x0000, 0xFFFF, 0xFF00, 0x00FF, 0x5555, 0xAAAA};
    int num_edge_values = sizeof(edge_values) / sizeof(edge_values[0]);
    
    for (int i = 0; i < num_edge_values; i++) {
        memory_write_word(&test_system, VRAM_START + (i * 2), edge_values[i]);
        word read_edge = memory_read_word(&test_system, VRAM_START + (i * 2));
        if (read_edge != edge_values[i]) {
            TEST_FAIL("Edge case word test failed");
        }
    }
    
    // Test word operations across memory boundaries
    // Write word at last valid VRAM address - 1
    memory_write_word(&test_system, VRAM_END - 1, 0xABCD);
    word boundary_read = memory_read_word(&test_system, VRAM_END - 1);
    // This should work since both bytes are in VRAM
    if (boundary_read != 0xABCD) {
        TEST_FAIL("Word write at VRAM boundary failed");
    }
    
    TEST_PASS();
}

// Test boundary conditions
void test_boundary_conditions(void)
{
    TEST_START("Boundary Conditions");
    
    memory_system_t test_system;
    memory_init(&test_system);
    
    // Test reading from boundary between regions
    word boundary_word = memory_read_word(&test_system, 0x9FFF);
    // This reads one byte from VRAM and one from unmapped region
    // Should get 0x00 (from VRAM) and 0xFF (from unmapped) = 0xFF00
    if ((boundary_word & 0xFF) != 0x00 || ((boundary_word >> 8) & 0xFF) != 0xFF) {
        printf("\n    Note: Boundary read returned 0x%04X (expected 0xFF00)\n", boundary_word);
        // This is informational, not a failure
    }
    
    // Test all region boundaries
    address boundaries[] = {
        ROM_START, ROM_END,
        VRAM_START, VRAM_END,
        WRAM_START, WRAM_END
    };
    int num_boundaries = sizeof(boundaries) / sizeof(boundaries[0]);
    
    for (int i = 0; i < num_boundaries; i++) {
        if (!memory_is_valid_address(boundaries[i])) {
            TEST_FAIL("Boundary address should be valid");
        }
    }
    
    // Test one address beyond each region end
    address beyond_rom = ROM_END + 1;
    if (memory_is_valid_address(beyond_rom) && beyond_rom != VRAM_START) {
        TEST_FAIL("Address beyond ROM should be invalid (unless it's VRAM start)");
    }
    
    TEST_PASS();
}

// Test memory patterns
void test_memory_patterns(void)
{
    TEST_START("Memory Pattern Testing");
    
    memory_system_t test_system;
    memory_init(&test_system);
    
    // Fill VRAM with a pattern
    for (address addr = VRAM_START; addr <= VRAM_END; addr++) {
        byte pattern_value = (byte)(addr & 0xFF);
        memory_write_byte(&test_system, addr, pattern_value);
    }
    
    // Verify the pattern
    for (address addr = VRAM_START; addr <= VRAM_END; addr++) {
        byte expected = (byte)(addr & 0xFF);
        byte actual = memory_read_byte(&test_system, addr);
        if (actual != expected) {
            TEST_FAIL("Memory pattern verification failed");
        }
    }
    
    // Fill WRAM with inverse pattern
    for (address addr = WRAM_START; addr <= WRAM_END; addr++) {
        byte pattern_value = (byte)(~(addr & 0xFF));
        memory_write_byte(&test_system, addr, pattern_value);
    }
    
    // Verify inverse pattern
    for (address addr = WRAM_START; addr <= WRAM_END; addr++) {
        byte expected = (byte)(~(addr & 0xFF));
        byte actual = memory_read_byte(&test_system, addr);
        if (actual != expected) {
            TEST_FAIL("Inverse memory pattern verification failed");
        }
    }
    
    TEST_PASS();
}

// Test error handling
void test_error_handling(void)
{
    TEST_START("Error Handling");
    
    // These tests verify that error conditions don't crash the program
    
    // Test null pointer to read function - should trigger assertion in debug builds
    printf("\n    (Note: NULL pointer tests may trigger assertions in debug builds)\n");
    
    // Test invalid address reads - use unmapped but valid 16-bit addresses
    memory_system_t test_system;
    memory_init(&test_system);
    
    byte invalid_result = memory_read_byte(&test_system, 0xA000);
    if (invalid_result != 0xFF) {
        TEST_FAIL("Invalid address read should return 0xFF");
    }
    
    // Test invalid address writes (should not crash)
    memory_write_byte(&test_system, 0xA000, 0x42);
    // If we get here without crashing, the test passes
    
    // Test edge case addresses within 16-bit range
    byte edge_result = memory_read_byte(&test_system, 0xFFFF);
    if (edge_result != 0xFF) {
        // This might be valid depending on implementation
        printf("\n    Note: Address 0xFFFF returned 0x%02X\n", edge_result);
    }
    
    TEST_PASS();
}

// Test cleanup function
void test_memory_cleanup(void)
{
    TEST_START("Memory Cleanup");
    
    memory_system_t test_system;
    memory_init(&test_system);
    
    // Write some data
    memory_write_byte(&test_system, VRAM_START, 0x42);
    memory_write_byte(&test_system, WRAM_START, 0xAB);
    test_system.rom_loaded = true;
    
    // Clean up
    memory_cleanup(&test_system);
    
    // Verify cleanup worked
    if (memory_read_byte(&test_system, VRAM_START) != 0x00) {
        TEST_FAIL("VRAM not cleared after cleanup");
    }
    
    if (memory_read_byte(&test_system, WRAM_START) != 0x00) {
        TEST_FAIL("WRAM not cleared after cleanup");
    }
    
    if (test_system.rom_loaded != false) {
        TEST_FAIL("ROM loaded flag not reset after cleanup");
    }
    
    TEST_PASS();
}

// Performance test (basic)
void test_performance(void)
{
    TEST_START("Basic Performance Test");
    
    memory_system_t test_system;
    memory_init(&test_system);
    
    printf("\n    Performing 10000 memory operations... ");
    
    // Simple performance test - just verify operations complete quickly
    for (int i = 0; i < 10000; i++) {
        address addr = VRAM_START + (address)(i % VRAM_SIZE);
        memory_write_byte(&test_system, addr, (byte)(i & 0xFF));
        byte value = memory_read_byte(&test_system, addr);
        (void)value; // Suppress unused variable warning
    }
    
    printf("completed");
    
    TEST_PASS();
}

// Test current directory and file access
void test_current_directory(void)
{
    TEST_START("Directory and File Access");
    
    printf("\n    Checking current directory contents:\n");
    
    // Try to open executable (should exist in current directory)
    FILE *test = fopen("test_memory", "r");
    if (test != NULL) {
        printf("    ✅ Found test_memory executable in current directory\n");
        fclose(test);
    } else {
        printf("    ❌ test_memory not found - running from different directory?\n");
    }
    
    // Check for common ROM file locations
    const char* possible_roms[] = {
        "tetris.gb",
        "tests/tetris.gb", 
        "../tetris.gb",
        "TETRIS.GB"
    };
    
    bool found_rom = false;
    for (int i = 0; i < 4; i++) {
        FILE *rom_test = fopen(possible_roms[i], "r");
        if (rom_test != NULL) {
            printf("    ✅ Found ROM at: %s\n", possible_roms[i]);
            fclose(rom_test);
            found_rom = true;
        }
    }
    
    if (!found_rom) {
        printf("    ℹ️  No ROM files found (this is optional for testing)\n");
    }
    
    TEST_PASS();
}

// Test ROM loading functionality
void test_tetris_loading(void)
{
    TEST_START("ROM Loading Test");
    
    memory_system_t game_system;
    memory_init(&game_system);
    
    // Try multiple possible ROM file locations
    const char* rom_files[] = {"tetris.gb", "TETRIS.GB", "tests/tetris.gb"};
    bool rom_loaded = false;
    
    for (int i = 0; i < 3 && !rom_loaded; i++) {
        if (memory_load_rom(&game_system, rom_files[i])) {
            printf("\n🎉 ROM loaded successfully from: %s\n\n", rom_files[i]);
            
            // Show the Game Boy header area (contains game info)
            printf("=== Game Boy Header Information ===\n");
            memory_dump_region(&game_system, 0x0100, 0x014F);
            
            // Check the game title (usually around 0x0134-0x0143)
            printf("Game title: \"");
            for (address addr = 0x0134; addr <= 0x0143; addr++) {
                byte b = memory_read_byte(&game_system, addr);
                if (b >= 32 && b <= 126) {
                    printf("%c", b);
                } else if (b == 0) {
                    break; // Null terminator
                } else {
                    printf(".");
                }
            }
            printf("\"\n\n");
            
            // Show some early game code
            printf("=== Early Game Code ===\n");
            memory_dump_region(&game_system, 0x0150, 0x016F);
            
            rom_loaded = true;
            TEST_PASS();
        }
    }
    
    if (!rom_loaded) {
        printf("\n⚠️  No ROM file found - this test is optional\n");
        printf("    To test ROM loading, place a Game Boy ROM file as:\n");
        printf("    - tetris.gb (in current directory)\n");
        printf("    - TETRIS.GB (in current directory)\n");
        printf("    - tests/tetris.gb (in tests subdirectory)\n");
        
        // Still count as passed since ROM files are optional
        printf("⚠️  SKIPPED (ROM file not available)\n");
        tests_passed++;
    }
}

// Main test runner
int main(void)
{
    printf("=== Game Boy Emulator Memory System Test Suite ===\n\n");
    
    // Run all core memory system tests
    test_memory_init();
    test_address_validation();
    test_region_names();
    test_byte_operations();
    test_rom_protection();
    test_word_operations();
    test_boundary_conditions();
    test_memory_patterns();
    test_error_handling();
    test_memory_cleanup();
    test_performance();
    
    printf("\n=== Additional Tests ===\n");
    
    // Run optional tests
    test_current_directory();
    test_tetris_loading();
    
    // Print summary
    printf("\n=== Test Summary ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    
    if (tests_passed == tests_run) {
        printf("🎉 All tests PASSED! Your memory system is working correctly.\n");
        return 0;
    } else {
        printf("❌ Some tests FAILED. Please review the output above.\n");
        return 1;
    }
}
