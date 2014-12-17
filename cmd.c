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
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "types.h"
#include "cmd.h"

static int _fd;
#define SEND(fmt, args...) dprintf(_fd, fmt, ##args)
#define SEND_OK() SEND("OK\r\n")
#define SEND_ERROR() SEND("ERROR\r\n")



/*
 *  +CPIN
 */
static inline
void
pincodeValidate(void)
{
    if (modemStatusHandler->isPinLocked == true) {
        SEND("+CPIN: SIM PIN\r\n");
    } else {
        SEND("+CPIN: READY\r\n");
    }
}

static inline
void
pincodeUnlock(char *pincode)
{
    if (strlen(modemStatusHandler->pincode) != strlen(pincode)) {
        SEND_ERROR();
    } else if (!strcmp(pincode, modemStatusHandler->pincode)) {
        modemStatusHandler->isPinLocked = false;
        SEND_OK();
        printf("SIM Card is unlocked\n");
    } else {
        SEND_ERROR();
    }
}

static inline
void
pincodeUpdate(char *pincode, char *new)
{
    int lenNew = strlen(new);
    
    // Ask for pin change
    if (strcmp(pincode, modemStatusHandler->pincode)) {
        SEND_ERROR();
    } else if (lenNew > MODEM_MAX_PINCODE_SIZE) {
        SEND_ERROR();
    } else {
        memcpy(modemStatusHandler->pincode, new, lenNew + 1);
        printf("SIM Card code is now : %s\n", modemStatusHandler->pincode);
        SEND_OK();
    }
}

/*
 *  +CMGS
 */
static inline
void
sendSMS(int nb, char **args)
{
    int i, len;
    char *sms = args[nb - 1];
    
    printf("Send SMS to %d number(s)\n", nb - 1);
    for (i=0; i<(nb - 1); i++) {
        printf("\t%s\n", args[i]);
    }
    printf("SMS Text = ");
    
    len = strlen(sms);
    for (i=0; i<len; i++) {
        if (sms[i] >= 0x20 && sms[i] <= 0x7E) {
            fwrite(&sms[i], 1, 1, stdout);
        } else {
            fprintf(stdout, "<%02hhx>", sms[i]);
        }
    }
    fwrite("\r\n", 1, 2, stdout);

    modemStatusHandler->smsCount++;
    SEND("\r\n+CMGS: %d\r\nOK\r\n", modemStatusHandler->smsCount);
}

static
void
dispatchCmd(uint32_t cmd, uint32_t nb_args, char **args)
{
    switch(cmd) {
        case CMD_EMPTY:
            break;
    
        case CMD_AT:
            SEND_OK();
            break;
        
        case CMD_PIN_STATUS:
            pincodeValidate();
            break;
        
        case CMD_PIN_UNLOCK:
            pincodeUnlock(args[0]);
            break;
            
        case CMD_PIN_CHANGE:
            pincodeUpdate(args[0], args[1]);
            break;
            
        case CMD_SEND_SMS:
            sendSMS(nb_args, args);
            break;
            
        default:
           SEND_ERROR(); 
    }
}

void
executeCmd(ModemParserCtx_t *ctx, int fd)
{
    uint32_t i;
    char *p;
    char *args[MODEM_MAX_ARG];
    
    _fd = fd;
    
    // Add trailing zero for each arguments
    // It's erase some ',' '\r' '\n' in the buffer
    for (i=0; i<ctx->arg_count; i++) {
        p = PTR_TO_ARG(i);
        p[LEN_ARG(i)] = '\0';
        args[i] = p;
    }
    
    dispatchCmd(ctx->cmd, ctx->arg_count, args);
}
