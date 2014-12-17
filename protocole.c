
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

#include "types.h"
#include "cmd.h"

void executeCmd(ModemParserCtx_t *ctx, int fd);

#define LEN(AT, FPC) (FPC - ctx->buffer - ctx->AT)
#define MARK(M,FPC) (ctx->M = (FPC) - ctx->buffer)
#define PTR_TO(F) (ctx->buffer + ctx->F)

/** machine **/

#line 143 "protocole.rl"


/** Data **/

#line 50 "protocole.c"
static const int modem_gsm_start = 34;
static const int modem_gsm_first_final = 34;
static const int modem_gsm_error = 0;

static const int modem_gsm_en_main = 34;
static const int modem_gsm_en_errors = 33;


#line 147 "protocole.rl"

void
ModemParserInit(ModemParserCtx_t *ctx)  {
    int cs = 0;

    
#line 66 "protocole.c"
	{
	cs = modem_gsm_start;
	}

#line 153 "protocole.rl"

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

    
#line 104 "protocole.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 10: goto st35;
		case 13: goto st35;
		case 65: goto st1;
	}
	goto tr44;
tr0:
#line 73 "protocole.rl"
	{
        {goto st33;}
    }
	goto st0;
tr44:
#line 64 "protocole.rl"
	{
        p--; {goto st33;}
    }
	goto st0;
#line 132 "protocole.c"
st0:
cs = 0;
	goto _out;
tr18:
#line 112 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
#line 54 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st35;
tr47:
#line 54 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 160 "protocole.c"
	switch( (*p) ) {
		case 10: goto st35;
		case 13: goto st35;
		case 65: goto st1;
	}
	goto st0;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	if ( (*p) == 84 )
		goto st2;
	goto tr0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 13: goto tr2;
		case 43: goto st3;
	}
	goto tr0;
tr2:
#line 89 "protocole.rl"
	{
        ctx->cmd = CMD_AT;
    }
#line 54 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st36;
tr26:
#line 112 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
#line 97 "protocole.rl"
	{
        ctx->cmd = CMD_PIN_UNLOCK;
    }
#line 54 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st36;
tr30:
#line 112 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
#line 101 "protocole.rl"
	{
        ctx->cmd = CMD_PIN_CHANGE;
    }
#line 54 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st36;
tr40:
#line 93 "protocole.rl"
	{
        ctx->cmd = CMD_PIN_STATUS;
    }
#line 54 "protocole.rl"
	{
        executeCmd(ctx, fd);
        ModemParserInit(ctx);   // Reset the parser
    }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 243 "protocole.c"
	switch( (*p) ) {
		case 10: goto tr47;
		case 13: goto st35;
		case 65: goto st1;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 67 )
		goto st4;
	goto tr0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 77: goto st5;
		case 80: goto st15;
	}
	goto tr0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 71 )
		goto st6;
	goto tr0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 83 )
		goto st7;
	goto tr0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 61 )
		goto st8;
	goto tr0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 43 )
		goto tr10;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto tr0;
tr10:
#line 105 "protocole.rl"
	{
        ctx->cmd = CMD_SEND_SMS;
    }
#line 47 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st9;
tr19:
#line 47 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 316 "protocole.c"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto tr0;
tr11:
#line 105 "protocole.rl"
	{
        ctx->cmd = CMD_SEND_SMS;
    }
#line 47 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st10;
tr20:
#line 47 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 340 "protocole.c"
	switch( (*p) ) {
		case 13: goto tr13;
		case 44: goto tr14;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto tr0;
tr13:
#line 112 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
#line 118 "protocole.rl"
	{
        write(fd, "> ", 2);
    }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 364 "protocole.c"
	switch( (*p) ) {
		case 10: goto tr16;
		case 32: goto tr15;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 65 )
			goto tr15;
	} else
		goto tr15;
	goto tr0;
tr15:
#line 47 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 391 "protocole.c"
	switch( (*p) ) {
		case 26: goto tr18;
		case 32: goto st12;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st12;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st12;
		} else if ( (*p) >= 65 )
			goto st12;
	} else
		goto st12;
	goto tr0;
tr16:
#line 47 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 418 "protocole.c"
	switch( (*p) ) {
		case 26: goto tr18;
		case 32: goto tr15;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 65 )
			goto tr15;
	} else
		goto tr15;
	goto tr0;
tr14:
#line 112 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 447 "protocole.c"
	if ( (*p) == 43 )
		goto tr19;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr20;
	goto tr0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 73 )
		goto st16;
	goto tr0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 78 )
		goto st17;
	goto tr0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 61 )
		goto st18;
	goto tr0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 63 )
		goto st32;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr24;
	goto tr0;
tr24:
#line 47 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 493 "protocole.c"
	switch( (*p) ) {
		case 13: goto tr26;
		case 44: goto tr27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st27;
	goto tr0;
tr27:
#line 112 "protocole.rl"
	{
        ctx->arg[ctx->arg_count] = ctx->mark;
        ctx->arg_len[ctx->arg_count] = LEN(mark, p);
        ctx->arg_count++;
    }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 513 "protocole.c"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto tr0;
tr29:
#line 47 "protocole.rl"
	{
        MARK(mark, p);
    }
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 527 "protocole.c"
	if ( (*p) == 13 )
		goto tr30;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st22;
	goto tr0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 13 )
		goto tr30;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st23;
	goto tr0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 13 )
		goto tr30;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st24;
	goto tr0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 13 )
		goto tr30;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st25;
	goto tr0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 13 )
		goto tr30;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st26;
	goto tr0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 13 )
		goto tr30;
	goto tr0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 13: goto tr26;
		case 44: goto tr27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st28;
	goto tr0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 13: goto tr26;
		case 44: goto tr27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st29;
	goto tr0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 13: goto tr26;
		case 44: goto tr27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st30;
	goto tr0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 13: goto tr26;
		case 44: goto tr27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st31;
	goto tr0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 13: goto tr26;
		case 44: goto tr27;
	}
	goto tr0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 13 )
		goto tr40;
	goto tr0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto tr43;
	}
	goto st33;
tr43:
#line 80 "protocole.rl"
	{
        write(fd, "ERROR\r\n", 7);
        ctx->arg_count = 0;
        {goto st34;}
    }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 657 "protocole.c"
	if ( (*p) == 10 )
		goto tr48;
	goto st0;
tr48:
#line 80 "protocole.rl"
	{
        write(fd, "ERROR\r\n", 7);
        ctx->arg_count = 0;
        {goto st34;}
    }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 673 "protocole.c"
	goto st0;
	}
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
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
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 1: 
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
#line 73 "protocole.rl"
	{
        {goto st33;}
    }
	break;
#line 756 "protocole.c"
	}
	}

	_out: {}
	}

#line 185 "protocole.rl"
    
    ctx->cs = cs;
}
