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
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include "RING.h"



#define RING_INC_ROLL_OVER(n, s, e) ((n+1 >= e)?s:n+1)



RING_t RING_init(uint8_t* buf, size_t len)
{
    return (RING_t){.buf = buf, .end = buf + len, .write = buf, .read = buf};
}


int RING_isEmpty(RING_t ring)
{
    return ring.read == ring.write;
}

int RING_isFull(RING_t ring)
{
    return RING_INC_ROLL_OVER(ring.write, ring.buf, ring.end) == ring.read;
}

size_t RING_pushAvailable(RING_t ring)
{
    if(ring.write < ring.read)
        return ring.read - ring.write - 1;
    else
        return (ring.end - ring.buf) - (ring.write - ring.read) - 1;
}

size_t RING_popAvailable(RING_t ring)
{
    if(ring.read <= ring.write)
        return ring.write - ring.read;
    else
        return (ring.end - ring.buf) - (ring.read - ring.write);
}


int RING_push(RING_t* ring, uint8_t data)
{
    if(RING_isFull(*ring))
        return 1;
    
    *ring->write = data;
    ring->write = RING_INC_ROLL_OVER(ring->write, ring->buf, ring->end);
    
    return 0;
}

int RING_pushOver(RING_t* ring, uint8_t data)
{
    *ring->write = data;
    ring->write = RING_INC_ROLL_OVER(ring->write, ring->buf, ring->end);
    
    if(ring->read == ring->write)
    {
        ring->read = RING_INC_ROLL_OVER(ring->read, ring->buf, ring->end);
        return 1;
    }
    
    return 0;
}


int RING_pop(RING_t* ring, uint8_t* data)
{
    if(RING_isEmpty(*ring))
        return 1;
    
    *data = *ring->read;
    ring->read = RING_INC_ROLL_OVER(ring->read, ring->buf, ring->end);
    
    return 0;
}

int RING_peek(RING_t* ring, uint8_t* data)
{
    if(RING_isEmpty(*ring))
        return 1;
    
    *data = *ring->read;
    
    return 0;
}
