
// BitWrite and bitRead functions for 64-bit integers
void bitWrite64(uint64_t &value, int bit, int bitValue) 
{
if (bit > 63) return; // Prevent out-of-range access
if (bitValue) {value |= (uint64_t(1) << bit);}  // Set bit
else {value &= ~(uint64_t(1) << bit);} // Clear bit
}

int bitRead64(uint64_t value, int bit) 
{
if (bit > 63) {return 0;} // Out of range
return (int)((value >> bit) & 0x01ULL); 
// ull or llu (or caps) force the compiler to treat a constant as an unsigned and long long integer
}

// Branchless population count for 64-bit integers (counts number of 1s)
static inline int popcount64_branchless(uint64_t x) 
{
// Step 1: Subtract pairs of bits
x = x - ((x >> 1) & 0x5555555555555555ULL);
// Step 2: Count bits in nibbles
x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
// Step 3: Sum counts in bytes
x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
// Step 4: Multiply to sum all bytes into the high byte
return (x * 0x0101010101010101ULL) >> 56;
}

// Function to generate a random uint64_t
uint64_t random_uint64(void) 
{
uint64_t r = 0;
unsigned long seedValue = millis();
srand(seedValue);
// RAND_MAX is often 2^31 - 1, so we combine multiple calls
for (int i = 0; i < 4; i++) {
r = (r << 16) ^ (rand() & 0xFFFF); // take lower 16 bits each time
}
return r;
}

// Convert exactly 8 characters to uint64_t
uint64_t str8_to_uint64(const char str[8]) {
    uint64_t value = 0;
    for (int i = 0; i <= 7; i++) {
        value = (value << 8) | (uint8_t)str[i];
    }
    return value;
}

// Convert uint64_t back to exactly 8 characters
void uint64_to_str8(char str[9], uint64_t value) {
    for (int i = 7; i >= 0; i--) {
        str[i] = (char)(value & 0xFF);
        value >>= 8;
    }
    str[8] = '\0'; // Null-terminate for safety
}

// Convert exactly 8 characters to uint64_t in reverse order from above
uint64_t str8_to_uint64_reverse(const char str[8]) {
    uint64_t value = 0;
    for (int i = 7; i >= 0; i--) {
        value = (value << 8) | (uint8_t)str[i];
    }
    return value;
}

// Convert uint64_t back to exactly 8 characters in reverse order from above
void uint64_to_str8_reverse(char str[9], uint64_t value) {
    for (int i = 0; i <= 7; i++) {
        str[i] = (char)(value & 0xFF);
        value >>= 8;
    }
    str[8] = '\0'; // Null-terminate for safety
}

int isStr8Ascii(const char str[9]) // checks if an 8 char string is ascii
{
for (int i = 0; i <= 7; i++) {
  if ((uint8_t)str[i] < 0x20 || (uint8_t)str[i] > 0x7e){ // printable charaqcters
    return 0;
    }
  }
return 1;
}
