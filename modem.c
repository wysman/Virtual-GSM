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
 
/*
 *  Run a pseudo-terminal to test the modem,
 *  minicom -p /dev/pts/14 -o
 *
 *  To send a CTRL-Z in minicom, you must send a escape before (CTRL-A)
 */

#define _XOPEN_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "types.h"

#define MODEM_READ_BUFFER_SIZE  32

void ModemParserExecute(ModemParserCtx_t *ctx, int fd, const char *buffer, size_t len);
void ModemParserInit(ModemParserCtx_t *ctx);

ModemStatus_t GSM = {
    .smsCount = 0,
    .isPinLocked = true,
    .pincode = "123456"
};
ModemStatus_t *modemStatusHandler = &GSM;

ModemParserCtx_t modemCtx = {
    .buffer = NULL,
};
ModemParserCtx_t *modemCtxHandler = &modemCtx;

int
main (void)
{
    int ret;
    char buffer[MODEM_READ_BUFFER_SIZE];

    // Create virtual COM
    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    assert(fd != 0);

    
    ret = grantpt(fd);
    assert(ret == 0);
    
    ret = unlockpt(fd);
    assert(ret == 0);
    
    char *name = ptsname(fd);
    printf("Virtual COM port : %s\n", name);

    // Init parser ctx & excute on incoming data
    ModemParserInit(modemCtxHandler);
    while (1) {
        ret = read (fd, buffer, MODEM_READ_BUFFER_SIZE);
        if (ret < 0) {
            break;
        }
        
        ModemParserExecute(modemCtxHandler, fd, buffer, ret);
    }
    
    return 0;
}
