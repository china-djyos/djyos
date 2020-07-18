#ifndef BOARD_H_
#define BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CTRL_LED1               IO_CONFIG_PA1
#define CTRL_LED2               IO_CONFIG_PA3
#define CTRL_LED3                               IO_CONFIG_PA6
#define CTRL_MUTE                               IO_CONFIG_PD10
#define CTRL_IR                                 IO_CONFIG_PA2

extern void Board_Init(void);

#ifdef __cplusplus
}
#endif

#endif //BOARD_H_
