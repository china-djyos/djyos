#ifndef __CPU_PERI_NAND__
#define __CPU_PERI_NAND__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct NFC_REG
{
    vu32 NFCONF;        //NAND Flash configuration
    vu32 NFCONT;        //NAND Flash control

    vu8  NFCMD;         //NAND Flash command
    vu8  rsv0[3];

    vu8  NFADDR;        //NAND Flash address
    vu8  rsv1[3];

    vu8  NFDATA;        //NAND Flash data
    vu8  rsv2[3];

    vu32 NFMECCD0;      //NAND Flash ECC for Main
    vu32 NFMECCD1;      //NAND Flash ECC for Main
    vu32 NFSECCD;       //NAND Flash ECC for Spare Area
    vu32 NFSBLK;        //NAND Flash programmable start block address
    vu32 NFEBLK;        //NAND Flash programmable end block address
    vu32 NFSTAT;        //NAND Flash operation status
    vu32 NFECCERR0;     //NAND Flash ECC Error Status for I/O [7:0]
    vu32 NFECCERR1;     //NAND Flash ECC Error Status for I/O [15:8]
    vu32 NFMECC0;       //SLC or MLC NAND Flash ECC status
    vu32 NFMECC1;       //SLC or MLC NAND Flash ECC status
    vu32 NFSECC;        //NAND Flash ECC for I/O[15:0]
    vu32 NFMLCBITPT;    //NAND Flash 4-bit ECC Error Pattern for data[7:0]
};

#define NFC_REG_BASE    ((struct NFC_REG*)0x4E000000)

/*
#define NFCONF      0x4E000000      //NAND Flash configuration
#define NFCONT      0x4E000004      //NAND Flash control
#define NFCMD       0x4E000008      //NAND Flash command
#define NFADDR      0x4E00000C      //NAND Flash address
#define NFDATA      0x4E000010      //NAND Flash data
#define NFMECCD0    0x4E000014      //NAND Flash ECC for Main
#define NFMECCD1    0x4E000018      //NAND Flash ECC for Main
#define NFSECCD     0x4E00001C      //NAND Flash ECC for Spare Area
#define NFSBLK      0x4E000020      //NAND Flash programmable start block address
#define NFEBLK      0x4E000024      //NAND Flash programmable end block address
#define NFSTAT      0x4E000028      //NAND Flash operation status
#define NFECCERR0   0x4E00002C      //NAND Flash ECC Error Status for I/O [7:0]
#define NFECCERR1   0x4E000030      //NAND Flash ECC Error Status for I/O [15:8]
#define NFMECC0     0x4E000034      //SLC or MLC NAND Flash ECC status
#define NFMECC1     0x4E000038      //SLC or MLC NAND Flash ECC status
#define NFSECC      0x4E00003C      //NAND Flash ECC for I/O[15:0]
#define NFMLCBITPT  0x4E000040      //NAND Flash 4-bit ECC Error Pattern for data[7:0]
*/
#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_NAND__
