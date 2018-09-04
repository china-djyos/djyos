#include "cpu-optional.h"
#if (CN_CPU_OPTIONAL_FLOAT_HARD == 1)

.syntax		unified
.cpu		cortex-m7
.thumb
.file	"soft_hard_float.S"
/*
@----汇编传参并执行函数-------------------------------------------------
@功能:
@参数:  ptab，参数列表
@      pflag，参数类型列表
@      old_vm，被复位线程的控制块指针
@返回:  无
@函数原型:extern void __asm_ExecuteCmd(union param *ptab,
@   	enum param_typr *pflag,Ex_shell_func fun,u32 num);
@-----------------------------------------------------------------------------
*/

.text
.align 2
.global __asm_ExecuteCmd
.thumb
.thumb_func
__asm_ExecuteCmd:
	PUSH    {R4-R12,lr}  //保存这几个寄存器可能要用到
	sub sp ,#40
	 /*通过浮点寄存器传递的参数*/
	mov r4 ,#0          //浮点寄存器计数
	mov r5 ,#0          //双精度浮点寄存器计数
	mov r6 ,#0          //参数总数计数
float_double:           //不能保障调用时没有进行浮点计算所以浮点寄存器也要压栈
   /* 4,5,6,7,8,9,10*/
	ldr    r7   ,[r1,r6] //读类型数组
	and    r7   ,r7, #255 //读类型数组
	cmp    r7   ,#9    //判断类型是否为float
	beq    fd_float
	cmp    r7   ,#10    //判断类型是否为double
	beq    fd_double
	b      fd_next

fd_float:
		lsls    r8 ,r6, #3  //根据变量计数得到参数表的偏移    (0/8/16/24/32...) lsls：左移
		ldr     r7 ,[r0,r8] //读形参值数组
		cmp r4  ,#0
		beq    fd_s0
		cmp r4  ,#1
		beq    fd_s1
		cmp r4  ,#2
		beq    fd_s2
		cmp r4  ,#3
		beq    fd_s3
		cmp r4  ,#4
		beq    fd_s4
		cmp r4  ,#5
		beq    fd_s5
		cmp r4  ,#6
		beq    fd_s6
		cmp r4  ,#7
		beq    fd_s7
		cmp r4  ,#8
		beq    fd_s8
		cmp r4  ,#9
		beq    fd_s9
		cmp r4  ,#10
		beq    fd_s10
		cmp r4  ,#11
		beq    fd_s11
		cmp r4  ,#12
		beq    fd_s12
		cmp r4  ,#13
		beq    fd_s13
		cmp r4  ,#14
		beq    fd_s14
		cmp r4  ,#15
		beq    fd_s15
		cmp r4  ,#16
		beq    fd_s16
		cmp r4  ,#17
		beq    fd_s17
		cmp r4  ,#18
		beq    fd_s18
		cmp r4  ,#19
		beq    fd_s19
	fd_s0:
		vmov.f32        s0, r7
		b fd_end
	fd_s1:
		vmov.f32        s1, r7
		b fd_end
	fd_s2:
		vmov.f32        s2, r7
		b fd_end
	fd_s3:
		vmov.f32        s3, r7
		b fd_end
	fd_s4:
		vmov.f32        s4, r7
		b fd_end
	fd_s5:
		vmov.f32       s5, r7
		b fd_end
	fd_s6:
		vmov.f32        s6, r7
		b fd_end
	fd_s7:
		vmov.f32        s7, r7
		b fd_end
	fd_s8:
		vmov.f32        s8, r7
		b fd_end
	fd_s9:
		vmov.f32        s9, r7
		b fd_end
	fd_s10:
		vmov.f32        s10, r7
		b fd_end
	fd_s11:
		vmov.f32        s9, r7
		b fd_end
	fd_s12:
		vmov.f32        s9, r7
		b fd_end
	fd_s13:
		vmov.f32        s9, r7
		b fd_end
	fd_s14:
		vmov.f32        s9, r7
		b fd_end
	fd_s15:
		vmov.f32        s9, r7
		b fd_end
	fd_s16:
		vmov.f32        s9, r7
		b fd_end
	fd_s17:
		vmov.f32        s9, r7
		b fd_end
	fd_s18:
		vmov.f32        s9, r7
		b fd_end
	fd_s19:
		vmov.f32        s9, r7
fd_end:
	add r4   ,#1
	b  fd_next
fd_double:
		lsls    r8 ,r6, #3  //根据变量计数得到参数表的偏移
		add r5 , r4,#1
		lsr r5 ,#1
		cmp r5  ,#0
		beq     fd_d0
		cmp r5  ,#1
		beq     fd_d1
		cmp r5  ,#2
		beq     fd_d2
		cmp r5  ,#3
		beq     fd_d3
		cmp r5  ,#4
		beq     fd_d4
		cmp r5  ,#5
		beq     fd_d5
		cmp r5  ,#6
		beq     fd_d6
		cmp r5  ,#7
		beq     fd_d7
		cmp r5  ,#8
		beq     fd_d8
		cmp r5  ,#9
		beq     fd_d9
		cmp r5  ,#10
		beq     fd_d10
	fd_d0:
		add      R0  ,R8
		vldr     d0, [r0, 0]
		sub      R0  ,R8
		b fd_dend
	fd_d1:
		add      R0  ,R8
		vldr     d1, [r0, 0]
		sub      R0  ,R8
		b fd_dend
	fd_d2:
		add      R0  ,R8
		vldr     d2, [r0, 0]
		sub      R0  ,R8
		b fd_dend
	fd_d3:
		add      R0  ,R8
		vldr     d3, [r0, 0]
		sub      R0  ,R8
		b fd_dend
	fd_d4:
	    add      R0  ,R8
		vldr     d4, [r0, 0]
		sub      R0  ,R8
		b fd_dend
	fd_d5:
	    add      R0  ,R8
		vldr     d5, [r0, 0]
		sub      R0  ,R8
		b fd_dend
	fd_d6:
	    add      R0  ,R8
		vldr     d6, [r0, 0]
		sub      R0  ,R8
		b fd_dend
	fd_d7:
	    add      R0  ,R8
		vldr     d7, [r0, 0]
		sub      R0  ,R8
		b fd_dend
	fd_d8:
		add      R0  ,R8
	    LDR      r11,[R0]
	    LDR      r12,[R0,#4]
	    strd     r11, r12, [sp]
		sub      R0  ,R8
		b fd_dend
	fd_d9:
	    add      R0  ,R8
	    LDR      r11,[R0]
	    LDR      r12,[R0,#4]
	    strd     r11, r12, [sp,#8]
		sub      R0  ,R8
		b fd_dend
fd_d10:
	    add      R0  ,R8
	    LDR      r11,[R0]
	    LDR      r12,[R0,#4]
	    strd     r11, r12, [sp,#16]
		sub      R0  ,R8
	fd_dend:
		add R4  ,#3
		and r4  ,#254
fd_next:
	add    r6   ,#1
	cmp r6 ,r3
	bne float_double

/*浮点参数传递完毕通用寄存器传参*/
	MOV R4 ,#0   //寄存器传参计数器
	MOV R5 ,#0   //低16位压栈大小计数器高16位第一个压栈参数在表中的偏移
	MOV R6 ,#0   //检索参数总数计数

	mov r7 ,r0   //参数表备份
	mov r8 ,r1   //参数类型表备份
	mov r9 ,r2   //函数地址备份
	mov r10,r3   //参数个数            /* 0,1,2,3,4,5,6，11,12*/
	/*R11保存数组类型 */
	/*R12保存当前类型占字节数 */
R0_R3:
	ldr    r11   ,[r8,r6] //读类型数组
	and    r11   ,r11, #255 //读类型数组
	/*获取该类型占字节大小*/
	cmp    r11   ,#0
	beq    size_1
	cmp    r11   ,#1
	beq    size_2
	cmp    r11   ,#2
	beq    size_4
	cmp    r11   ,#3
	beq    size_8
	cmp    r11   ,#4
	beq    size_1
	cmp    r11   ,#5
	beq    size_2
	cmp    r11   ,#6
	beq    size_4
	cmp    r11   ,#7
	beq    size_8
	cmp    r11   ,#8
	beq    size_4
	cmp    r11   ,#9  //判断类型是否为float
	beq    R_next
	cmp    r11   ,#10 //判断类型是否为double
	beq    R_next
	cmp    r11   ,#11
	beq    size_1
	cmp    r11   ,#12
	beq    size_4

	size_8:
		mov r12 ,#8
		b config_R
	size_4:
		mov r12 ,#4
		b config_R
	size_2:
		mov r12 ,#2
		b config_R
	size_1:
		mov r12 ,#1
		b config_R
	config_R:
		cmp    R4   ,#0
		beq    R_R0
		cmp    R4   ,#1
		beq    R_R1
		cmp    R4   ,#2
		beq    R_R2
		cmp    R4   ,#3
		beq    R_R3
		cmp    R5  ,#0
		bne    add_S
		mov    r5, r6, lsl#16  //r5的高16位存放第一个压栈参数在表中的偏移，r6的值没有改变
	add_S:
		//变量压栈4字节对齐 //size =8则+8
		cmp    r12  ,#8
		bne   Stack_4
		add    R5    ,#8
		b  R_next
		R_R0:
			mov    r0, r6, lsl#3
			ldr    r0   ,[r7,r0]
			add    r4   ,#1
			cmp    r12   ,#8
			bne    R_next
			mov    r1, r6, lsl#3
			add    r1   ,#4
			ldr    r1   ,[r7,r1]
			add    r4   ,#1
			b R_next
		R_R1:
			mov    r1, r6, lsl#3
			ldr    r1   ,[r7,r1]
			add    r4   ,#1
			cmp    r12   ,#8
			beq  R_R2
//			bne    R_next
//			mov    r2, r6, lsl#3
//			add    r2   ,#4
//			ldr    r2   ,[r7,r2]
//			add    r4   ,#1
			b R_next
		R_R2:
			mov    r2, r6, lsl#3
			ldr    r2   ,[r7,r2]
			add    r4   ,#1
			cmp    r12   ,#8
			bne    R_next
			mov    r3, r6, lsl#3
			add    r3   ,#4
			ldr    r3   ,[r7,r3]
			add    r4   ,#1
			b  R_next
		R_R3:
			cmp    r12   ,#8
			beq    Stack_8
			mov    r3, r6, lsl#3
			ldr    r3   ,[r7,r3]
			add    r4   ,#1
			b  R_next
Stack_8:
    mov    r5, r6, lsl#16
    add    R5    ,#8
	add    r4   ,#2
	b  R_next
Stack_4:
	add    R5    ,#4
	b  R_next
Stack_2:
	add    R5    ,#2
	b  R_next
Stack_1:
	add    R5    ,#1
R_next:
	add    r6 ,#1
	cmp r6  ,r10
	bne R0_R3

/*
	R5  //低16位压栈大小计数器
		//高16位第一个压栈参数在表中的偏移
	r4 保存源sp地址用于函数返回时使用
	R5压栈偏移
	R6 //检索参数总数计数
	mov r7 ,r0   //参数表备份
	mov r8 ,r1   //参数类型表备份
	mov r9 ,r2   //函数地址备份
	mov r10,r3   //参数个数
	R11 参数类型
	R12:
*/

/*通过栈传递的参数*/
	cmp     r5 ,#0 //无需压栈
	bne    Stacks
	BLX     R9
	add sp ,#40
	POP     {R4-R12,lr}//恢复保存的寄存器
    BX      LR       //返回
Stacks:

	mov     r4 ,sp       //备份栈指针在函数返回时恢复
	LSR     r6 ,r5, #16  //得到第一个压栈参数在表中的偏移  LSR：右移
                         //取出r5高16位的值存放到r6,r5的值不变
	mov   r11 ,r10         //lsl:左移
    mov   r10 ,r11,lsl#16   //r10的高16位放参数的总个数，
    add   r10 ,#0       //r10的低16位初始值为0


	and     r5 ,r5, #255 //取出r5的低8位的值存放到r5,r5的值改变
    add     r5 ,#7
	and     r5 ,#0xfffffff8//调整指针八字节对其。
	add     r5 ,#8
	sub     sp ,r5
	mov     r5 ,#0
loop:
	ldr    r11 ,[r8,r6] //读类型数组
	and    r11   ,r11, #255 //读类型数组
	cmp    r11   ,#0
	beq    S_size_1
	cmp    r11   ,#1
	beq    S_size_2
	cmp    r11   ,#2
	beq    S_size_4
	cmp    r11   ,#3
	beq    S_size_8
	cmp    r11   ,#4
	beq    S_size_1
	cmp    r11   ,#5
	beq    S_size_2
	cmp    r11   ,#6
	beq    S_size_4
	cmp    r11   ,#7
	beq    S_size_8
	cmp    r11   ,#8
	beq    S_size_4
	cmp    r11   ,#9  //判断类型是否为float
	beq    S_next
	cmp    r11   ,#10 //判断类型是否为double
	beq    S_next
	cmp    r11   ,#11
	beq    S_size_1
	cmp    r11   ,#12
	beq    S_size_4

	S_size_8:
		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读数据表
		add     r12 ,#4                  //???
		ldr     r12 ,[r7,r12] //读类型数组
		str     r11, [sp ,r5]
		add     r5,  #4
		str     r12, [sp ,r5]
		add     r5,  #4

		b S_next
	S_size_4:
	S_size_2:
	S_size_1:
	    add    r6 ,#1
        LSR  r12 ,r10, #16  //从r10的高16位读参数总数放到r12
	    cmp  r6 ,r12
	    beq  r6_equal_r12


	   	ldr    r12 ,[r8,r6] //读类型数组
	    and    r12   ,r12, #255 //读类型数组


  	    and    r11 ,r10, #255
  	    cmp r11 ,#0
  	    beq sub_r6_1
  	    cmp r11 ,#1
  	    beq sub_r6_2
  	    cmp r11 ,#2
  	    beq sub_r6_3
  	    cmp r11 ,#3
  	    beq sub_r6_4
  	    cmp r11 ,#4
  	    beq sub_r6_5
  	    cmp r11 ,#5
  	    beq sub_r6_6


sub_r6_1:
   sub    r6 ,#1
    b  cmp_r5
sub_r6_2:
   sub    r6 ,#2
   b  cmp_r5
sub_r6_3:
   sub    r6 ,#3
   b  cmp_r5
sub_r6_4:
   sub    r6 ,#4
   b  cmp_r5
sub_r6_5:
   sub    r6 ,#5
   b  cmp_r5
sub_r6_6:
   sub    r6 ,#6
   b  cmp_r5

cmp_r5:

        cmp r5 ,#0
        beq  u64_x_u64
        cmp r5 ,#8
        beq  u64_x_u64
        cmp r5 ,#16
        beq  u64_x_u64
        cmp r5 ,#24
        beq  u64_x_u64
        cmp r5 ,#32
        beq  u64_x_u64
        cmp r5 ,#40
        beq  u64_x_u64
        cmp r5 ,#48
        beq  u64_x_u64
        cmp r5 ,#56
        beq  u64_x_u64


       b    S_size_4_bak

 u64_x_u64:
	    cmp    r12   ,#9          //float
	    beq   float1_double1
        cmp    r12   ,#10          //double
	    beq  float1_double1
        cmp    r12   ,#3   //u64
	    beq  S_size_8_bak
	    cmp    r12   ,#7  //s64
	    beq  S_size_8_bak
	    b   S_size_4_bak

float1_double1:
        add    R10    ,#1
        and    r11 ,r10, #255
  	   	cmp r11 ,#1
        beq add_r6_1
       	cmp r11 ,#2
        beq add_r6_2
               	cmp r11 ,#3
        beq add_r6_3
               	cmp r11 ,#4
        beq add_r6_4
               	cmp r11 ,#5
        beq add_r6_5
               	cmp r11 ,#6
        beq add_r6_6


add_r6_1:
 add    R6    ,#1
 b   S_size_4
add_r6_2:
 add    R6    ,#2
 b   S_size_4
 add_r6_3:
 add    R6    ,#3
 b   S_size_4
 add_r6_4:
 add    R6    ,#4
 b   S_size_4
 add_r6_5:
 add    R6    ,#5
 b   S_size_4
 add_r6_6:
 add    R6    ,#6
 b   S_size_4



r6_equal_r12:

    and    r11 ,r10, #255
  	   			 cmp r11 ,#0
     	beq sub_r6_1_bak
  	      	    cmp r11 ,#1
  	    beq sub_r6_2_bak
  	      	    cmp r11 ,#2
  	    beq sub_r6_3_bak
  	      	    cmp r11 ,#3
  	    beq sub_r6_4_bak
  	      	    cmp r11 ,#4
  	    beq sub_r6_5_bak
  	      	    cmp r11 ,#5
  	    beq sub_r6_6_bak


sub_r6_1_bak:
   sub    r6 ,#1
    b  S_size_4_bak
sub_r6_2_bak:
   sub    r6 ,#2
   b  S_size_4_bak
sub_r6_3_bak:
   sub    r6 ,#3
   b  S_size_4_bak
sub_r6_4_bak:
   sub    r6 ,#4
   b  S_size_4_bak
sub_r6_5_bak:
   sub    r6 ,#5
   b  S_size_4_bak
sub_r6_6_bak:
   sub    r6 ,#6
   b  S_size_4_bak

S_size_4_bak:


		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读类型数组
		str     r11 ,[sp,r5]
		add   r5,  #4
		b S_next


S_size_8_bak:

 		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读类型数组
		str     r11 ,[sp,r5]
		add   r5,  #8
		b S_next

	S_next:
    and     r10 ,#0xffffff00//r10的低8位清0
	add    r6 ,#1
    LSR  r11 ,r10, #16  //从r10的高16位读参数总数放到r11
	cmp r6  ,r11
	bne loop

call_fun:
	/*调用相应的函数*/
	BLX     R9
	mov   sp ,r4
	add sp ,#40
	POP     {R4-R12,lr}//恢复保存的寄存器
    BX      LR       //返回
.end



#endif  /*CN_CPU_OPTIONAL_FLOAT_HARD*/



#if (CN_CPU_OPTIONAL_FLOAT_SOFT == 1)


.syntax		unified
.cpu		cortex-m7
.thumb
.file	"soft_float.S"
/*
@----汇编传参并执行函数-------------------------------------------------
@功能:
@参数:  ptab，参数列表
@      pflag，参数类型列表
@      old_vm，被复位线程的控制块指针
@返回:  无
@函数原型:extern void __asm_ExecuteCmd(union param *ptab,
@   	enum param_typr *pflag,Ex_shell_func fun,u32 num);
@-----------------------------------------------------------------------------
*/

.text
.align 2
.global __asm_ExecuteCmd
.thumb
.thumb_func
__asm_ExecuteCmd:

	PUSH    {R4-R12,lr}  //保存这几个寄存器可能要用到
/*参数传递的通用寄存器传参*/
	MOV R4 ,#0   //寄存器传参计数器
	MOV R5 ,#0   //低16位压栈大小计数器高16位第一个压栈参数在表中的偏移
	MOV R6 ,#0   //检索参数总数计数

	mov r7 ,r0   //参数表备份
	mov r8 ,r1   //参数类型表备份
	mov r9 ,r2   //函数地址备份
	mov r10,r3   //参数个数            /* 0,1,2,3,4,5,6，11,12*/
	/*R11保存数组类型 */
	/*R12保存当前类型占字节数 */
R0_R3:
	ldr    r11   ,[r8,r6] //读类型数组
	and    r11   ,r11, #255 //读类型数组
	/*获取该类型占字节大小*/
	cmp    r11   ,#0
	beq    size_1
	cmp    r11   ,#1
	beq    size_2
	cmp    r11   ,#2
	beq    size_4
	cmp    r11   ,#3
	beq    size_8
	cmp    r11   ,#4
	beq    size_1
	cmp    r11   ,#5
	beq    size_2
	cmp    r11   ,#6
	beq    size_4
	cmp    r11   ,#7
	beq    size_8
	cmp    r11   ,#8
	beq    size_4
	cmp    r11   ,#9  //判断类型是否为float
	beq    size_4
	cmp    r11   ,#10 //判断类型是否为double
	beq    size_8
	cmp    r11   ,#11
	beq    size_1
	cmp    r11   ,#12
	beq    size_4

	size_8:
		mov r12 ,#8
		b config_R
	size_4:
		mov r12 ,#4
		b config_R
	size_2:
		mov r12 ,#2
		b config_R
	size_1:
		mov r12 ,#1
		b config_R
	config_R:
		cmp    R4   ,#0
		beq    R_R0
		cmp    R4   ,#1
		beq    R_R1
		cmp    R4   ,#2
		beq    R_R2
		cmp    R4   ,#3
		beq    R_R3
		cmp    R5  ,#0
		bne    add_S
		mov    r5, r6, lsl#16
	add_S:
		//变量压栈4字节对齐 //size =8则+8
		cmp    r12  ,#8
		bne   Stack_4
		add    R5    ,#8
		b  R_next
		R_R0:
			mov    r0, r6, lsl#3
			ldr    r0   ,[r7,r0]
			add    r4   ,#1
			cmp    r12   ,#8
			bne    R_next
			mov    r1, r6, lsl#3
			add    r1   ,#4
			ldr    r1   ,[r7,r1]
			add    r4   ,#1
			b R_next
		R_R1:
			mov    r1, r6, lsl#3
			ldr    r1   ,[r7,r1]
			add    r4   ,#1
			cmp    r12   ,#8
			beq   R_R2
//			bne    R_next
//			mov    r2, r6, lsl#3
//			add    r2   ,#4
//			ldr    r2   ,[r7,r2]
//			add    r4   ,#1
			b R_next
		R_R2:
			mov    r2, r6, lsl#3
			ldr    r2   ,[r7,r2]
			add    r4   ,#1
			cmp    r12   ,#8
			bne    R_next
			mov    r3, r6, lsl#3
			add    r3   ,#4
			ldr    r3   ,[r7,r3]
			add    r4   ,#1
			b  R_next
		R_R3:
			cmp    r12   ,#8
			beq    Stack_8
			mov    r3, r6, lsl#3
			ldr    r3   ,[r7,r3]
			add    r4   ,#1
			b  R_next
Stack_8:
    mov    r5, r6, lsl#16
    add    R5    ,#8
	add    r4   ,#2
	b  R_next
Stack_4:
	add    R5    ,#4
	b  R_next
Stack_2:
	add    R5    ,#2
	b  R_next
Stack_1:
	add    R5    ,#1
R_next:
	add    r6 ,#1
	cmp r6  ,r10
	bne R0_R3

/*
	R5  //低16位压栈大小计数器
		//高16位第一个压栈参数在表中的偏移
	r4 保存源sp地址用于函数返回时使用
	R5压栈偏移
	R6 //检索参数总数计数
	mov r7 ,r0   //参数表备份
	mov r8 ,r1   //参数类型表备份
	mov r9 ,r2   //函数地址备份
	mov r10,r3   //参数个数
	R11 参数类型
	R12:
*/

/*通过栈传递的参数*/
	cmp     r5 ,#0 //无需压栈
	bne    Stacks
	BLX     R9
	POP     {R4-R12,lr}//恢复保存的寄存器
    BX      LR       //返回
Stacks:

	mov     r4 ,sp       //备份栈指针在函数返回时恢复
	LSR     r6 ,r5, #16  //得到第一个压栈参数在表中的偏移

    mov   r11 ,r10
    mov   r10 ,r6,lsl#16   //r10的高16位放第一个压栈参数在表中的偏移
    add   r10 ,r11        //r10的低16位放参数个数

	and     r5 ,r5, #255 //读类型数组
	add     r5 ,#7
	and     r5 ,#0xfffffff8//调整指针八字节对其。
	add     r5 ,#8
	sub     sp ,r5
	mov     r5 ,#0
loop:
	ldr    r11 ,[r8,r6] //读类型数组
	and    r11   ,r11, #255 //读类型数组
	cmp    r11   ,#0
	beq    S_size_1
	cmp    r11   ,#1
	beq    S_size_2
	cmp    r11   ,#2
	beq    S_size_4
	cmp    r11   ,#3
	beq    S_size_8
	cmp    r11   ,#4
	beq    S_size_1
	cmp    r11   ,#5
	beq    S_size_2
	cmp    r11   ,#6
	beq    S_size_4
	cmp    r11   ,#7
	beq    S_size_8
	cmp    r11   ,#8
	beq    S_size_4
	cmp    r11   ,#9  //判断类型是否为float
	beq    S_size_4
	cmp    r11   ,#10 //判断类型是否为double
	beq    S_size_8
	cmp    r11   ,#11
	beq    S_size_1
	cmp    r11   ,#12
	beq    S_size_4

	S_size_8:
		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读数据表
		add     r12 ,#4                  //???
		ldr     r12 ,[r7,r12] //读类型数组
		str     r11, [sp ,r5]
		add     r5,  #4
		str     r12, [sp ,r5]
		add     r5,  #4
		b S_next

	S_size_4:
	S_size_2:
	S_size_1:

	    sub    r6 ,#1
	   	ldr    r11 ,[r8,r6] //读类型数组
	    and    r11   ,r11, #255 //读类型数组


	    add    r6 ,#2
	    and    r12 ,r10, #255 //从r10的低16位读参数总数放到r12
	    cmp  r6 ,r12
	    beq  S_size_4_bak
	   	ldr    r12 ,[r8,r6] //读类型数组
	    and    r12   ,r12, #255 //读类型数组

	    cmp    r11   ,#10     //double
	    beq    double_x_double
	    cmp    r11   ,#3      //u64
	    beq    double_x_double
	    cmp    r11   ,#7      //s64
	    beq    double_x_double

       //判断第一个压栈的不是8字节，而是四字节
        LSR  r11 ,r10, #16  //得到第一个压栈参数在表中的偏移
        add  r11 ,#1
        cmp  r6  ,r11
        beq  double_x_double  //判断第一个压栈


        sub    r6 ,#3
        sub   r11 ,#2
        cmp   r6, r11
        beq  S_size_4_bak_bak   //判断第二个压栈
	   	ldr    r11 ,[r8,r6] //读类型数组
	    and    r11   ,r11, #255 //读类型数组
        add    r6 ,#3
        cmp    r11   ,#10     //double    //判断第三个压栈
	    beq    S_size_4_bak
	    cmp    r11   ,#3      //u64
	    beq    S_size_4_bak
	    cmp    r11   ,#7      //s64
	    beq    S_size_4_bak


        LSR  r11 ,r10, #16  //得到第一个压栈参数在表中的偏移
        sub    r6 ,#4
        sub   r11 ,#1
        cmp   r6, r11
        beq  double_x_double_bak   //判断第三个压栈
        ldr    r11 ,[r8,r6] //读类型数组
	    and    r11   ,r11, #255 //读类型数组
        add    r6 ,#4
        cmp    r11   ,#10     //double  //判断第四个压栈
	    beq    double_x_double
	    cmp    r11   ,#3      //u64
	    beq    double_x_double
	    cmp    r11   ,#7      //s64
	    beq    double_x_double


        LSR  r11 ,r10, #16  //得到第一个压栈参数在表中的偏移
        sub    r6 ,#5
        sub   r11 ,#1
        cmp   r6, r11
        beq  S_size_4_bak_bak_bak   //判断第四个压栈
        ldr    r11 ,[r8,r6] //读类型数组
	    and    r11   ,r11, #255 //读类型数组
        add    r6 ,#5
        cmp    r11   ,#10     //double  //判断第五个压栈
	    beq    S_size_4_bak
	    cmp    r11   ,#3      //u64
	    beq    S_size_4_bak
	    cmp    r11   ,#7      //s64
	    beq    S_size_4_bak

        b  double_x_double



	   double_x_double:
	     cmp    r12   ,#10   //double
	     beq  S_size_8_bak
         cmp    r12   ,#3    //u64
	     beq  S_size_8_bak
	     cmp    r12   ,#7    //s64
	     beq  S_size_8_bak
	     b   S_size_4_bak

	  double_x_double_bak:
	     cmp    r12   ,#10   //double
	     beq  S_size_8_bak_bak
         cmp    r12   ,#3    //u64
	     beq  S_size_8_bak_bak
	     cmp    r12   ,#7    //s64
	     beq  S_size_8_bak_bak
	     add   r6 ,#1
	     b   S_size_4_bak_bak

S_size_4_bak:

        sub    r6 ,#1
		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读类型数组
		str     r11 ,[sp,r5]
		add   r5,  #4
		b S_next

S_size_4_bak_bak:

        add    r6 ,#2
		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读类型数组
		str     r11 ,[sp,r5]
		add   r5,  #4
		b S_next

S_size_4_bak_bak_bak:

        add    r6 ,#4
		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读类型数组
		str     r11 ,[sp,r5]
		add   r5,  #4
		b S_next


S_size_8_bak:

		sub    r6 ,#1
 		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读类型数组
		str     r11 ,[sp,r5]
		add   r5,  #4
		mov r12 ,#0
	    str     r12, [sp ,r5]
		add     r5,  #4
		b S_next
S_size_8_bak_bak:

       	add    r6 ,#3
 		lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
		ldr     r11 ,[r7,r12] //读类型数组
		str     r11 ,[sp,r5]
		add   r5,  #4
		mov r12 ,#0
	    str     r12, [sp ,r5]
		add     r5,  #4
		b S_next


	S_next:
	add    r6 ,#1
    and     r11 ,r10, #255 //从r10的低16位读参数总数放到r11
	cmp r6  ,r11
	bne loop

call_fun:
	/*调用相应的函数*/
	BLX     R9
	mov   sp ,r4
	POP     {R4-R12,lr}//恢复保存的寄存器
    BX      LR       //返回
.end


#endif   /*CN_CPU_OPTIONAL_FLOAT_SOFT*/




