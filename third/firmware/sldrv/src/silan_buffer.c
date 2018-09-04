/*
 * buffer.c
 */

#define MODULE_NAME           "BUFFER"
//#define __DEBUG__

#ifndef NULL
#define NULL   0
#endif

#include "silan_buffer.h"
#include "silan_printf.h"

static inline int get_bytes(buf_t *buf, char *rd, char *wt)
{
	return ((wt - rd) < 0) ? ((wt - rd) + buf->bytes) : (wt - rd);
}

static inline int get_cont_read_bytes(buf_t *buf, char *rd, char *wt)
{
	if (rd == wt) {
		return 0;
	} else if (rd < wt) {
		return (int)(wt - rd);
	} else /*if (rd > wt)*/{
		return (int)(buf->lmt - rd);
	}
}

static inline int get_cont_write_bytes(buf_t *buf, char *rd, char *wt)
{
	if (rd < wt) {
		if (rd == buf->base) {
			return (int)(buf->lmt - wt - 1);
		} else {
			return (int)(buf->lmt - wt);
		}
	} else if (rd > wt) {
		return (int)(rd - wt - 1);
	} else if (rd == wt) {
		if(rd == buf->base) {
			return (int)(buf->lmt - wt - 1);
		} else {
			return (int)(buf->lmt - wt);
		}
	}
}

int buf_pre_read(buf_t *buf, char **read_p, int offset, int bytes)
{
	char *rd, *wt;
	int  cont_read_bytes;
	wt = buf->wt;
	rd = buf->rd;
	cont_read_bytes = get_cont_read_bytes(buf, rd, wt);

	if (!cont_read_bytes) {
		*read_p = NULL;
		return 0;
	} else if ((offset+bytes) > get_bytes(buf, rd, wt)) {
		*read_p = NULL;
		return 0;
	} else if ((offset+bytes) > cont_read_bytes) {
		if (offset >= cont_read_bytes) {
			offset -= cont_read_bytes;
			rd = buf->base + offset;
			cont_read_bytes = get_cont_read_bytes(buf, rd, wt);
			*read_p = rd;
			return (bytes <= cont_read_bytes) ? bytes : cont_read_bytes;
		} else {
			*read_p = rd + offset;
			return cont_read_bytes - offset;
		}
	} else {
		*read_p = rd + offset;
		return bytes;
	}
}

int buf_read(buf_t *buf, int bytes)
{
	char *rd, *wt;
	int  cont_read_bytes;
	rd = buf->rd;
	wt = buf->wt;
	cont_read_bytes = get_cont_read_bytes(buf, rd, wt);

	if (bytes <= cont_read_bytes) {
		//SL_ASSERT((rd+bytes) <= buf->lmt);
		buf->rd = ((rd+bytes) >= buf->lmt) ? buf->base : (rd+bytes);
		return bytes;
	} else {
		//SL_DBGLOG_IRQ("buf(%p)rd: %d, %d, %p, %p", \
				buf, bytes, cont_read_bytes, rd, wt);
		return 0;
	}
}

int buf_pre_write(buf_t *buf, char **write_p, int offset, int bytes)
{
	char *rd, *wt;
	int  cont_write_bytes, remains;
	wt = buf->wt;
	rd = buf->rd;
	//SL_DBGLOG("bytes=%d,wt=%p,rd=%p", bytes, wt, rd);
	cont_write_bytes = get_cont_write_bytes(buf, rd, wt);
	remains = buf->bytes - get_bytes(buf, rd, wt);

	if (!cont_write_bytes) {
		*write_p = NULL;
		return 0;
	} else if ((offset+bytes) >= remains) {
		*write_p = NULL;
		return 0;
	} else if ((offset+bytes) > cont_write_bytes) {
		if (offset >= cont_write_bytes) {
			offset -= cont_write_bytes;
			wt = buf->base + offset;
			cont_write_bytes = get_cont_read_bytes(buf, rd, wt);
			*write_p = wt;
			return (bytes <= cont_write_bytes) ? bytes : cont_write_bytes;
		} else {
			*write_p = wt + offset;
			return cont_write_bytes - offset;
		}
	} else {
		*write_p = wt + offset;
		return bytes;
	}
}

int buf_write(buf_t *buf, int bytes)
{
	char *rd, *wt;
	int cont_write_bytes;
	wt = buf->wt;
	rd = buf->rd;
	cont_write_bytes = get_cont_write_bytes(buf, rd, wt);

	if (bytes <= cont_write_bytes) {
		//SL_ASSERT((wt+bytes) <= buf->lmt);
		buf->wt = ((wt+bytes) >= buf->lmt) ? buf->base : (wt+bytes);
		return bytes;
	} else {
		//SL_DBGLOG_IRQ("buf(%p)wt: %d, %d, %p, %p", \
				buf, bytes, cont_write_bytes, rd, wt);
		return 0;
	}
}

int buf_init(buf_t *buf, char *base, int bytes)
{
	if (((int)base & 0x3) || (bytes <= 1)) { 
		return -1; 
	}

	buf->base  = buf->rd = buf->wt = base;
	buf->lmt   = base + bytes;
	buf->bytes = bytes;
	
	SL_DBGLOG("base=%p,byte=%d", buf->base, buf->bytes);
	return 0;
}

