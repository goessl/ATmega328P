/*
 * RING.c
 * 
 * Author:      Sebastian Gössl
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2018 Sebastian Gössl
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



#include "RING.h"



#define RING_INC_ROLL_OVER(n, s, e) (((n)+1>=(e)) ? (s) : (n)+1)



RING_t RING_init(uint8_t* buf, size_t len)
{
    return RING_INIT(buf, len);
}



bool RING_isEmpty(RING_t RING)
{
    return RING.read == RING.write;
}

bool RING_isFull(RING_t RING)
{
    return RING_INC_ROLL_OVER(RING.write, RING.buf, RING.end) == RING.read;
}

size_t RING_pushAvailable(RING_t RING)
{
    if(RING.write < RING.read)
        return RING.read - RING.write - 1;
    else
        return (RING.end - RING.buf) - (RING.write - RING.read) - 1;
}

size_t RING_popAvailable(RING_t RING)
{
    if(RING.read <= RING.write)
        return RING.write - RING.read;
    else
        return (RING.end - RING.buf) - (RING.read - RING.write);
}


bool RING_push(RING_t* RING, uint8_t data)
{
    if(RING_isFull(*RING))
        return 1;
    
    *RING->write = data;
    RING->write = RING_INC_ROLL_OVER(RING->write, RING->buf, RING->end);
    
    return 0;
}

bool RING_pushOver(RING_t* RING, uint8_t data)
{
    *RING->write = data;
    RING->write = RING_INC_ROLL_OVER(RING->write, RING->buf, RING->end);
    
    if(RING->read == RING->write)
    {
        RING->read = RING_INC_ROLL_OVER(RING->read, RING->buf, RING->end);
        return 1;
    }
    
    return 0;
}


bool RING_pop(RING_t* RING, uint8_t* data)
{
    if(RING_isEmpty(*RING))
        return 1;
    
    *data = *RING->read;
    RING->read = RING_INC_ROLL_OVER(RING->read, RING->buf, RING->end);
    
    return 0;
}

bool RING_peek(RING_t* RING, uint8_t* data)
{
    if(RING_isEmpty(*RING))
        return 1;
    
    *data = *RING->read;
    
    return 0;
}
