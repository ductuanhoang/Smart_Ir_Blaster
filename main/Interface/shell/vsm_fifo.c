/** @file:    generic_fifo.c
 *  @details:  generic FIFO structure
 *
 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". VINSMART
 * JSC MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
 * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. VINSMART JSC
 * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
 * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
 * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
 *
 * VINSMART JSC SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
 * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
 * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
 * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
 * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
 * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
 *
 * (C)Copyright VINSMART JSC 2019 All rights reserved
 ******************************************************************************/

/***************************** Include Files *********************************/
#include "vsm_fifo.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/********************** Internal Function Prototypes *************************/
static void fifo_copy_from(fifo_t* p_fifo, void *);
static void fifo_copy_to(fifo_t* p_fifo, const void *);

/************************** Variable Definitions *****************************/

/********************* Exported Function Definitions *************************/

bool fifo_create(fifo_t* p_fifo, void * buf, uint16_t count, size_t size)
{
    // Sanity check for memory and element sizes
    if (buf != NULL && p_fifo != NULL && count != 0) {
        p_fifo->itemspace = buf;
        p_fifo->itemsize = size;
        p_fifo->allocatedbytes = count * size;
        p_fifo->readoffset = 0;
        p_fifo->writeoffset = 0;
        p_fifo->storedbytes = 0;
        return true;
    }
    return false;
}

bool fifo_add(fifo_t* p_fifo, const void * item)
{
    if (!fifo_is_full(p_fifo)) {
        fifo_copy_to(p_fifo, item);
        p_fifo->storedbytes += p_fifo->itemsize;
        return true;
    } else {
        return false;
    }
}

bool fifo_get(fifo_t* p_fifo, void * item)
{
    if (!fifo_is_empty(p_fifo)) {
        fifo_copy_from(p_fifo, item);
        p_fifo->storedbytes -= p_fifo->itemsize;
        return true;
    } else {
        return false;
    }
}

bool fifo_is_full(fifo_t* p_fifo)
{
    if (p_fifo->storedbytes >= p_fifo->allocatedbytes)
        return true;
    else
        return false;
}

bool fifo_is_empty(fifo_t* p_fifo)
{
    if (p_fifo->storedbytes == 0)
        return true;
    else
        return false;
}

bool fifo_discard(fifo_t* p_fifo, uint16_t count, enum fifo_side side)
{
    uint16_t t;
    t = p_fifo->itemsize * count; // Compute byte size of elements to be deleted
    if (t <= p_fifo->storedbytes) // Check if we can remove the requested ammount of data
    {
        if (side == E_FIFO_FRONT) {
            p_fifo->readoffset = (p_fifo->readoffset + t) % p_fifo->allocatedbytes; // Increase read pointer n elements
            p_fifo->storedbytes -= t; // Decrease stored bytes number
        } else if (side == E_FIFO_BACK) {
            p_fifo->writeoffset = (p_fifo->writeoffset - t) % p_fifo->allocatedbytes; // Decrease write pointer n elements
            p_fifo->storedbytes -= t; // Decrease stored bytes number
        }
        return true;
    }
    return false;
}

/**
 * @brief      Get maximum element index
 *
 * @param      p_fifo  The fifo
 *
 * @return     { description_of_the_return_value }
 */
int32_t fifo_calculate_max_min(
    fifo_t* p_fifo,
    int32_t *max, int32_t *min, int32_t *begin, int32_t *end,
    int32_t *max_index, int32_t *min_index, int32_t *begin_index, int32_t *end_index)
{
    if(NULL == p_fifo) {
        return -1;
    }
    if(p_fifo->itemsize != 4) {
        return -2;
    }

    int32_t *p = (int32_t*)p_fifo->itemspace;
    int32_t count = p_fifo->allocatedbytes / p_fifo->itemsize;
    int32_t tmp_max = p[0];
    int32_t tmp_min = p[0];


    for(int32_t i = 1; i < count; i ++) {
        if(p[i] > tmp_max) {
            tmp_max = p[i];
            *max_index = i;
        }

        if(p[i] < tmp_min) {
            tmp_min = p[i];
            *min_index = i;
        }
    }


    *begin = p[0];
    *end = p[count - 1];
    *max = tmp_max;
    *min = tmp_min;
    *begin_index = 0;
    *end_index = count-1;

      return 0;
}


/**
 * @brief      Calculate average value of all values in fifo buffer
 *
 * @param      p_fifo     The fifo
 * @param      avg_value  The average value
 *
 * @return     0 if success, -1 if fail
 */
int32_t fifo_calculate_avg( fifo_t* p_fifo, int32_t* avg_value)
{
    int32_t count = p_fifo->allocatedbytes / p_fifo->itemsize;

    int32_t avg = 0;
    int32_t *p = (int32_t*)p_fifo->itemspace;

    if (count == 0) {
        return -1;
    }

    for(int32_t i = 1; i < count; i ++) {

        avg = (avg + p[i])/2;
    }
    *avg_value = avg;

    return 0;
}



/********************* Internal Function Definitions *************************/
static void fifo_copy_from(fifo_t* p_fifo, void * item)
{
    memcpy(item, (uint8_t*)p_fifo->itemspace + p_fifo->readoffset, p_fifo->itemsize);
    p_fifo->readoffset += p_fifo->itemsize;
    if (p_fifo->readoffset >= p_fifo->allocatedbytes) {
        p_fifo->readoffset = 0;
    }
}

static void fifo_copy_to(fifo_t* p_fifo, const void *item)
{
    memcpy((uint8_t*)p_fifo->itemspace + p_fifo->writeoffset, item, p_fifo->itemsize);
    p_fifo->writeoffset += p_fifo->itemsize;
    if (p_fifo->writeoffset >= p_fifo->allocatedbytes) {
        p_fifo->writeoffset = 0;
    }
}

/*****************************************************************************/
