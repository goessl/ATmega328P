/*
 * ring.h
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



#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



typedef struct
{
    uint8_t *buf;
    uint8_t *end;
    uint8_t *write;
    uint8_t *read;
} Ring_t;



#define RING_INIT(buf_, len_) \
    ((Ring_t){.buf = (buf_), .end = (buf_)+(len_), \
        .write = (buf_), .read = (buf_)})



Ring_t ring_init(uint8_t *buf, size_t len);

bool ring_isEmpty(Ring_t ring);
bool ring_isFull(Ring_t ring);
size_t ring_pushAvailable(Ring_t ring);
size_t ring_popAvailable(Ring_t ring);

bool ring_push(Ring_t *ring, uint8_t data);
bool ring_pushOver(Ring_t *ring, uint8_t data);

bool ring_pop(Ring_t *ring, uint8_t *data);
bool ring_peek(Ring_t *ring, uint8_t *data);



#endif /* RING_H_ */
