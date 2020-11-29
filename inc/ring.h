/*
 * ring.h
 * 
 * Circular buffer (FIFO) implementation.
 * 
 * Author:      Sebastian Goessl
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2018 Sebastian Goessl
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */



#ifndef RING_H_
#define RING_H_



#include <stdbool.h>    //bool type
#include <stddef.h>     //size_t type
#include <stdint.h>     //uint8_t type



/**
 * Ring buffer handler.
 */
typedef struct
{
    /** Data buffer start, included. */
    uint8_t *buf;
    /** Data buffer end, excluded. */
    uint8_t *end;
    /** Next write location. */
    uint8_t *write;
    /** Next read location. */
    uint8_t *read;
} Ring_t;



/**
 * Ring buffer handler initializer, macro version.
 * Use only if really needed.
 */
#define RING_INIT(buf_, len_) \
    ((Ring_t){.buf = (buf_), .end = (buf_)+(len_), \
        .write = (buf_), .read = (buf_)})



/**
 * Initializes a new ring buffer handler for the given buffer location.
 * The ring buffer will only be able to hold (len-1) elements.
 * 
 * @param buf location that will be used to actually store the data
 * @param len size of the buffer location
 * @return a new ring buffer handler
 */
Ring_t ring_init(uint8_t *buf, size_t len);

/**
 * Returns true if no elements can be popped from the ring buffer
 * and false otherwise.
 * 
 * @param ring ring buffer to test
 * @return if no elements can be popped from the ring buffer
 */
bool ring_isEmpty(Ring_t ring);
/**
 * Returns true if no elements can be pushed into the ring buffer
 * and false otherwise.
 * 
 * @param ring ring buffer to test
 * @return if no elements can be pushed into the ring buffer
 */
bool ring_isFull(Ring_t ring);
/**
 * Returns the number of elements that can be pushed into the ring buffer.
 * 
 * @param ring ring buffer to test
 * @return the number of elements that can be pushed into the ring buffer
 */
size_t ring_pushAvailable(Ring_t ring);
/**
 * Returns the number of elements that can be popped from the ring buffer.
 * 
 * @param ring ring buffer to test
 * @return the number of elements that can be popped from the ring buffer
 */
size_t ring_popAvailable(Ring_t ring);

/**
 * Adds a new element to the ring buffer.
 * If the ring buffer is full nothing will be changed and 1 will be returned.
 * 
 * @param ring pointer to the ring buffer the element should be pushed into
 * @param data the element that should be pushed into the ring buffer
 * @return 0 if the element was successfully pushed, 1 otherwise
 */
bool ring_push(Ring_t *ring, uint8_t data);
/**
 * Adds a new element to the buffer, potentially overwriting the oldest data.
 * If the ring buffer is full the oldest data will be overwritten
 * and 1 will be returned.
 * 
 * @param ring pointer to the ring buffer the element should be pushed into
 * @param data the element that should be pushed into the ring buffer
 * @return 0 if the element was pushed without overwriting data, 1 otherwise
 */
bool ring_pushOver(Ring_t *ring, uint8_t data);

/**
 * Retrieves the next element from the buffer to the given location
 * and removes it from the ring buffer.
 * If the ring buffer is empty, no element will be popped
 * nor written to the given location and 1 will be returned.
 * 
 * @param ring pointer to the ring buffer the element should be popped from
 * @param data location where the popped element should be written to
 * @return 0 if an element was successfully popped, 1 otherwise
 */
bool ring_pop(Ring_t *ring, uint8_t *data);
/**
 * Retrieves the next element from the buffer to the given location
 * without removing it from the ring buffer.
 * If the ring buffer is empty, no element will be peeked
 * nor written to the given location and 1 will be returned.
 * 
 * @param ring pointer to the ring buffer the element should be peeked from
 * @param data location where the peeked element should be written to
 * @return 0 if an element was successfully peeked, 1 otherwise
 */
bool ring_peek(Ring_t *ring, uint8_t *data);



#endif /* RING_H_ */
