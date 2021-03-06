
#line 1 "protocole.rl"
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

#line 146 "protocole.rl"


/** Data **/

#line 51 "protocole.c"
static const int modem_gsm_start = 1;
static const int modem_gsm_first_final = 37;
static const int modem_gsm_error = 0;

static const int modem_gsm_en_main = 1;
static const int modem_gsm_en_errors = 36;


#line 150 "protocole.rl"

void
ModemParserInit(ModemParserCtx_t *ctx)  {
    int cs = 0;

    
#line 67 "protocole.c"
	{
	cs = modem_gsm_start;
	}

#line 156 "protocole.rl"

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

    
#line 105 "protocole.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 10: goto st2;
		case 13: goto st3;
		case 65: goto st4;
	}
	goto tr0;
tr0:
#line 65 "protocole.rl"
	{
        p--; {goto st36;}
    }
#line 74 "protocole.rl"
	{
        {goto st36;}
    }
	goto st0;
tr4:
#line 74 "protocole.rl"
	{
        {goto st36;}
    }
	goto st0;
#line 137 "protocole.c"
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 10: goto st2;
		case 13: goto st3;
		case 65: goto st4;
	}
	goto tr4;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto st2;
	goto tr4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 84 )
		goto st5;
	goto tr4;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 13: goto tr6;
		case 43: goto st6;
	}
	goto tr4;
tr6:
#line 90 "protocole.rl"
	{
        ctx->cmd = CMD_AT;
    }
#line 55 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st37;
tr30:
#line 113 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
#line 98 "protocole.rl"
	{
        ctx->cmd = CMD_PIN_UNLOCK;
    }
#line 55 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st37;
tr34:
#line 113 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
#line 102 "protocole.rl"
	{
        ctx->cmd = CMD_PIN_CHANGE;
    }
#line 55 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st37;
tr44:
#line 94 "protocole.rl"
	{
        ctx->cmd = CMD_PIN_STATUS;
    }
#line 55 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 234 "protocole.c"
	switch( (*p) ) {
		case 10: goto tr48;
		case 13: goto st3;
		case 65: goto st4;
	}
	goto st0;
tr22:
#line 113 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
#line 55 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st38;
tr48:
#line 55 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 265 "protocole.c"
	switch( (*p) ) {
		case 10: goto st2;
		case 13: goto st3;
		case 65: goto st4;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 67 )
		goto st7;
	goto tr4;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 77: goto st8;
		case 80: goto st18;
	}
	goto tr4;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 71 )
		goto st9;
	goto tr4;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 83 )
		goto st10;
	goto tr4;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 61 )
		goto st11;
	goto tr4;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 43 )
		goto tr14;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto tr4;
tr14:
#line 106 "protocole.rl"
	{
        ctx->cmd = CMD_SEND_SMS;
    }
#line 48 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st12;
tr23:
#line 48 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 338 "protocole.c"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto tr4;
tr15:
#line 106 "protocole.rl"
	{
        ctx->cmd = CMD_SEND_SMS;
    }
#line 48 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st13;
tr24:
#line 48 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 362 "protocole.c"
	switch( (*p) ) {
		case 13: goto tr17;
		case 44: goto tr18;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto tr4;
tr17:
#line 113 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
#line 119 "protocole.rl"
	{
        sleep(1);   // Emulate network lag
        tcflush(fd, TCIFLUSH);
        write(fd, "> ", 2);
    }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 388 "protocole.c"
	switch( (*p) ) {
		case 10: goto tr20;
		case 32: goto tr19;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr19;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr19;
		} else if ( (*p) >= 65 )
			goto tr19;
	} else
		goto tr19;
	goto tr4;
tr19:
#line 48 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 415 "protocole.c"
	switch( (*p) ) {
		case 26: goto tr22;
		case 32: goto st15;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st15;
		} else if ( (*p) >= 65 )
			goto st15;
	} else
		goto st15;
	goto tr4;
tr20:
#line 48 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 442 "protocole.c"
	switch( (*p) ) {
		case 26: goto tr22;
		case 32: goto tr19;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr19;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr19;
		} else if ( (*p) >= 65 )
			goto tr19;
	} else
		goto tr19;
	goto tr4;
tr18:
#line 113 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 471 "protocole.c"
	if ( (*p) == 43 )
		goto tr23;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr24;
	goto tr4;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 73 )
		goto st19;
	goto tr4;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 78 )
		goto st20;
	goto tr4;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 61 )
		goto st21;
	goto tr4;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 63 )
		goto st35;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto tr4;
tr28:
#line 48 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 517 "protocole.c"
	switch( (*p) ) {
		case 13: goto tr30;
		case 44: goto tr31;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st30;
	goto tr4;
tr31:
#line 113 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 537 "protocole.c"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr33;
	goto tr4;
tr33:
#line 48 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 551 "protocole.c"
	if ( (*p) == 13 )
		goto tr34;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st25;
	goto tr4;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 13 )
		goto tr34;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st26;
	goto tr4;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 13 )
		goto tr34;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st27;
	goto tr4;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 13 )
		goto tr34;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st28;
	goto tr4;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 13 )
		goto tr34;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st29;
	goto tr4;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 13 )
		goto tr34;
	goto tr4;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 13: goto tr30;
		case 44: goto tr31;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st31;
	goto tr4;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 13: goto tr30;
		case 44: goto tr31;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st32;
	goto tr4;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 13: goto tr30;
		case 44: goto tr31;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st33;
	goto tr4;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 13: goto tr30;
		case 44: goto tr31;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st34;
	goto tr4;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 13: goto tr30;
		case 44: goto tr31;
	}
	goto tr4;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 13 )
		goto tr44;
	goto tr4;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto tr47;
	}
	goto st36;
tr47:
#line 81 "protocole.rl"
	{
        write(fd, "ERROR\r\n", 7);
        ctx->arg_count = 0;
        {goto st1;}
    }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 681 "protocole.c"
	if ( (*p) == 10 )
		goto tr49;
	goto st0;
tr49:
#line 81 "protocole.rl"
	{
        write(fd, "ERROR\r\n", 7);
        ctx->arg_count = 0;
        {goto st1;}
    }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 697 "protocole.c"
	goto st0;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2: 
	case 3: 
	case 4: 
	case 5: 
	case 6: 
	case 7: 
	case 8: 
	case 9: 
	case 10: 
	case 11: 
	case 12: 
	case 13: 
	case 14: 
	case 15: 
	case 16: 
	case 17: 
	case 18: 
	case 19: 
	case 20: 
	case 21: 
	case 22: 
	case 23: 
	case 24: 
	case 25: 
	case 26: 
	case 27: 
	case 28: 
	case 29: 
	case 30: 
	case 31: 
	case 32: 
	case 33: 
	case 34: 
	case 35: 
#line 74 "protocole.rl"
	{
        {goto st36;}
    }
	break;
	case 1: 
#line 65 "protocole.rl"
	{
        p--; {goto st36;}
    }
#line 74 "protocole.rl"
	{
        {goto st36;}
    }
	break;
#line 794 "protocole.c"
	}
	}

	_out: {}
	}

#line 188 "protocole.rl"
    
    ctx->cs = cs;
}
