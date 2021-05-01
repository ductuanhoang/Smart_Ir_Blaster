#ifndef __GENERIC_FIFO_H__
#define __GENERIC_FIFO_H__
/** @file:    generic_fifo.h
 *  @details:  common service source code
 *
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
 * (C)Copyright VINSMART JSC 2020 All rights reserved
 ******************************************************************************/

/***************************** Include Files *********************************/
#include "../../Common.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/
typedef struct fifo_descriptor {
    /**
     * Start address on memory for item space.
     */
    void * itemspace;
    /**
     * Size in bytes for each element in the buffer.
     */
    size_t itemsize;
    /**
     * Size in bytes of the whole buffer.
     */
    size_t allocatedbytes;
    /**
     * Memory offset from which we will read data.
     */
    uint16_t readoffset;
    /**
     * Memory offset where data will be written.
     */
    uint16_t writeoffset;
    /**
    ¨* Number of bytes used currently by stored items.
     */
    size_t storedbytes;
}fifo_t;

enum fifo_side {
    E_FIFO_FRONT,
    E_FIFO_BACK,
};

#ifdef __cplusplus
extern "C" {
#endif

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

/********************* Exported Function Definitions *************************/

    /**
     * @brief      Creates a statically allocated FIFO buffer
     *
     * @param      p_fifo  The pointer to the structure holding fifo_t data
     * @param      buf     Pointer to the memory used to store actual fifo items
     * @param[in]  count   The number of items to store on the FIFO
     * @param[in]  size    The size in bytes of each item on the buffer
     *
     * @return     TRUE if success,
     *             FALSE if failed
     */
    bool fifo_create(fifo_t* p_fifo, void * buf, uint16_t count, size_t size);

    /**
     * @brief      Add item to the FIFO buffer
     *
     * @param      p_fifo  The pointer to the structure holding fifo_t data
     * @param[in]  item    Pointer to  data to be written to the fifo  buffer
     *
     * @return    TRUE if success,
     *            FALSE if failed
     */
    bool fifo_add(fifo_t* p_fifo, const void * item);

    /**
     * @brief      Get one item from the FIFO buffer.
     *
     * @param      p_fifo  The pointer to the structure holding fifo_t data
     * @param      item    Pointer to a location to hold the data read from the fifo buffer
     *
     * @return     TRUE if there is data available on the fifo buffer to be copied.
     *             FALSE if the buffer is empty and no data can be read
     */
    bool fifo_get(fifo_t* p_fifo, void * item);


    /**
     * @brief      Check the FIFO is full or not
     *
     * @param      p_fifo  The pointer to the structure holding fifo_t data
     *
     * @return     TRUE if the buffer is full,
     *             FALSE otherwise
     */
    bool fifo_is_full(fifo_t* p_fifo);

    /**
     * @brief      Check the FIFO is empty.or not
     *
     * @param      p_fifo  The pointer to the structure holding fifo_t data
     *
     * @return     TRUE if the buffer is empty
     *             FALSE otherwise.
     */
    bool fifo_is_empty(fifo_t* p_fifo);

    /**
     * @brief      Discard data from the buffer.
     *
     * This function discards data from the back or the front side of the buffer,
     * the side and the ammount of discarded data depends on the parameters passed.
     *
     * @param      p_fifo  The pointer to the structure holding fifo_t data
     * @param[in]  count   The number of elements to discard from the buffer
     * @param[in]  side    Defines if data should be discarted from the front or back side
     * of the buffer
     *
     * @return     TRUE if the data was discarted
     *             FALSE if not.
     */
    bool fifo_discard(fifo_t* p_fifo, uint16_t count, enum fifo_side side);


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
        int32_t *max_index, int32_t *min_index, int32_t *begin_index, int32_t *end_index);


    /**
     * @brief      Calculate average value of all values in fifo buffer
     *
     * @param      p_fifo     The fifo
     * @param      avg_value  The average value
     *
     * @return     0 if success, -1 if fail
     */
    int32_t fifo_calculate_avg(
            fifo_t* p_fifo,
            int32_t* avg_value
        );
/********************* Internal Function Definitions *************************/

/*****************************************************************************/

#ifdef __cplusplus
}
#endif


#endif /* __GENERIC_FIFO_H__ */
