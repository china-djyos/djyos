#ifndef __KEY_HARD_H__
#define __KEY_HARD_H__

#ifdef __cplusplus
extern "C" {
#endif

bool_t ModuleInstall_Keyboard_TQ2416(const char *dev_name);
bool_t key_hard_init(void);
u32 keyboard_scan(void);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_HARD_H__ */
