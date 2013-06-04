/*
 *  The MIT License (MIT)
 *  
 *  Copyright (c) 2012 William MARTIN
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
#ifndef _TYPES_H_
#define _TYPES_H_

#define MODEM_MAX_PINCODE_SIZE  6
#define MODEM_MAX_ARG   20

#include <stdbool.h>
#include <stdint.h>

typedef struct _ModemStatus {
    char pincode[MODEM_MAX_PINCODE_SIZE + 1];
    bool isPinLocked;
    int smsCount;
} ModemStatus_t;
extern ModemStatus_t *modemStatusHandler;

typedef struct _ModemParserCtx {
    // ragel ctx
    int cs;
    char *p, *pe;
    
    // protocole ctx
    char *buffer;
    uint32_t buffer_size;
    uint32_t buffer_len;
    uint32_t mark;

    uint32_t cmd;
    uint32_t arg[MODEM_MAX_ARG];
    uint32_t arg_len[MODEM_MAX_ARG];
    uint32_t arg_count;
} ModemParserCtx_t;

#define PTR_TO_CMD() (ctx->buffer + ctx->cmd)
#define LEN_CMD() (ctx->cmd_len)

#define PTR_TO_ARG(N) (ctx->buffer + ctx->arg[N])
#define LEN_ARG(N) (ctx->arg_len[N])

#endif /* _TYPES_H_ */
