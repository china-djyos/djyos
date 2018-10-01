/*
 * buffer.h
 */

#ifndef __SILAN_BUFFER_H__
#define __SILAN_BUFFER_H__

typedef struct {
	char *base;
	char *lmt;
	char *rd;
	char *wt;
	int   bytes;
}buf_t;

int buf_init(buf_t *buf, char *base, int bytes);

#define buf_try_read(buf, read_p, bytes)      buf_pre_read(buf, read_p, 0, bytes)
int buf_pre_read(buf_t *buf, char **read_p, int offset, int bytes);
int buf_read(buf_t *buf, int bytes);

#define buf_try_write(buf, write_p, bytes)    buf_pre_write(buf, write_p, 0, bytes)
int buf_pre_write(buf_t *buf, char **write_p, int offset, int bytes);
int buf_write(buf_t *buf, int bytes);

#endif
