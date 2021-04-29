#ifndef __misc_h__
#define __misc_h__

#ifdef __cplusplus
extern "C" {
#endif


s32 getargs(s32 argc, char *argv[],char *string);
s32 string2arg(s32 *argc, char *argv[],char *string);

#ifdef __cplusplus
}
#endif

#endif //__misc_h__


