#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

struct GPIO_REG{

    vu32 GPACON;                  // Port A - offset 0
    vu32 GPADAT;                  // Data

    vu32 PAD1[2];

    vu32 GPBCON;                  // Port B - offset 0x10
    vu32 GPBDAT;                  // Data
    vu32 GPBUDP;                   // Pull-up disable
    vu32 PAD2;

    vu32 GPCCON;                  // Port C - offset 0x20
    vu32 GPCDAT;                  // Data
    vu32 GPCUDP;                   // Pull-up disable
    vu32 PAD3;

    vu32 GPDCON;                  // Port D - offset 0x30
    vu32 GPDDAT;                  // Data
    vu32 GPDUDP;                   // Pull-up disable
    vu32 PAD4;

    vu32 GPECON;                  // Port E - offset 0x40
    vu32 GPEDAT;                  // Data
    vu32 GPEUDP;                   // Pull-up disable
    vu32 PAD5;

    vu32 GPFCON;                  // Port F - offset 0x50
    vu32 GPFDAT;
    vu32 GPFUDP;
    vu32 PAD6;

    vu32 GPGCON;                  // Port G - offset 0x60
    vu32 GPGDAT;
    vu32 GPGUDP;
    vu32 PAD7;

    vu32 GPHCON;                  // Port H - offset 0x70
    vu32 GPHDAT;
    vu32 GPHUDP;
    vu32 PAD8;

    vu32 MISCCR;                  // misc control reg - offset 0x80
    vu32 DCLKCON;                 // DCLK0/1 control reg

    vu32 EXTINT0;                 // external interrupt control reg 0
    vu32 EXTINT1;                 // external interrupt control reg 1
    vu32 EXTINT2;                 // external interrupt control reg 2

    vu32 EINTFLT0;                // reserved
    vu32 EINTFLT1;                // reserved
    vu32 EINTFLT2;                // external interrupt filter reg 2
    vu32 EINTFLT3;                // external interrupt filter reg 3

    vu32 EINTMASK;                // external interrupt mask reg
    vu32 EINTPEND;                // external interrupt pending reg

    vu32 GSTATUS0;                // external pin status
    vu32 GSTATUS1;                // chip ID
    vu32 GSTATUS2;                // reset status
    vu32 GSTATUS3;                // inform register
    vu32 GSTATUS4;                // inform register

    vu32 DSC0;                  // C0 - added by simon
    vu32 DSC1;
    vu32 DSC2;
    vu32 MSLCON;

    vu32 GPJCON;                    // D0
    vu32 GPJDAT;
    vu32 GPJUDP;
    vu32 PDA9;

    vu32 GPKCON;                    // E0
    vu32 GPKDAT;

    vu32 GPKUDP;

    vu32 PDA10;

    vu32 GPLCON;                    // F0
    vu32 GPLDAT;
    vu32 GPLUDP;
    vu32 PDA11;

    vu32 GPMCON;                    // 100
    vu32 GPMDAT;
    vu32 GPMUDP;
    vu32 PDA12;
};

#define GPIO_REG_BASE   ((struct GPIO_REG*)0x56000000)

#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H__*/
