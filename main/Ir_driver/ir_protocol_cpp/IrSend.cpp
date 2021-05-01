/*
 * IrSend.cpp
 *
 *  Created on: Jan 11, 2021
 *      Author: ductu
 */

/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrSend.h"
#include "../ir_driver_basic.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: 
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
Ir_Send::Ir_Send(uint32_t frequency, uint16_t dutycycle)
{
  // TODO Auto-generated constructor stub
  this->_dutycycle = dutycycle;
  this->_frequency = frequency;

  ir_basic_install_tx_channel(_frequency, _dutycycle);
}

Ir_Send::~Ir_Send()
{
  // TODO Auto-generated destructor stub
  //	bool ir_driver_uinstall(rmt_channel_t channel)
  ir_basic_uninstall_tx_channel();
  this->_dutycycle = 0;
  this->_frequency = 0;
}

void Ir_Send::sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                          const uint16_t onemark, const uint32_t onespace,
                          const uint16_t zeromark, const uint32_t zerospace,
                          const uint16_t footermark, const uint32_t gap,
                          const uint64_t data, const uint16_t nbits,
                          const uint16_t frequency, const bool MSBfirst,
                          const uint16_t repeat, const uint8_t dutycycle)
{
  sendGeneric(headermark, headerspace, onemark, onespace, zeromark, zerospace,
              footermark, gap, 0U, data, nbits, frequency, MSBfirst, repeat,
              dutycycle);
}

void Ir_Send::sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                          const uint16_t onemark, const uint32_t onespace,
                          const uint16_t zeromark, const uint32_t zerospace,
                          const uint16_t footermark, const uint32_t gap,
                          const uint32_t mesgtime, const uint64_t data,
                          const uint16_t nbits, const uint16_t frequency,
                          const bool MSBfirst, const uint16_t repeat,
                          const uint8_t dutycycle)
{
  // We always send a message, even for repeat=0, hence '<= repeat'.
  for (uint16_t r = 0; r <= repeat; r++)
  {
    if (headermark && headerspace)
    {
      mark_space(headermark, headerspace);
    }
    // Data
    sendData(onemark, onespace, zeromark, zerospace, data, nbits, MSBfirst);

    // Footer
    if (footermark && (gap > 0x7fff))
    {
      mark_space(footermark, 0x7fff);
    }
    else if (footermark)
    {
      mark_space(footermark, gap);
    }
    else
    {
    	mark_space(0, 0);
    }
  }
}

void Ir_Send::sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                          const uint16_t onemark, const uint32_t onespace,
                          const uint16_t zeromark, const uint32_t zerospace,
                          const uint16_t footermark, const uint32_t gap,
                          const uint8_t *dataptr, const uint16_t nbytes,
                          const uint16_t frequency, const bool MSBfirst,
                          const uint16_t repeat, const uint8_t dutycycle)
{
  if (headermark && headerspace)
  {
    mark_space(headermark, headerspace);
  }

  // Data
  for (uint16_t i = 0; i < nbytes; i++)
  {
    sendData(onemark, onespace, zeromark, zerospace, *(dataptr + i), 8, MSBfirst);
  }
  // Footer
  if (footermark && (gap > 0x7fff))
  {
    mark_space(footermark, 0x7fff);
  }
  else if (footermark)
  {
    mark_space(footermark, gap);
  }
  else
  {
	  mark_space(0, 0);
  }
}
void Ir_Send::sendGeneric(uint16_t *data_raw, uint16_t length)
{
  sendRaw(data_raw, length);
}

void Ir_Send::sendManchester(const uint16_t headermark,
                             const uint32_t headerspace,
                             const uint16_t half_period,
                             const uint16_t footermark, const uint32_t gap,
                             const uint64_t data, const uint16_t nbits,
                             const uint16_t frequency, const bool MSBfirst,
                             const uint16_t repeat, const uint8_t dutycycle,
                             const bool GEThomas)
{
  // Setup
  // We always send a message, even for repeat=0, hence '<= repeat'.
  for (uint16_t r = 0; r <= repeat; r++)
  {
    // Header
    if (headermark && headerspace)
    {
      mark_space(headermark, headerspace);
    }
    // Data
    sendManchesterData(half_period, data, nbits, MSBfirst, GEThomas);
    // Footer
    if (footermark && (gap > 0x7fff))
    {
      mark_space(footermark, 0x7fff);
    }
    else if (footermark)
    {
      mark_space(footermark, gap);
    }
    else
    {
    	mark_space(0, 0);
    }
  }
}
void Ir_Send::sendManchesterData(const uint16_t half_period,
                                 const uint64_t data,
                                 const uint16_t nbits, const bool MSBfirst,
                                 const bool GEThomas)
{
  if (nbits == 0)
    return; // Nothing to send.
  uint16_t bits = nbits;
  uint64_t copy = (GEThomas) ? data : ~data;

  if (MSBfirst)
  { // Send the MSB first.
    // Send 0's until we get down to a bit size we can actually manage.
    if (bits > (sizeof(data) * 8))
    {
      sendManchesterData(half_period, 0ULL, bits - sizeof(data) * 8, MSBfirst,
                         GEThomas);
      bits = sizeof(data) * 8;
    }
    // Send the supplied data.
    for (uint64_t mask = 1ULL << (bits - 1); mask; mask >>= 1)
    {
      if (copy & mask)
      {
        mark_space(half_period, half_period);
      }
      else
      {
        mark_space(half_period, half_period);
      }
    }
  }
  else
  { // Send the Least Significant Bit (LSB) first / MSB last.
    for (bits = 0; bits < nbits; bits++, copy >>= 1)
    {
      if (copy & 1)
      {
        mark_space(half_period, half_period);
      }
      else
      {
        mark_space(half_period, half_period);
      }
    }
  }
}

void Ir_Send::sendRaw(uint16_t *data_raw, uint16_t length)
{
  ir_driver_send_raw(data_raw, length);
}

void Ir_Send::mark_space(uint32_t mask, uint32_t space)
{
  ir_basic_mask_space(mask, space);
}

void Ir_Send::mark(uint16_t mask) {
    ir_basic_mark(mask);
}

void Ir_Send::space(uint32_t space) {
    ir_basic_space(space);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_Send::sendData(uint16_t onemark, uint32_t onespace, uint16_t zeromark,
                       uint32_t zerospace, uint64_t data, uint16_t nbits,
                       bool MSBfirst)
{
  if (nbits == 0) // If we are asked to send nothing, just return.
    return;
  if (MSBfirst)
  { // Send the MSB first.
    // Send 0's until we get down to a bit size we can actually manage.
    while (nbits > sizeof(data) * 8)
    {
      mark_space(zeromark, zerospace);
      nbits--;
    }
    // Send the supplied data.
    for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
    {
      if (data & mask)
      { // Send a 1
        mark_space(onemark, onespace);
      }
      else
      { // Send a 0
        mark_space(zeromark, zerospace);
      }
    }
  }
  else
  { // Send the Least Significant Bit (LSB) first / MSB last.
    for (uint16_t bit = 0; bit < nbits; bit++, data >>= 1)
    {
      if (data & 1)
      { // Send a 1
        mark_space(onemark, onespace);
      }
      else
      { // Send a 0
        mark_space(zeromark, zerospace);
      }
    }
  }
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/
