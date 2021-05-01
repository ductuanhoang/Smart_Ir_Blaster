// Copyright 2017 David Conran

#include "IRutils.h"

#include <stdint.h>
#include <string.h>
#include <algorithm>

#include "IrSend.h"

/// Reverse the order of the requested least significant nr. of bits.
/// @param[in] input Bit pattern/integer to reverse.
/// @param[in] nbits Nr. of bits to reverse. (LSB -> MSB)
/// @return The reversed bit pattern.
uint64_t reverseBits(uint64_t input, uint16_t nbits)
{
  if (nbits <= 1)
    return input; // Reversing <= 1 bits makes no change at all.
  // Cap the nr. of bits to rotate to the max nr. of bits in the input.
  nbits = min(nbits, (uint16_t)(sizeof(input) * 8));
  uint64_t output = 0;
  for (uint16_t i = 0; i < nbits; i++)
  {
    output <<= 1;
    output |= (input & 1);
    input >>= 1;
  }
  // Merge any remaining unreversed bits back to the top of the reversed bits.
  return (input << nbits) | output;
}

/// Sum all the bytes of an array and return the least significant 8-bits of
/// the result.
/// @param[in] start A ptr to the start of the byte array to calculate over.
/// @param[in] length How many bytes to use in the calculation.
/// @param[in] init Starting value of the calculation to use. (Default is 0)
/// @return The 8-bit calculated result of all the bytes and init value.
uint8_t sumBytes(const uint8_t *const start, const uint16_t length,
                 const uint8_t init)
{
  uint8_t checksum = init;
  const uint8_t *ptr;
  for (ptr = start; ptr - start < length; ptr++)
    checksum += *ptr;
  return checksum;
}

/// Calculate a rolling XOR of all the bytes of an array.
/// @param[in] start A ptr to the start of the byte array to calculate over.
/// @param[in] length How many bytes to use in the calculation.
/// @param[in] init Starting value of the calculation to use. (Default is 0)
/// @return The 8-bit calculated result of all the bytes and init value.
uint8_t xorBytes(const uint8_t *const start, const uint16_t length,
                 const uint8_t init)
{
  uint8_t checksum = init;
  const uint8_t *ptr;
  for (ptr = start; ptr - start < length; ptr++)
    checksum ^= *ptr;
  return checksum;
}

/// Count the number of bits of a certain type in an array.
/// @param[in] start A ptr to the start of the byte array to calculate over.
/// @param[in] length How many bytes to use in the calculation.
/// @param[in] ones Count the binary nr of `1` bits. False is count the `0`s.
/// @param[in] init Starting value of the calculation to use. (Default is 0)
/// @return The nr. of bits found of the given type found in the array.
uint16_t countBits(const uint8_t *const start, const uint16_t length,
                   const bool ones, const uint16_t init)
{
  uint16_t count = init;
  for (uint16_t offset = 0; offset < length; offset++)
    for (uint8_t currentbyte = *(start + offset);
         currentbyte;
         currentbyte >>= 1)
      if (currentbyte & 1)
        count++;
  if (ones || length == 0)
    return count;
  else
    return (length * 8) - count;
}

/// Count the number of bits of a certain type in an Integer.
/// @param[in] data The value you want bits counted for. Starting from the LSB.
/// @param[in] length How many bits to use in the calculation? Starts at the LSB
/// @param[in] ones Count the binary nr of `1` bits. False is count the `0`s.
/// @param[in] init Starting value of the calculation to use. (Default is 0)
/// @return The nr. of bits found of the given type found in the Integer.
uint16_t countBits(const uint64_t data, const uint8_t length, const bool ones,
                   const uint16_t init)
{
  uint16_t count = init;
  uint8_t bitsSoFar = length;
  for (uint64_t remainder = data; remainder && bitsSoFar;
       remainder >>= 1, bitsSoFar--)
    if (remainder & 1)
      count++;
  if (ones || length == 0)
    return count;
  else
    return length - count;
}

/// Invert/Flip the bits in an Integer.
/// @param[in] data The Integer that will be inverted.
/// @param[in] nbits How many bits are to be inverted. Starting from the LSB.
/// @return An Integer with the appropriate bits inverted/flipped.
uint64_t invertBits(const uint64_t data, const uint16_t nbits)
{
  // No change if we are asked to invert no bits.
  if (nbits == 0)
    return data;
  uint64_t result = ~data;
  // If we are asked to invert all the bits or more than we have, it's simple.
  if (nbits >= sizeof(data) * 8)
    return result;
  // Mask off any unwanted bits and return the result.
  return (result & ((1ULL << nbits) - 1));
}

/// Convert degrees Celsius to degrees Fahrenheit.
float celsiusToFahrenheit(const float deg) { return (deg * 9.0) / 5.0 + 32.0; }

/// Convert degrees Fahrenheit to degrees Celsius.
float fahrenheitToCelsius(const float deg) { return (deg - 32.0) * 5.0 / 9.0; }

namespace irutils
{
  /// Sum all the nibbles together in a series of bytes.
  /// @param[in] start A ptr to the start of the byte array to calculate over.
  /// @param[in] length How many bytes to use in the calculation.
  /// @param[in] init Starting value of the calculation to use. (Default is 0)
  /// @return The 8-bit calculated result of all the bytes and init value.
  uint8_t sumNibbles(const uint8_t *const start, const uint16_t length,
                     const uint8_t init)
  {
    uint8_t sum = init;
    const uint8_t *ptr;
    for (ptr = start; ptr - start < length; ptr++)
      sum += (*ptr >> 4) + (*ptr & 0xF);
    return sum;
  }

  /// Sum all the nibbles together in an integer.
  /// @param[in] data The integer to be summed.
  /// @param[in] count The number of nibbles to sum. Starts from LSB. Max of 16.
  /// @param[in] init Starting value of the calculation to use. (Default is 0)
  /// @param[in] nibbleonly true, the result is 4 bits. false, it's 8 bits.
  /// @return The 4/8-bit calculated result of all the nibbles and init value.
  uint8_t sumNibbles(const uint64_t data, const uint8_t count,
                     const uint8_t init, const bool nibbleonly)
  {
    uint8_t sum = init;
    uint64_t copy = data;
    const uint8_t nrofnibbles = (count < 16) ? count : (64 / 4);
    for (uint8_t i = 0; i < nrofnibbles; i++, copy >>= 4)
      sum += copy & 0xF;
    return nibbleonly ? sum & 0xF : sum;
  }

  /// Convert a byte of Binary Coded Decimal(BCD) into an Integer.
  /// @param[in] bcd The BCD value.
  /// @return A normal Integer value.
  uint8_t bcdToUint8(const uint8_t bcd)
  {
    if (bcd > 0x99)
      return 255; // Too big.
    return (bcd >> 4) * 10 + (bcd & 0xF);
  }

  /// Convert an Integer into a byte of Binary Coded Decimal(BCD).
  /// @param[in] integer The number to convert.
  /// @return An 8-bit BCD value.
  uint8_t uint8ToBcd(const uint8_t integer)
  {
    if (integer > 99)
      return 255; // Too big.
    return ((integer / 10) << 4) + (integer % 10);
  }

  /// Return the value of `position`th bit of an Integer.
  /// @param[in] data Value to be examined.
  /// @param[in] position Nr. of the Nth bit to be examined. `0` is the LSB.
  /// @param[in] size Nr. of bits in data.
  /// @return The bit's value.
  bool getBit(const uint64_t data, const uint8_t position, const uint8_t size)
  {
    if (position >= size)
      return false; // Outside of range.
    return data & (1ULL << position);
  }

  /// Return the value of `position`th bit of an Integer.
  /// @param[in] data Value to be examined.
  /// @param[in] position Nr. of the Nth bit to be examined. `0` is the LSB.
  /// @return The bit's value.
  bool getBit(const uint8_t data, const uint8_t position)
  {
    if (position >= 8)
      return false; // Outside of range.
    return data & (1 << position);
  }

  /// Return the value of an Integer with the `position`th bit changed.
  /// @param[in] data Value to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  /// @param[in] size Nr. of bits in data.
  /// @return A suitably modified integer.
  uint64_t setBit(const uint64_t data, const uint8_t position, const bool on,
                  const uint8_t size)
  {
    if (position >= size)
      return data; // Outside of range.
    uint64_t mask = 1ULL << position;
    if (on)
      return data | mask;
    else
      return data & ~mask;
  }

  /// Return the value of an Integer with the `position`th bit changed.
  /// @param[in] data Value to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  /// @return A suitably modified integer.
  uint8_t setBit(const uint8_t data, const uint8_t position, const bool on)
  {
    if (position >= 8)
      return data; // Outside of range.
    uint8_t mask = 1 << position;
    if (on)
      return data | mask;
    else
      return data & ~mask;
  }

  /// Alter the value of an Integer with the `position`th bit changed.
  /// @param[in,out] data A pointer to the 8-bit integer to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  void setBit(uint8_t *const data, const uint8_t position, const bool on)
  {
    uint8_t mask = 1 << position;
    if (on)
      *data |= mask;
    else
      *data &= ~mask;
  }

  /// Alter the value of an Integer with the `position`th bit changed.
  /// @param[in,out] data A pointer to the 32-bit integer to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  void setBit(uint32_t *const data, const uint8_t position, const bool on)
  {
    uint32_t mask = (uint32_t)1 << position;
    if (on)
      *data |= mask;
    else
      *data &= ~mask;
  }

  /// Alter the value of an Integer with the `position`th bit changed.
  /// @param[in,out] data A pointer to the 64-bit integer to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  void setBit(uint64_t *const data, const uint8_t position, const bool on)
  {
    uint64_t mask = (uint64_t)1 << position;
    if (on)
      *data |= mask;
    else
      *data &= ~mask;
  }

  /// Alter an uint8_t value by overwriting an arbitrary given number of bits.
  /// @param[in,out] dst A pointer to the value to be changed.
  /// @param[in] offset Nr. of bits from the Least Significant Bit to be ignored
  /// @param[in] nbits Nr of bits of data to be placed into the destination.
  /// @param[in] data The value to be placed.
  void setBits(uint8_t *const dst, const uint8_t offset, const uint8_t nbits,
               const uint8_t data)
  {
    if (offset >= 8 || !nbits)
      return; // Short circuit as it won't change.
    // Calculate the mask for the supplied value.
    uint8_t mask = UINT8_MAX >> (8 - ((nbits > 8) ? 8 : nbits));
    // Calculate the mask & clear the space for the data.
    // Clear the destination bits.
    *dst &= ~(uint8_t)(mask << offset);
    // Merge in the data.
    *dst |= ((data & mask) << offset);
  }

  /// Alter an uint32_t value by overwriting an arbitrary given number of bits.
  /// @param[in,out] dst A pointer to the value to be changed.
  /// @param[in] offset Nr. of bits from the Least Significant Bit to be ignored
  /// @param[in] nbits Nr of bits of data to be placed into the destination.
  /// @param[in] data The value to be placed.
  void setBits(uint32_t *const dst, const uint8_t offset, const uint8_t nbits,
               const uint32_t data)
  {
    if (offset >= 32 || !nbits)
      return; // Short circuit as it won't change.
    // Calculate the mask for the supplied value.
    uint32_t mask = UINT32_MAX >> (32 - ((nbits > 32) ? 32 : nbits));
    // Calculate the mask & clear the space for the data.
    // Clear the destination bits.
    *dst &= ~(mask << offset);
    // Merge in the data.
    *dst |= ((data & mask) << offset);
  }

  /// Alter an uint64_t value by overwriting an arbitrary given number of bits.
  /// @param[in,out] dst A pointer to the value to be changed.
  /// @param[in] offset Nr. of bits from the Least Significant Bit to be ignored
  /// @param[in] nbits Nr of bits of data to be placed into the destination.
  /// @param[in] data The value to be placed.
  void setBits(uint64_t *const dst, const uint8_t offset, const uint8_t nbits,
               const uint64_t data)
  {
    if (offset >= 64 || !nbits)
      return; // Short circuit as it won't change.
    // Calculate the mask for the supplied value.
    uint64_t mask = UINT64_MAX >> (64 - ((nbits > 64) ? 64 : nbits));
    // Calculate the mask & clear the space for the data.
    // Clear the destination bits.
    *dst &= ~(mask << offset);
    // Merge in the data.
    *dst |= ((data & mask) << offset);
  }

  /// Create byte pairs where the second byte of the pair is a bit
  /// inverted/flipped copy of the first/previous byte of the pair.
  /// @param[in,out] ptr A pointer to the start of array to modify.
  /// @param[in] length The byte size of the array.
  /// @note A length of `<= 1` will do nothing.
  /// @return A ptr to the modified array.
  uint8_t *invertBytePairs(uint8_t *ptr, const uint16_t length)
  {
    for (uint16_t i = 1; i < length; i += 2)
    {
      // Code done this way to avoid a compiler warning bug.
      uint8_t inv = ~*(ptr + i - 1);
      *(ptr + i) = inv;
    }
    return ptr;
  }

  /// Check an array to see if every second byte of a pair is a bit
  /// inverted/flipped copy of the first/previous byte of the pair.
  /// @param[in] ptr A pointer to the start of array to check.
  /// @param[in] length The byte size of the array.
  /// @note A length of `<= 1` will always return true.
  /// @return true, if every second byte is inverted. Otherwise false.
  bool checkInvertedBytePairs(const uint8_t *const ptr,
                              const uint16_t length)
  {
    for (uint16_t i = 1; i < length; i += 2)
    {
      // Code done this way to avoid a compiler warning bug.
      uint8_t inv = ~*(ptr + i - 1);
      if (*(ptr + i) != inv)
        return false;
    }
    return true;
  }

  /// Perform a low level bit manipulation sanity check for the given cpu
  /// architecture and the compiler operation. Calls to this should return
  /// 0 if everything is as expected, anything else means the library won't work
  /// as expected.
  /// @return A bit mask value of potential issues.
  ///   0: (e.g. 0b00000000) Everything appears okay.
  ///   0th bit set: (0b1) Unexpected bit field/packing encountered.
  ///                Try a different compiler.
  ///   1st bit set: (0b10) Unexpected Endianness. Try a different compiler flag
  ///                or use a CPU different architecture.
  ///  e.g. A result of 3 (0b11) would mean both a bit field and an Endianness
  ///       issue has been found.
  uint8_t lowLevelSanityCheck(void)
  {
    const uint64_t kExpectedBitFieldResult = 0x8000012340000039ULL;
    volatile uint32_t EndianTest = 0x12345678;
    const uint8_t kBitFieldError = 0b01;
    const uint8_t kEndiannessError = 0b10;
    uint8_t result = 0;
    union bitpackdata
    {
      struct
      {
        uint64_t lowestbit : 1;  // 0th bit
        uint64_t next7bits : 7;  // 1-7th bits
        uint64_t _unused_1 : 20; // 8-27th bits
        // Cross the 32 bit boundary.
        uint64_t crossbits : 16;   // 28-43rd bits
        uint64_t _usused_2 : 18;   // 44-61st bits
        uint64_t highest2bits : 2; // 62-63rd bits
      };
      uint64_t all;
    };

    bitpackdata data;
    data.lowestbit = true;
    data.next7bits = 0b0011100; // 0x1C
    data._unused_1 = 0;
    data.crossbits = 0x1234;
    data._usused_2 = 0;
    data.highest2bits = 0b10; // 2

    if (data.all != kExpectedBitFieldResult)
      result |= kBitFieldError;
      // Check that we are using Little Endian for integers
#if defined(BYTE_ORDER) && defined(LITTLE_ENDIAN)
    if (BYTE_ORDER != LITTLE_ENDIAN)
      result |= kEndiannessError;
#endif
#if defined(__IEEE_BIG_ENDIAN) || defined(__IEEE_BYTES_BIG_ENDIAN)
    result |= kEndiannessError;
#endif
    // Brute force check for little endian.
    if (*((uint8_t *)(&EndianTest)) != 0x78) // NOLINT(readability/casting)
      result |= kEndiannessError;
    return result;
  }
} // namespace irutils
