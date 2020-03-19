/*
 * uart2_main.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P
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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include <avr/interrupt.h>
#include <stdio.h>
#include "uart2.h"



#define MAX_LEN 80



void init(void);
char *fqgets(char *s, size_t n, FILE* fp);

int main(void)
{
    char s[MAX_LEN];
    
    
    
    init();
    
    while(1)
    {
        if(fqgets(s, MAX_LEN, &uart2_in))
            fputs(s, &uart2_out);
    }
}

void init(void)
{
    uart2_init();
    sei();
}



//Non-blocking gets, returns s when the line is complete
//https://gist.github.com/sebig3000/17c049f3562fccbbdfaeff090d166d60
char *fqgets(char *s, size_t n, FILE* fp)
{
    int c;
    static size_t i = 0;
    
    
    while((c = getc(fp)) != EOF)
    {
        s[i++] = c;
        
        if(c == '\n' || i >= n-1)
        {
            s[i] = '\0';
            i = 0;
            
            return s;
        }
    }
    
    return NULL;
}
