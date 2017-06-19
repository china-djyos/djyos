/* isatty.c */

#include <unistd.h>

int
_DEFUN(isatty, (fd), int fd)
{
  return _isatty (fd);
}
