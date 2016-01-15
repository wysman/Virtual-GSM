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
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "types.h"
#include "cmd.h"

void executeCmd(ModemParserCtx_t *ctx, int fd);

#define LEN(AT, FPC) (FPC - ctx->buffer - ctx->AT)
#define MARK(M,FPC) (ctx->M = (FPC) - ctx->buffer)
#define PTR_TO(F) (ctx->buffer + ctx->F)

/** machine **/
%%{
    machine modem_gsm;

    #
    # Save the position of the current character in the buffer
    #
    action mark {
        MARK(mark, fpc);
    }

    #
    # EOL recieved with a valid payload
    #
    action done {
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }

    #
    # Error at the start of a request,
    # We have maybe lost the start of the request
    # Try to parse new incomming data as request start
    #    
    action syntax_error_initial {
        fhold; fgoto errors;
    }

    #
    # Error during the request,
    # We have maybe lost the end of the previous request
    # Parse new data as new request
    #
    action syntax_error {
        fgoto errors;
    }
    
    #
    # Action execute when receive a EOL, when we are in error state
    #
    action done_errors {
        write(fd, "ERROR\r\n", 7);
        ctx->arg_count = 0;
        fgoto main;
    }

    #
    # Save the current AT command
    #
    action cmd_at {
        ctx->cmd = CMD_AT;
    }

    action cmd_pin_status {
        ctx->cmd = CMD_PIN_STATUS;
    }
    
    action cmd_pin_unlock {
        ctx->cmd = CMD_PIN_UNLOCK;
    }

    action cmd_pin_change {
        ctx->cmd = CMD_PIN_CHANGE;
    }
    
    action cmd_send_sms {
        ctx->cmd = CMD_SEND_SMS;
    }  
    
    #
    # Save argument position in the buffer
    #
    action arg {
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, fpc);
        ctx->arg_count++;
    }
    
    action send_sms_prompt {
        sleep(1);   // Emulate network lag
        tcflush(fd, TCIFLUSH);
        write(fd, "> ", 2);
    }

# grammar
    CR = 0x0D;
    LF = 0x0A;
    FF = 0x0C;
    ctrlz = 0x1A;
    esc = 0x1B;
    phoneNumber = ("+"? digit+) >mark %arg;
    phoneNumberList = phoneNumber ("," phoneNumber)*;
    smsAllowChacarters = (CR | LF | FF | [a-zA-Z0-9] | space);
    
# elements
    CMD_AT = 'AT' %cmd_at CR LF?;
    CMD_PIN_STATUS = "AT+CPIN=?" %cmd_pin_status CR LF?;
    CMD_PIN_UNLOCK = "AT+CPIN=" (digit{1,6}) >mark %arg %cmd_pin_unlock CR LF?;
    CMD_PIN_CHANGE = "AT+CPIN=" (digit{1,6}) >mark %arg "," (digit{1,6}) >mark %arg %cmd_pin_change CR LF?;
    CMD_SEND_SMS = "AT+CMGS=" %cmd_send_sms phoneNumberList (CR >send_sms_prompt LF?) smsAllowChacarters+ >mark %arg ctrlz @done;
    CMDS = (CMD_AT | CMD_PIN_STATUS | CMD_PIN_UNLOCK | CMD_PIN_CHANGE | CMD_SEND_SMS);

# protocole
    main := ( (CR? LF?)* CMDS? @done )+ >err(syntax_error_initial) @err(syntax_error);
    errors := (any -- (CR | LF))* (CR LF?) @done_errors;
}%%

/** Data **/
%% write data;

void
ModemParserInit(ModemParserCtx_t *ctx)  {
    int cs = 0;

    %% write init;

    if (ctx->buffer == NULL) {
        ctx->buffer_size = 1024;
        ctx->buffer = malloc(ctx->buffer_size);
        assert(ctx->buffer != NULL);
    }
    
    ctx->cmd = CMD_EMPTY;
    ctx->arg_count = 0;
    ctx->buffer_len = 0;
    ctx->cs = cs;
}

void
ModemParserExecute(ModemParserCtx_t *ctx, int fd, const char *buffer, size_t len) 
{
    const char *p, *pe, *eof;
    int cs = ctx->cs;

    if ((ctx->buffer_len + len) > ctx->buffer_size) {
        ctx->buffer_size += 1024;
        ctx->buffer = realloc(ctx->buffer, ctx->buffer_size);
        assert(ctx->buffer != NULL);
    }

    strncpy(ctx->buffer + ctx->buffer_len, buffer, len);
    ctx->buffer_len += len;

    p = ctx->buffer + ctx->buffer_len - len;
    pe = ctx->buffer + ctx->buffer_len;

    %% write exec;
    
    ctx->cs = cs;
}
