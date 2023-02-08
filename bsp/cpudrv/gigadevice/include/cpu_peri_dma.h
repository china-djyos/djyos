#ifndef __CPU_PERI_DMA_H__
#define __CPU_PERI_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

//dma位域定义，bo_为位偏移，1位的用位带地址，bb_前缀，多位用掩码，bm_前缀
#define bb_dma1_ch1_ccr_base   (0x42000000 + 0x20008*32)            //位带基址
#define bb_dma1_ch1_ccr_en     (*(vu32*)(bb_dma1_ch1_ccr_base+4*0))
#define bb_dma1_ch1_ccr_tcie   (*(vu32*)(bb_dma1_ch1_ccr_base+4*1))
#define bb_dma1_ch1_ccr_htie   (*(vu32*)(bb_dma1_ch1_ccr_base+4*2))
#define bb_dma1_ch1_ccr_teie   (*(vu32*)(bb_dma1_ch1_ccr_base+4*3))
#define bb_dma1_ch1_ccr_dir    (*(vu32*)(bb_dma1_ch1_ccr_base+4*4))
#define bb_dma1_ch1_ccr_circ   (*(vu32*)(bb_dma1_ch1_ccr_base+4*5))
#define bb_dma1_ch1_ccr_pinc   (*(vu32*)(bb_dma1_ch1_ccr_base+4*6))
#define bb_dma1_ch1_ccr_minc   (*(vu32*)(bb_dma1_ch1_ccr_base+4*7))
#define bb_dma1_ch1_ccr_m2m    (*(vu32*)(bb_dma1_ch1_ccr_base+4*14))
#define bm_dma1_ch1_ccr_psize  0x00000300
#define bm_dma1_ch1_ccr_msize  0x00000c00
#define bm_dma1_ch1_ccr_pl     0x00003000
#define bo_dma1_ch1_ccr_en     0    //1=通道启用
#define bo_dma1_ch1_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma1_ch1_ccr_htie   2    //1=允许半传输中断
#define bo_dma1_ch1_ccr_teie   3    //1=允许传输错误中断
#define bo_dma1_ch1_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma1_ch1_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma1_ch1_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma1_ch1_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma1_ch1_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma1_ch1_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma1_ch1_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma1_ch1_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma1_ch2_ccr_base   (0x42000000 + 0x2001c*32)            //位带基址
#define bb_dma1_ch2_ccr_en     (*(vu32*)(bb_dma1_ch2_ccr_base+4*0))
#define bb_dma1_ch2_ccr_tcie   (*(vu32*)(bb_dma1_ch2_ccr_base+4*1))
#define bb_dma1_ch2_ccr_htie   (*(vu32*)(bb_dma1_ch2_ccr_base+4*2))
#define bb_dma1_ch2_ccr_teie   (*(vu32*)(bb_dma1_ch2_ccr_base+4*3))
#define bb_dma1_ch2_ccr_dir    (*(vu32*)(bb_dma1_ch2_ccr_base+4*4))
#define bb_dma1_ch2_ccr_circ   (*(vu32*)(bb_dma1_ch2_ccr_base+4*5))
#define bb_dma1_ch2_ccr_pinc   (*(vu32*)(bb_dma1_ch2_ccr_base+4*6))
#define bb_dma1_ch2_ccr_minc   (*(vu32*)(bb_dma1_ch2_ccr_base+4*7))
#define bb_dma1_ch2_ccr_m2m    (*(vu32*)(bb_dma1_ch2_ccr_base+4*14))
#define bm_dma1_ch2_ccr_psize  0x00000300
#define bm_dma1_ch2_ccr_msize  0x00000c00
#define bm_dma1_ch2_ccr_pl     0x00003000
#define bo_dma1_ch2_ccr_en     0    //1=通道启用
#define bo_dma1_ch2_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma1_ch2_ccr_htie   2    //1=允许半传输中断
#define bo_dma1_ch2_ccr_teie   3    //1=允许传输错误中断
#define bo_dma1_ch2_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma1_ch2_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma1_ch2_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma1_ch2_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma1_ch2_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma1_ch2_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma1_ch2_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma1_ch2_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma1_ch3_ccr_base   (0x42000000 + 0x20030*32)            //位带基址
#define bb_dma1_ch3_ccr_en     (*(vu32*)(bb_dma1_ch3_ccr_base+4*0))
#define bb_dma1_ch3_ccr_tcie   (*(vu32*)(bb_dma1_ch3_ccr_base+4*1))
#define bb_dma1_ch3_ccr_htie   (*(vu32*)(bb_dma1_ch3_ccr_base+4*2))
#define bb_dma1_ch3_ccr_teie   (*(vu32*)(bb_dma1_ch3_ccr_base+4*3))
#define bb_dma1_ch3_ccr_dir    (*(vu32*)(bb_dma1_ch3_ccr_base+4*4))
#define bb_dma1_ch3_ccr_circ   (*(vu32*)(bb_dma1_ch3_ccr_base+4*5))
#define bb_dma1_ch3_ccr_pinc   (*(vu32*)(bb_dma1_ch3_ccr_base+4*6))
#define bb_dma1_ch3_ccr_minc   (*(vu32*)(bb_dma1_ch3_ccr_base+4*7))
#define bb_dma1_ch3_ccr_m2m    (*(vu32*)(bb_dma1_ch3_ccr_base+4*14))
#define bm_dma1_ch3_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma1_ch3_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma1_ch3_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma1_ch3_ccr_en     0    //1=通道启用
#define bo_dma1_ch3_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma1_ch3_ccr_htie   2    //1=允许半传输中断
#define bo_dma1_ch3_ccr_teie   3    //1=允许传输错误中断
#define bo_dma1_ch3_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma1_ch3_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma1_ch3_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma1_ch3_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma1_ch3_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma1_ch3_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma1_ch3_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma1_ch3_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma1_ch4_ccr_base   (0x42000000 + 0x20044*32)            //位带基址
#define bb_dma1_ch4_ccr_en     (*(vu32*)(bb_dma1_ch4_ccr_base+4*0))
#define bb_dma1_ch4_ccr_tcie   (*(vu32*)(bb_dma1_ch4_ccr_base+4*1))
#define bb_dma1_ch4_ccr_htie   (*(vu32*)(bb_dma1_ch4_ccr_base+4*2))
#define bb_dma1_ch4_ccr_teie   (*(vu32*)(bb_dma1_ch4_ccr_base+4*3))
#define bb_dma1_ch4_ccr_dir    (*(vu32*)(bb_dma1_ch4_ccr_base+4*4))
#define bb_dma1_ch4_ccr_circ   (*(vu32*)(bb_dma1_ch4_ccr_base+4*5))
#define bb_dma1_ch4_ccr_pinc   (*(vu32*)(bb_dma1_ch4_ccr_base+4*6))
#define bb_dma1_ch4_ccr_minc   (*(vu32*)(bb_dma1_ch4_ccr_base+4*7))
#define bb_dma1_ch4_ccr_m2m    (*(vu32*)(bb_dma1_ch4_ccr_base+4*14))
#define bm_dma1_ch4_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma1_ch4_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma1_ch4_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma1_ch4_ccr_en     0    //1=通道启用
#define bo_dma1_ch4_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma1_ch4_ccr_htie   2    //1=允许半传输中断
#define bo_dma1_ch4_ccr_teie   3    //1=允许传输错误中断
#define bo_dma1_ch4_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma1_ch4_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma1_ch4_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma1_ch4_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma1_ch4_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma1_ch4_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma1_ch4_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma1_ch4_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma1_ch5_ccr_base   (0x42000000 + 0x20058*32)            //位带基址
#define bb_dma1_ch5_ccr_en     (*(vu32*)(bb_dma1_ch5_ccr_base+4*0))
#define bb_dma1_ch5_ccr_tcie   (*(vu32*)(bb_dma1_ch5_ccr_base+4*1))
#define bb_dma1_ch5_ccr_htie   (*(vu32*)(bb_dma1_ch5_ccr_base+4*2))
#define bb_dma1_ch5_ccr_teie   (*(vu32*)(bb_dma1_ch5_ccr_base+4*3))
#define bb_dma1_ch5_ccr_dir    (*(vu32*)(bb_dma1_ch5_ccr_base+4*4))
#define bb_dma1_ch5_ccr_circ   (*(vu32*)(bb_dma1_ch5_ccr_base+4*5))
#define bb_dma1_ch5_ccr_pinc   (*(vu32*)(bb_dma1_ch5_ccr_base+4*6))
#define bb_dma1_ch5_ccr_minc   (*(vu32*)(bb_dma1_ch5_ccr_base+4*7))
#define bb_dma1_ch5_ccr_m2m    (*(vu32*)(bb_dma1_ch5_ccr_base+4*14))
#define bm_dma1_ch5_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma1_ch5_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma1_ch5_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma1_ch5_ccr_en     0    //1=通道启用
#define bo_dma1_ch5_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma1_ch5_ccr_htie   2    //1=允许半传输中断
#define bo_dma1_ch5_ccr_teie   3    //1=允许传输错误中断
#define bo_dma1_ch5_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma1_ch5_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma1_ch5_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma1_ch5_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma1_ch5_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma1_ch5_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma1_ch5_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma1_ch5_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma1_ch6_ccr_base   (0x42000000 + 0x2006c*32)            //位带基址
#define bb_dma1_ch6_ccr_en     (*(vu32*)(bb_dma1_ch6_ccr_base+4*0))
#define bb_dma1_ch6_ccr_tcie   (*(vu32*)(bb_dma1_ch6_ccr_base+4*1))
#define bb_dma1_ch6_ccr_htie   (*(vu32*)(bb_dma1_ch6_ccr_base+4*2))
#define bb_dma1_ch6_ccr_teie   (*(vu32*)(bb_dma1_ch6_ccr_base+4*3))
#define bb_dma1_ch6_ccr_dir    (*(vu32*)(bb_dma1_ch6_ccr_base+4*4))
#define bb_dma1_ch6_ccr_circ   (*(vu32*)(bb_dma1_ch6_ccr_base+4*5))
#define bb_dma1_ch6_ccr_pinc   (*(vu32*)(bb_dma1_ch6_ccr_base+4*6))
#define bb_dma1_ch6_ccr_minc   (*(vu32*)(bb_dma1_ch6_ccr_base+4*7))
#define bb_dma1_ch6_ccr_m2m    (*(vu32*)(bb_dma1_ch6_ccr_base+4*14))
#define bm_dma1_ch6_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma1_ch6_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma1_ch6_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma1_ch6_ccr_en     0    //1=通道启用
#define bo_dma1_ch6_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma1_ch6_ccr_htie   2    //1=允许半传输中断
#define bo_dma1_ch6_ccr_teie   3    //1=允许传输错误中断
#define bo_dma1_ch6_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma1_ch6_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma1_ch6_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma1_ch6_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma1_ch6_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma1_ch6_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma1_ch6_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma1_ch6_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma1_ch7_ccr_base   (0x42000000 + 0x20080*32)            //位带基址
#define bb_dma1_ch7_ccr_en     (*(vu32*)(bb_dma1_ch7_ccr_base+4*0))
#define bb_dma1_ch7_ccr_tcie   (*(vu32*)(bb_dma1_ch7_ccr_base+4*1))
#define bb_dma1_ch7_ccr_htie   (*(vu32*)(bb_dma1_ch7_ccr_base+4*2))
#define bb_dma1_ch7_ccr_teie   (*(vu32*)(bb_dma1_ch7_ccr_base+4*3))
#define bb_dma1_ch7_ccr_dir    (*(vu32*)(bb_dma1_ch7_ccr_base+4*4))
#define bb_dma1_ch7_ccr_circ   (*(vu32*)(bb_dma1_ch7_ccr_base+4*5))
#define bb_dma1_ch7_ccr_pinc   (*(vu32*)(bb_dma1_ch7_ccr_base+4*6))
#define bb_dma1_ch7_ccr_minc   (*(vu32*)(bb_dma1_ch7_ccr_base+4*7))
#define bb_dma1_ch7_ccr_m2m    (*(vu32*)(bb_dma1_ch7_ccr_base+4*14))
#define bm_dma1_ch7_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma1_ch7_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma1_ch7_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma1_ch7_ccr_en     0    //1=通道启用
#define bo_dma1_ch7_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma1_ch7_ccr_htie   2    //1=允许半传输中断
#define bo_dma1_ch7_ccr_teie   3    //1=允许传输错误中断
#define bo_dma1_ch7_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma1_ch7_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma1_ch7_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma1_ch7_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma1_ch7_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma1_ch7_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma1_ch7_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma1_ch7_ccr_pl     12   //通道优先级，00最低，11最高


#define bb_dma2_ch1_ccr_base   (0x42000000 + 0x20408*32)            //位带基址
#define bb_dma2_ch1_ccr_en     (*(vu32*)(bb_dma2_ch1_ccr_base+4*0))
#define bb_dma2_ch1_ccr_tcie   (*(vu32*)(bb_dma2_ch1_ccr_base+4*1))
#define bb_dma2_ch1_ccr_htie   (*(vu32*)(bb_dma2_ch1_ccr_base+4*2))
#define bb_dma2_ch1_ccr_teie   (*(vu32*)(bb_dma2_ch1_ccr_base+4*3))
#define bb_dma2_ch1_ccr_dir    (*(vu32*)(bb_dma2_ch1_ccr_base+4*4))
#define bb_dma2_ch1_ccr_circ   (*(vu32*)(bb_dma2_ch1_ccr_base+4*5))
#define bb_dma2_ch1_ccr_pinc   (*(vu32*)(bb_dma2_ch1_ccr_base+4*6))
#define bb_dma2_ch1_ccr_minc   (*(vu32*)(bb_dma2_ch1_ccr_base+4*7))
#define bb_dma2_ch1_ccr_m2m    (*(vu32*)(bb_dma2_ch1_ccr_base+4*14))
#define bm_dma2_ch1_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma2_ch1_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma2_ch1_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma2_ch1_ccr_en     0    //1=通道启用
#define bo_dma2_ch1_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma2_ch1_ccr_htie   2    //1=允许半传输中断
#define bo_dma2_ch1_ccr_teie   3    //1=允许传输错误中断
#define bo_dma2_ch1_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma2_ch1_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma2_ch1_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma2_ch1_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma2_ch1_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma2_ch1_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma2_ch1_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma2_ch1_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma2_ch2_ccr_base   (0x42000000 + 0x2041c*32)           //位带基址
#define bb_dma2_ch2_ccr_en     (*(vu32*)(bb_dma2_ch2_ccr_base+4*0))
#define bb_dma2_ch2_ccr_tcie   (*(vu32*)(bb_dma2_ch2_ccr_base+4*1))
#define bb_dma2_ch2_ccr_htie   (*(vu32*)(bb_dma2_ch2_ccr_base+4*2))
#define bb_dma2_ch2_ccr_teie   (*(vu32*)(bb_dma2_ch2_ccr_base+4*3))
#define bb_dma2_ch2_ccr_dir    (*(vu32*)(bb_dma2_ch2_ccr_base+4*4))
#define bb_dma2_ch2_ccr_circ   (*(vu32*)(bb_dma2_ch2_ccr_base+4*5))
#define bb_dma2_ch2_ccr_pinc   (*(vu32*)(bb_dma2_ch2_ccr_base+4*6))
#define bb_dma2_ch2_ccr_minc   (*(vu32*)(bb_dma2_ch2_ccr_base+4*7))
#define bb_dma2_ch2_ccr_m2m    (*(vu32*)(bb_dma2_ch2_ccr_base+4*14))
#define bm_dma2_ch2_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma2_ch2_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma2_ch2_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma2_ch2_ccr_en     0    //1=通道启用
#define bo_dma2_ch2_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma2_ch2_ccr_htie   2    //1=允许半传输中断
#define bo_dma2_ch2_ccr_teie   3    //1=允许传输错误中断
#define bo_dma2_ch2_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma2_ch2_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma2_ch2_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma2_ch2_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma2_ch2_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma2_ch2_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma2_ch2_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma2_ch2_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma2_ch3_ccr_base   (0x42000000 + 0x20430*32)            //位带基址
#define bb_dma2_ch3_ccr_en     (*(vu32*)(bb_dma2_ch3_ccr_base+4*0))
#define bb_dma2_ch3_ccr_tcie   (*(vu32*)(bb_dma2_ch3_ccr_base+4*1))
#define bb_dma2_ch3_ccr_htie   (*(vu32*)(bb_dma2_ch3_ccr_base+4*2))
#define bb_dma2_ch3_ccr_teie   (*(vu32*)(bb_dma2_ch3_ccr_base+4*3))
#define bb_dma2_ch3_ccr_dir    (*(vu32*)(bb_dma2_ch3_ccr_base+4*4))
#define bb_dma2_ch3_ccr_circ   (*(vu32*)(bb_dma2_ch3_ccr_base+4*5))
#define bb_dma2_ch3_ccr_pinc   (*(vu32*)(bb_dma2_ch3_ccr_base+4*6))
#define bb_dma2_ch3_ccr_minc   (*(vu32*)(bb_dma2_ch3_ccr_base+4*7))
#define bb_dma2_ch3_ccr_m2m    (*(vu32*)(bb_dma2_ch3_ccr_base+4*14))
#define bm_dma2_ch3_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma2_ch3_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma2_ch3_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma2_ch3_ccr_en     0    //1=通道启用
#define bo_dma2_ch3_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma2_ch3_ccr_htie   2    //1=允许半传输中断
#define bo_dma2_ch3_ccr_teie   3    //1=允许传输错误中断
#define bo_dma2_ch3_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma2_ch3_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma2_ch3_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma2_ch3_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma2_ch3_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma2_ch3_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma2_ch3_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma2_ch3_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma2_ch4_ccr_base   (0x42000000 + 0x20444*32)            //位带基址
#define bb_dma2_ch4_ccr_en     (*(vu32*)(bb_dma2_ch4_ccr_base+4*0))
#define bb_dma2_ch4_ccr_tcie   (*(vu32*)(bb_dma2_ch4_ccr_base+4*1))
#define bb_dma2_ch4_ccr_htie   (*(vu32*)(bb_dma2_ch4_ccr_base+4*2))
#define bb_dma2_ch4_ccr_teie   (*(vu32*)(bb_dma2_ch4_ccr_base+4*3))
#define bb_dma2_ch4_ccr_dir    (*(vu32*)(bb_dma2_ch4_ccr_base+4*4))
#define bb_dma2_ch4_ccr_circ   (*(vu32*)(bb_dma2_ch4_ccr_base+4*5))
#define bb_dma2_ch4_ccr_pinc   (*(vu32*)(bb_dma2_ch4_ccr_base+4*6))
#define bb_dma2_ch4_ccr_minc   (*(vu32*)(bb_dma2_ch4_ccr_base+4*7))
#define bb_dma2_ch4_ccr_m2m    (*(vu32*)(bb_dma2_ch4_ccr_base+4*14))
#define bm_dma2_ch4_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma2_ch4_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma2_ch4_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma2_ch4_ccr_en     0    //1=通道启用
#define bo_dma2_ch4_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma2_ch4_ccr_htie   2    //1=允许半传输中断
#define bo_dma2_ch4_ccr_teie   3    //1=允许传输错误中断
#define bo_dma2_ch4_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma2_ch4_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma2_ch4_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma2_ch4_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma2_ch4_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma2_ch4_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma2_ch4_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma2_ch4_ccr_pl     12   //通道优先级，00最低，11最高

#define bb_dma2_ch5_ccr_base   (0x42000000 + 0x20458*32)           //位带基址
#define bb_dma2_ch5_ccr_en     (*(vu32*)(bb_dma2_ch5_ccr_base+4*0))
#define bb_dma2_ch5_ccr_tcie   (*(vu32*)(bb_dma2_ch5_ccr_base+4*1))
#define bb_dma2_ch5_ccr_htie   (*(vu32*)(bb_dma2_ch5_ccr_base+4*2))
#define bb_dma2_ch5_ccr_teie   (*(vu32*)(bb_dma2_ch5_ccr_base+4*3))
#define bb_dma2_ch5_ccr_dir    (*(vu32*)(bb_dma2_ch5_ccr_base+4*4))
#define bb_dma2_ch5_ccr_circ   (*(vu32*)(bb_dma2_ch5_ccr_base+4*5))
#define bb_dma2_ch5_ccr_pinc   (*(vu32*)(bb_dma2_ch5_ccr_base+4*6))
#define bb_dma2_ch5_ccr_minc   (*(vu32*)(bb_dma2_ch5_ccr_base+4*7))
#define bb_dma2_ch5_ccr_m2m    (*(vu32*)(bb_dma2_ch5_ccr_base+4*14))
#define bm_dma2_ch5_ccr_psize  0x00000300  //外设数据宽度，00=8，01=16，10=32
#define bm_dma2_ch5_ccr_msize  0x00000c00  //存储器数据宽度，00=8，01=16，10=32
#define bm_dma2_ch5_ccr_pl     0x00003000  //通道优先级，00最低，11最高
#define bo_dma2_ch5_ccr_en     0    //1=通道启用
#define bo_dma2_ch5_ccr_tcie   1    //1=允许传输完成中断
#define bo_dma2_ch5_ccr_htie   2    //1=允许半传输中断
#define bo_dma2_ch5_ccr_teie   3    //1=允许传输错误中断
#define bo_dma2_ch5_ccr_dir    4    //0=从外设读，1=从存储器读
#define bo_dma2_ch5_ccr_circ   5    //0=执行循环操作，1反之
#define bo_dma2_ch5_ccr_pinc   6    //0=外设地址不增量，1反之
#define bo_dma2_ch5_ccr_minc   7    //0=存储器地址不增量，1反之
#define bo_dma2_ch5_ccr_m2m    14   //0=非存储器到存储器模式，1=启用存储器到存储器模式
#define bo_dma2_ch5_ccr_psize  8    //外设数据宽度，00=8，01=16，10=32
#define bo_dma2_ch5_ccr_msize  10   //存储器数据宽度，00=8，01=16，10=32
#define bo_dma2_ch5_ccr_pl     12   //通道优先级，00最低，11最高
//dma1 channel1:0x40020008
//dma1 channel2:0x4002001c
//dma1 channel3:0x40020030
//dma1 channel4:0x40020044
//dma1 channel5:0x40020058
//dma1 channel6:0x4002006c
//dma1 channel7:0x40020080
//dma2 channel1:0x40020408
//dma2 channel2:0x4002041c
//dma2 channel3:0x40020430
//dma2 channel4:0x40020444
//dma2 channel5:0x40020458
struct DMA_StChannelReg
{
  vu32 CCR;
  vu32 CNDTR;
  vu32 CPAR;
  vu32 CMAR;
};

#define bb_dma1_isr_base    (0x42000000 + 0x20000*32)           //位带基址
#define bb_dma1_isr_gif1    (*(vu32*)(bb_dma1_isr_base+4*0))    //全局中断标志
#define bb_dma1_isr_tcif1   (*(vu32*)(bb_dma1_isr_base+4*1))    //传输完成标志
#define bb_dma1_isr_htif1   (*(vu32*)(bb_dma1_isr_base+4*2))    //半传输完成标志
#define bb_dma1_isr_teif1   (*(vu32*)(bb_dma1_isr_base+4*3))    //传输错误标志
#define bb_dma1_isr_gif2    (*(vu32*)(bb_dma1_isr_base+4*4))    //全局中断标志
#define bb_dma1_isr_tcif2   (*(vu32*)(bb_dma1_isr_base+4*5))    //传输完成标志
#define bb_dma1_isr_htif2   (*(vu32*)(bb_dma1_isr_base+4*6))    //半传输完成标志
#define bb_dma1_isr_teif2   (*(vu32*)(bb_dma1_isr_base+4*7))    //传输错误标志
#define bb_dma1_isr_gif3    (*(vu32*)(bb_dma1_isr_base+4*8))    //全局中断标志
#define bb_dma1_isr_tcif3   (*(vu32*)(bb_dma1_isr_base+4*9))    //传输完成标志
#define bb_dma1_isr_htif3   (*(vu32*)(bb_dma1_isr_base+4*10))   //半传输完成标志
#define bb_dma1_isr_teif3   (*(vu32*)(bb_dma1_isr_base+4*11))   //传输错误标志
#define bb_dma1_isr_gif4    (*(vu32*)(bb_dma1_isr_base+4*12))   //全局中断标志
#define bb_dma1_isr_tcif4   (*(vu32*)(bb_dma1_isr_base+4*13))   //传输完成标志
#define bb_dma1_isr_htif4   (*(vu32*)(bb_dma1_isr_base+4*14))   //半传输完成标志
#define bb_dma1_isr_teif4   (*(vu32*)(bb_dma1_isr_base+4*15))   //传输错误标志
#define bb_dma1_isr_gif5    (*(vu32*)(bb_dma1_isr_base+4*16))   //全局中断标志
#define bb_dma1_isr_tcif5   (*(vu32*)(bb_dma1_isr_base+4*17))   //传输完成标志
#define bb_dma1_isr_htif5   (*(vu32*)(bb_dma1_isr_base+4*18))   //半传输完成标志
#define bb_dma1_isr_teif5   (*(vu32*)(bb_dma1_isr_base+4*19))   //传输错误标志
#define bb_dma1_isr_gif6    (*(vu32*)(bb_dma1_isr_base+4*10))   //全局中断标志
#define bb_dma1_isr_tcif6   (*(vu32*)(bb_dma1_isr_base+4*11))   //传输完成标志
#define bb_dma1_isr_htif6   (*(vu32*)(bb_dma1_isr_base+4*22))   //半传输完成标志
#define bb_dma1_isr_teif6   (*(vu32*)(bb_dma1_isr_base+4*23))   //传输错误标志
#define bb_dma1_isr_gif7    (*(vu32*)(bb_dma1_isr_base+4*24))   //全局中断标志
#define bb_dma1_isr_tcif7   (*(vu32*)(bb_dma1_isr_base+4*25))   //传输完成标志
#define bb_dma1_isr_htif7   (*(vu32*)(bb_dma1_isr_base+4*26))   //半传输完成标志
#define bb_dma1_isr_teif7   (*(vu32*)(bb_dma1_isr_base+4*27))   //传输错误标志

#define bb_dma1_ifcr_base   (0x42000000 + 0x20004*32)           //位带基址
#define bb_dma1_ifcr_cgif1  (*(vu32*)(bb_dma1_ifcr_base+4*0))   //清全局中断标志
#define bb_dma1_ifcr_ctcif1 (*(vu32*)(bb_dma1_ifcr_base+4*1))   //清传输完成标志
#define bb_dma1_ifcr_chtif1 (*(vu32*)(bb_dma1_ifcr_base+4*2))   //清半传输完成标志
#define bb_dma1_ifcr_cteif1 (*(vu32*)(bb_dma1_ifcr_base+4*3))   //清传输错误标志
#define bb_dma1_ifcr_cgif2  (*(vu32*)(bb_dma1_ifcr_base+4*4))   //清全局中断标志
#define bb_dma1_ifcr_ctcif2 (*(vu32*)(bb_dma1_ifcr_base+4*5))   //清传输完成标志
#define bb_dma1_ifcr_chtif2 (*(vu32*)(bb_dma1_ifcr_base+4*6))   //清半传输完成标志
#define bb_dma1_ifcr_cteif2 (*(vu32*)(bb_dma1_ifcr_base+4*7))   //清传输错误标志
#define bb_dma1_ifcr_cgif3  (*(vu32*)(bb_dma1_ifcr_base+4*8))   //清全局中断标志
#define bb_dma1_ifcr_ctcif3 (*(vu32*)(bb_dma1_ifcr_base+4*9))   //清传输完成标志
#define bb_dma1_ifcr_chtif3 (*(vu32*)(bb_dma1_ifcr_base+4*10))  //清半传输完成标志
#define bb_dma1_ifcr_cteif3 (*(vu32*)(bb_dma1_ifcr_base+4*11))  //清传输错误标志
#define bb_dma1_ifcr_cgif4  (*(vu32*)(bb_dma1_ifcr_base+4*12))  //清全局中断标志
#define bb_dma1_ifcr_ctcif4 (*(vu32*)(bb_dma1_ifcr_base+4*13))  //清传输完成标志
#define bb_dma1_ifcr_chtif4 (*(vu32*)(bb_dma1_ifcr_base+4*14))  //清半传输完成标志
#define bb_dma1_ifcr_cteif4 (*(vu32*)(bb_dma1_ifcr_base+4*15))  //清传输错误标志
#define bb_dma1_ifcr_cgif5  (*(vu32*)(bb_dma1_ifcr_base+4*16))  //清全局中断标志
#define bb_dma1_ifcr_ctcif5 (*(vu32*)(bb_dma1_ifcr_base+4*17))  //清传输完成标志
#define bb_dma1_ifcr_chtif5 (*(vu32*)(bb_dma1_ifcr_base+4*18))  //清半传输完成标志
#define bb_dma1_ifcr_cteif5 (*(vu32*)(bb_dma1_ifcr_base+4*19))  //清传输错误标志
#define bb_dma1_ifcr_cgif6  (*(vu32*)(bb_dma1_ifcr_base+4*10))  //清全局中断标志
#define bb_dma1_ifcr_ctcif6 (*(vu32*)(bb_dma1_ifcr_base+4*11))  //清传输完成标志
#define bb_dma1_ifcr_chtif6 (*(vu32*)(bb_dma1_ifcr_base+4*22))  //清半传输完成标志
#define bb_dma1_ifcr_cteif6 (*(vu32*)(bb_dma1_ifcr_base+4*23))  //清传输错误标志
#define bb_dma1_ifcr_cgif7  (*(vu32*)(bb_dma1_ifcr_base+4*24))  //清全局中断标志
#define bb_dma1_ifcr_ctcif7 (*(vu32*)(bb_dma1_ifcr_base+4*25))  //清传输完成标志
#define bb_dma1_ifcr_chtif7 (*(vu32*)(bb_dma1_ifcr_base+4*26))  //清半传输完成标志
#define bb_dma1_ifcr_cteif7 (*(vu32*)(bb_dma1_ifcr_base+4*27))  //清传输错误标志

#define bb_dma2_isr_base    (0x42000000 + 0x20400*32)           //位带基址
#define bb_dma2_isr_gif1    (*(vu32*)(bb_dma2_isr_base+4*0))    //全局中断标志
#define bb_dma2_isr_tcif1   (*(vu32*)(bb_dma2_isr_base+4*1))    //传输完成标志
#define bb_dma2_isr_htif1   (*(vu32*)(bb_dma2_isr_base+4*2))    //半传输完成标志
#define bb_dma2_isr_teif1   (*(vu32*)(bb_dma2_isr_base+4*3))    //传输错误标志
#define bb_dma2_isr_gif2    (*(vu32*)(bb_dma2_isr_base+4*4))    //全局中断标志
#define bb_dma2_isr_tcif2   (*(vu32*)(bb_dma2_isr_base+4*5))    //传输完成标志
#define bb_dma2_isr_htif2   (*(vu32*)(bb_dma2_isr_base+4*6))    //半传输完成标志
#define bb_dma2_isr_teif2   (*(vu32*)(bb_dma2_isr_base+4*7))    //传输错误标志
#define bb_dma2_isr_gif3    (*(vu32*)(bb_dma2_isr_base+4*8))    //全局中断标志
#define bb_dma2_isr_tcif3   (*(vu32*)(bb_dma2_isr_base+4*9))    //传输完成标志
#define bb_dma2_isr_htif3   (*(vu32*)(bb_dma2_isr_base+4*10))   //半传输完成标志
#define bb_dma2_isr_teif3   (*(vu32*)(bb_dma2_isr_base+4*11))   //传输错误标志
#define bb_dma2_isr_gif4    (*(vu32*)(bb_dma2_isr_base+4*12))   //全局中断标志
#define bb_dma2_isr_tcif4   (*(vu32*)(bb_dma2_isr_base+4*13))   //传输完成标志
#define bb_dma2_isr_htif4   (*(vu32*)(bb_dma2_isr_base+4*14))   //半传输完成标志
#define bb_dma2_isr_teif4   (*(vu32*)(bb_dma2_isr_base+4*15))   //传输错误标志
#define bb_dma2_isr_gif5    (*(vu32*)(bb_dma2_isr_base+4*16))   //全局中断标志
#define bb_dma2_isr_tcif5   (*(vu32*)(bb_dma2_isr_base+4*17))   //传输完成标志
#define bb_dma2_isr_htif5   (*(vu32*)(bb_dma2_isr_base+4*18))   //半传输完成标志
#define bb_dma2_isr_teif5   (*(vu32*)(bb_dma2_isr_base+4*19))   //传输错误标志

#define bb_dma2_ifcr_base   (0x42000000 + 0x20404*32)           //位带基址
#define bb_dma2_ifcr_cgif1  (*(vu32*)(bb_dma2_ifcr_base+4*0))   //清全局中断标志
#define bb_dma2_ifcr_ctcif1 (*(vu32*)(bb_dma2_ifcr_base+4*1))   //清传输完成标志
#define bb_dma2_ifcr_chtif1 (*(vu32*)(bb_dma2_ifcr_base+4*2))   //清半传输完成标志
#define bb_dma2_ifcr_cteif1 (*(vu32*)(bb_dma2_ifcr_base+4*3))   //清传输错误标志
#define bb_dma2_ifcr_cgif2  (*(vu32*)(bb_dma2_ifcr_base+4*4))   //清全局中断标志
#define bb_dma2_ifcr_ctcif2 (*(vu32*)(bb_dma2_ifcr_base+4*5))   //清传输完成标志
#define bb_dma2_ifcr_chtif2 (*(vu32*)(bb_dma2_ifcr_base+4*6))   //清半传输完成标志
#define bb_dma2_ifcr_cteif2 (*(vu32*)(bb_dma2_ifcr_base+4*7))   //清传输错误标志
#define bb_dma2_ifcr_cgif3  (*(vu32*)(bb_dma2_ifcr_base+4*8))   //清全局中断标志
#define bb_dma2_ifcr_ctcif3 (*(vu32*)(bb_dma2_ifcr_base+4*9))   //清传输完成标志
#define bb_dma2_ifcr_chtif3 (*(vu32*)(bb_dma2_ifcr_base+4*10))  //清半传输完成标志
#define bb_dma2_ifcr_cteif3 (*(vu32*)(bb_dma2_ifcr_base+4*11))  //清传输错误标志
#define bb_dma2_ifcr_cgif4  (*(vu32*)(bb_dma2_ifcr_base+4*12))  //清全局中断标志
#define bb_dma2_ifcr_ctcif4 (*(vu32*)(bb_dma2_ifcr_base+4*13))  //清传输完成标志
#define bb_dma2_ifcr_chtif4 (*(vu32*)(bb_dma2_ifcr_base+4*14))  //清半传输完成标志
#define bb_dma2_ifcr_cteif4 (*(vu32*)(bb_dma2_ifcr_base+4*15))  //清传输错误标志
#define bb_dma2_ifcr_cgif5  (*(vu32*)(bb_dma2_ifcr_base+4*16))  //清全局中断标志
#define bb_dma2_ifcr_ctcif5 (*(vu32*)(bb_dma2_ifcr_base+4*17))  //清传输完成标志
#define bb_dma2_ifcr_chtif5 (*(vu32*)(bb_dma2_ifcr_base+4*18))  //清半传输完成标志
#define bb_dma2_ifcr_cteif5 (*(vu32*)(bb_dma2_ifcr_base+4*19))  //清传输错误标志

struct DMA_StReg      //dma1 base 0x40020000,dma2 base 0x40020400
{
  vu32 ISR;
  vu32 IFCR;
};

extern struct DMA_StReg volatile * const pg_dma1_reg;
extern struct DMA_StReg volatile * const pg_dma2_reg;

extern struct DMA_StChannelReg volatile * const pg_dma1_channel1_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel2_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel3_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel4_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel5_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel6_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel7_reg;

extern struct DMA_StChannelReg volatile * const pg_dma2_channel1_reg;
extern struct DMA_StChannelReg volatile * const pg_dma2_channel2_reg;
extern struct DMA_StChannelReg volatile * const pg_dma2_channel3_reg;
extern struct DMA_StChannelReg volatile * const pg_dma2_channel4_reg;
extern struct DMA_StChannelReg volatile * const pg_dma2_channel5_reg;

// ============================================================================
#define DMA1_CH0    0
#define DMA1_CH1    1
#define DMA1_CH2    2
#define DMA1_CH3    3
#define DMA1_CH4    4
#define DMA1_CH5    5
#define DMA1_CH6    6
#define DMA1_CH7    7

#define DMA2_CH0    8
#define DMA2_CH1    9
#define DMA2_CH2    10
#define DMA2_CH3    11
#define DMA2_CH4    12
#define DMA2_CH5    13
#define DMA2_CH6    14
#define DMA2_CH7    15

// DMA传输的数据宽度
#define DMA_DATABITS_8  0
#define DMA_DATABITS_16 1
#define DMA_DATABITS_32 2

// DMA数据传输方向
#define DMA_DIR_P2M     0
#define DMA_DIR_M2P     1
#define DMA_DIR_M2M     2

// DMA中断
#define DMA_INT_TCIE   (1<<4)
#define DMA_INT_HTIE   (1<<3)
#define DMA_INT_TEIE   (1<<2)
#define DMA_INT_DMEIE  (1<<1)

// DMA驱动API
//##WIP## u32 DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u8 chx,u32 par,u32 mar,
//##WIP##         u8 dir,u8 msize,u8 psize,u16 ndtr);
//##WIP## void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u32 mar,u16 ndtr);
//##WIP## void DMA_Disable(DMA_Stream_TypeDef *DMA_Streamx);
//##WIP## void DMA_ClearIntFlag(DMA_Stream_TypeDef *DMA_Streamx);
//##WIP## void DMA_IntEnable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt);
//##WIP## void DMA_IntDisable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt);


#ifdef __cplusplus
}
#endif
#endif //__CPU_PERI_DMA_H__

