/*
 * test.c
 *
 *  Created on: 2018年4月19日
 *      Author: cheng
 */

#include <stdint.h>
#include <list.h>
#include <stddef.h>
#include <stdio.h>

#define ADD_TO_EX_SHELL   __attribute__((section(".ex_shell_cmd")))
typedef void (*Ex_shell_func)(void);

#define PARAMETER_MAX     (10)   //最大参数个数限制
static union Parameter{
 u8     data_u8;
 u16    data_u16;
 u32    data_u32;
 u64    data_u64;
 s8     data_s8;
 s16    data_s16;
 s32    data_s32;
 s64    data_s64;
 bool_t data_b;
 float  data_f;
 double data_d;
 char   data_c;
 char * data_pc;
}ParameterTab[PARAMETER_MAX];

static enum ParameterTypr
{
     flag_u8   =  0,
     flag_u16  =  1,
     flag_u32  =  2,
     flag_u64  =  3,
     flag_s8   =  4,
     flag_s16  =  5,
     flag_s32  =  6,
     flag_s64  =  7,
     flag_b    =  8,
     flag_f    =  9,
     flag_d    =  10,
     flag_c    =  11,
     flag_str  =  12,
     flag_error=  13,
}ParameterFlagTab[PARAMETER_MAX];



//======================被调用函数测试====================================
double test_data;
u32 vaule;
void test_fun0(double num1,float num2,u8 num3,u64 num4,u32 num5,u64 num6)
{
    test_data = num1+num2+num3+num4+num5+num6;
    printf("test0     %f \n\r",test_data);
}

void test_fun1(double num1,float num2,u64 num3,u32 num4,u32 num5,u64 num6)
{
    test_data = num1+num2+num3+num4+num5+num6;
    printf("test1     %f \n\r",test_data);
}

void test_fun2(u8 num1,u16 num2,u32 num3,u8 num4,u32 num5,u32 num6,u64 num7)
{
    vaule = num1+num2+num3+num4+num5+num6+num7;
    printf("test2    %d \n\r",vaule);
}


void test_fun3(double num1,double num2,double num3,u32 num4,double num5)
{
    test_data = num1+num2+num3+num4+num5;
    printf("test3    %f \n\r",test_data);
}


void test_fun4(double num1,float num2,u8 num3,u32 num4,u32 num5,s8 num6)
{
    test_data = num1+num2+num3+num4+num5+num6;
    printf("test4   %f \n\r",test_data);
}
void test_fun5(double num1,float num2,u8 num3,u32 num4,double num5,double num6)
{
    test_data = num1+num2+num3+num4+num5+num6;
    printf("test5   %f \n\r",test_data);
}
void test_fun6(double num1,float num2,u8 num3,u32 num4,u32 num5,double num6,double num7)
{
    test_data = num1+num2+num3+num4+num5+num6+num7;
    printf("test6   %d \n\r",(u32)test_data);
}


void test_fun7(u64 num1,u32 num2,u64 num3,u32 num4,u64 num5,u32 num6)
{

    vaule = num1+num2+num3+num4+num5+num6;
    printf("test7   %d \n\r",vaule);

}

void test_fun8(u64 num1,u64 num2,u64 num3,u32 num4,u64 num5)
{
    vaule = num1+num2+num3+num4+num5;
    printf("test8    %d \n\r",vaule);
}

void test_fun9(u64 num1,u32 num2,u8 num3,u32 num4,u64 num5,u64 num6)
{
    vaule = num1+num2+num3+num4+num5+num6;
    printf("test9   %d \n\r",vaule);
}

void test_fun10(u64 num1,u32 num2,u8 num3,u32 num4,u32 num5,u64 num6,u64 num7)
{
    vaule = num1+num2+num3+num4+num5+num6+num7;
    printf("test10   %d \n\r",vaule);
}

void test_fun11(u32 num1,u32 num2,u32 num3,u32 num4,double num5,u32 num6,double num7)
{
    test_data = num1+num2+num3+num4+num5+num6+num7;
    printf("test11   %f \n\r",test_data);
}


void test_fun12(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,u64 num6,double num7,u32 num8,u64 num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test12   %f \n\r",test_data);
}



void test_fun13(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,u32 num6,u32 num7,double num8,double num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test13   %f \n\r",test_data);
}

void test_fun14(u32 num1,u32 num2,u32 num3,u32 num4,double num5,u32 num6,u32 num7,u32 num8,double num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test14   %f \n\r",test_data);
}

void test_fun15(u32 num1,u32 num2,u32 num3,u32 num4,double num5,double num6,u32 num7,u32 num8,u32 num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test15   %f \n\r",test_data);
}

void test_fun16(u32 num1,u32 num2,u32 num3,u32 num4,double num5,u32 num6,u32 num7,double num8,double num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test16   %f \n\r",test_data);
}

void test_fun17(u32 num1,u32 num2,u32 num3,u32 num4,double num5,u8 num6,u8 num7,u8 num8,double num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test17   %f \n\r",test_data);
}

void test_fun18(u32 num1,u32 num2,u32 num3,u32 num4,double num5,u32 num6,u32 num7,u32 num8,u32 num9,double num10)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
    printf("Test18   %f \n\r",test_data);
}


void test_fun19(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,u32 num6,u32 num7,u32 num8,u32 num9,double num10)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
    printf("Test19   %f \n\r",test_data);
}

void test_fun20(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,double num6,u32 num7,u64 num8,u32 num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test20   %f \n\r",test_data);
}

void test_fun21(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,u64 num6,u32 num7,double num8,u32 num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test21   %f \n\r",test_data);
}

void test_fun22(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,double num6,u32 num7,double num8,u32 num9)
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test22   %f \n\r",test_data);
}

void test_fun23(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,double num6,double num7,u64 num8,u32 num9)//num5 占用8字节 ，double型的值不通过压栈传递
{
    test_data = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test23   %f \n\r",test_data);
}

void test_fun24(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,u64 num6)  //num5 占用8字节
{
    vaule = num1+num2+num3+num4+num5+num6;
    printf("Test24  %d \n\r",vaule);
}
void test_fun25(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,u32 num6,u32 num7,u64 num8)  //num7 占用8字节
{
    vaule = num1+num2+num3+num4+num5+num6+num7+num8;
    printf("Test25  %d \n\r",vaule);
}
void test_fun26(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,u32 num6,u32 num7,u32 num8,u32 num9,u64 num10)//num9 占用8字节
{
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
    printf("Test26  %d \n\r",vaule);
}

void test_fun27(u32 num1,u32 num2,u32 num3,u32 num4,u64 num5,u32 num6,u32 num7,u32 num8,u64 num9)//num8 占用8字节
{
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9;
    printf("Test27  %d \n\r",vaule);
}
//======================被调用函数测试====================================
static bool_t AsmExecuteCmd (u8 num, void* fun)
{
#if (_D_FPU_USED == 1)
    extern void __asm_ExecuteCmd(union Parameter *ptab,enum ParameterTypr *pflag,void* fun,u32 num);
    __asm_ExecuteCmd (ParameterTab,ParameterFlagTab,fun,num);
#endif

    return true;
}







void test2(float n8,u16 n16,u32 n32,u64 n64,u64 n66,u8 aa,u16 uu)
{

    double bb;
    aa = n64;
    bb = n66;
    aa = aa/bb;
    n8++;
    if(n8>n16)
        n64 = n66;
    if(n8 == n16)n64 = n66;
//        asm_test();
}



void asm_test(void)
{
    void *p_fun0;
    void *p_fun1,*p_fun2,*p_fun3,*p_fun4,*p_fun5,*p_fun6,*p_fun7,*p_fun8,*p_fun9,*p_fun10,*p_fun11,*p_fun12;
    void *p_fun13,*p_fun14,*p_fun15,*p_fun16,*p_fun17,*p_fun18,*p_fun19,*p_fun20,*p_fun21,*p_fun22,*p_fun23,*p_fun24;
    void *p_fun25,*p_fun26,*p_fun27,*p_fun28,*p_fun29,*p_fun30;
    p_fun0=test_fun0;
    p_fun1=test_fun1;
     p_fun2=test_fun2;
     p_fun3=test_fun3;
     p_fun4=test_fun4;
     p_fun5=test_fun5;
     p_fun6= test_fun6;
     p_fun7=test_fun7;
     p_fun8=test_fun8;
     p_fun9=test_fun9;
     p_fun10= test_fun10;
     p_fun11= test_fun11;
     p_fun12= test_fun12;

     p_fun13= test_fun13;
     p_fun14= test_fun14;
     p_fun15= test_fun15;
     p_fun16= test_fun16;
     p_fun17= test_fun17;
     p_fun18= test_fun18;
     p_fun19= test_fun19;
     p_fun20= test_fun20;
     p_fun21= test_fun21;
     p_fun22= test_fun22;
     p_fun23= test_fun23;
     p_fun24= test_fun24;
     p_fun25= test_fun25;
     p_fun26= test_fun26;
     p_fun27= test_fun27;

     test_fun0(1,2,3,4,5,6);
     test_fun1(1,2,3,4,5,6);
     test_fun2(1,2,3,4,5,6,7);
     test_fun3(1,2,3,4,5);

     test_fun4(1,2,3,4,5,6);
     test_fun5(1,2,3,4,5,6);
     test_fun6(1,2,3,4,5,6,7);

     test_fun7(1,2,3,4,5,6);
     test_fun8(1,2,3,4,5);
     test_fun9(1,2,3,4,5,6);
     test_fun10(1,2,3,4,5,6,7);

     test_fun11(1,2,3,4,5,6,7);
//     test_fun12(1,2,3,4,5,6,7,8,9);

     printf("\r\n");
     test_fun13(1,2,3,4,5,6,7,8,9);
     test_fun14(1,2,3,4,5,6,7,8,9);
     test_fun15(1,2,3,4,5,6,7,8,9);
     test_fun16(1,2,3,4,5,6,7,8,9);
     test_fun17(1,2,3,4,5,6,7,8,9);
     test_fun18(1,2,3,4,5,6,7,8,9,10);
     test_fun19(1,2,3,4,5,6,7,8,9,10);
//     test_fun20(1,2,3,4,5,6,7,8,9);
     test_fun21(1,2,3,4,5,6,7,8,9);//double型的形参值不通过压栈传递 ，而是用d寄存器
     test_fun22(1,2,3,4,5,6,7,8,9);//double型的形参值不通过压栈传递 ，而是用d寄存器
     printf("\r\n");

//还未解决的形参顺序。（既要解决test_fun24~test_fun26的类型，同时也要解决形参中带有double的混合着test_fun24~test_fun26类型的test_fun12/test_fun20/test_fun23）
     test_fun12(1,2,3,4,5,6,7,8,9);  //double型的形参值不通过压栈传递 ，而是用d寄存器,num5占用8字节，num8占用8字节，实际上是test_fun24和test_fun25的类型
     test_fun20(1,2,3,4,5,6,7,8,9);  //double型的形参值不通过压栈传递 ，而是用d寄存器，实际上是test_fun25的类型
     test_fun23(1,2,3,4,5,6,7,8,9);  //num5 占用8字节 ，double型的值不通过压栈传递，实际上是test_fun24的类型
     test_fun24(1,2,3,4,5,6); //num5 占用8字节
     test_fun25(1,2,3,4,5,6,7,8); //num7 占用8字节
     test_fun26(1,2,3,4,5,6,7,8,9,10);//num9 占用8字节
     test_fun27(1,2,3,4,5,6,7,8,9);//num8 占用8字节

     printf("\r\n");
     printf("----------11-------------");
     printf("\r\n\r\n");

     ParameterTab[0].data_d= 1;
     ParameterFlagTab[0] = flag_d;

     ParameterTab[1].data_f = 2;
     ParameterFlagTab[1] = flag_f;


     ParameterTab[2].data_u8 = 3;
     ParameterFlagTab[2] = flag_u8;

     ParameterTab[3].data_u64 = 4;
     ParameterFlagTab[3] = flag_u64;


     ParameterTab[4].data_u32= 5;
     ParameterFlagTab[4] = flag_u32;

     ParameterTab[5].data_u64 = 6;
     ParameterFlagTab[5] = flag_u64;



      AsmExecuteCmd(6,p_fun0);

         ParameterTab[0].data_d= 1;
         ParameterFlagTab[0] = flag_d;

         ParameterTab[1].data_f = 2;
         ParameterFlagTab[1] = flag_f;


         ParameterTab[2].data_u64 = 3;
         ParameterFlagTab[2] = flag_u64;

         ParameterTab[3].data_u32 = 4;
         ParameterFlagTab[3] = flag_u32;


         ParameterTab[4].data_u32= 5;
         ParameterFlagTab[4] = flag_u32;

         ParameterTab[5].data_u64 = 6;
         ParameterFlagTab[5] = flag_u64;



          AsmExecuteCmd(6,p_fun1);





     ParameterTab[0].data_u8= 1;
        ParameterFlagTab[0] = flag_u8;

         ParameterTab[1].data_u16 = 2;
         ParameterFlagTab[1] = flag_u16;


         ParameterTab[2].data_u32 = 3;
         ParameterFlagTab[2] = flag_u32;

         ParameterTab[3].data_u8 = 4;
         ParameterFlagTab[3] = flag_u8;


         ParameterTab[4].data_u32= 5;
         ParameterFlagTab[4] = flag_u32;

         ParameterTab[5].data_u32= 6;
          ParameterFlagTab[5] = flag_u32;

          ParameterTab[6].data_u64= 7;
           ParameterFlagTab[6] = flag_u64;

 AsmExecuteCmd(7,p_fun2);






    ParameterTab[0].data_d= 1;
    ParameterFlagTab[0] = flag_d;

     ParameterTab[1].data_d = 2;
     ParameterFlagTab[1] = flag_d;


     ParameterTab[2].data_d = 3;
     ParameterFlagTab[2] = flag_d;

     ParameterTab[3].data_u32 = 4;
     ParameterFlagTab[3] = flag_u32;

        ParameterTab[4].data_d = 5;
        ParameterFlagTab[4] = flag_d;



     AsmExecuteCmd(5,p_fun3);




     ParameterTab[0].data_d= 1;
     ParameterFlagTab[0] = flag_d;

      ParameterTab[1].data_f = 2;
      ParameterFlagTab[1] = flag_f;


      ParameterTab[2].data_u8 = 3;
      ParameterFlagTab[2] = flag_u8;

      ParameterTab[3].data_u32 = 4;
      ParameterFlagTab[3] = flag_u32;

         ParameterTab[4].data_u32 = 5;
         ParameterFlagTab[4] = flag_u32;

         ParameterTab[5].data_s8 = 6;
         ParameterFlagTab[5] = flag_s8;

      AsmExecuteCmd(6,p_fun4);



      ParameterTab[0].data_d= 1;
      ParameterFlagTab[0] = flag_d;

       ParameterTab[1].data_f = 2;
       ParameterFlagTab[1] = flag_f;


       ParameterTab[2].data_u8 = 3;
       ParameterFlagTab[2] = flag_u8;

       ParameterTab[3].data_u32 = 4;
       ParameterFlagTab[3] = flag_u32;

          ParameterTab[4].data_d = 5;
          ParameterFlagTab[4] = flag_d;

          ParameterTab[5].data_d = 6;
          ParameterFlagTab[5] = flag_d;

       AsmExecuteCmd(6,p_fun5);





       ParameterTab[0].data_d= 1;
       ParameterFlagTab[0] = flag_d;

        ParameterTab[1].data_f = 2;
        ParameterFlagTab[1] = flag_f;


        ParameterTab[2].data_u8 = 3;
        ParameterFlagTab[2] = flag_u8;

        ParameterTab[3].data_u32 = 4;
        ParameterFlagTab[3] = flag_u32;

        ParameterTab[4].data_u32 = 5;
        ParameterFlagTab[4] = flag_u32;

           ParameterTab[5].data_d = 6;
           ParameterFlagTab[5] = flag_d;

           ParameterTab[6].data_d = 7;
           ParameterFlagTab[6] = flag_d;

        AsmExecuteCmd(7,p_fun6);




        ParameterTab[0].data_u64= 1;
            ParameterFlagTab[0] = flag_u64;

             ParameterTab[1].data_u32 = 2;
             ParameterFlagTab[1] = flag_u32;


             ParameterTab[2].data_u64= 3;
             ParameterFlagTab[2] = flag_u64;

             ParameterTab[3].data_u32 = 4;
             ParameterFlagTab[3] = flag_u32;

             ParameterTab[4].data_u64 = 5;
             ParameterFlagTab[4] = flag_u64;

             ParameterTab[5].data_u32 = 6;
             ParameterFlagTab[5] = flag_u32;

             AsmExecuteCmd(6,p_fun7);



            ParameterTab[0].data_u64= 1;
           ParameterFlagTab[0] = flag_u64;

           ParameterTab[1].data_u64 = 2;
           ParameterFlagTab[1] = flag_u64;

           ParameterTab[2].data_u64= 3;
           ParameterFlagTab[2] = flag_u64;

          ParameterTab[3].data_u32 = 4;
          ParameterFlagTab[3] = flag_u32;

           ParameterTab[4].data_u64 = 5;
           ParameterFlagTab[4] = flag_u64;

          AsmExecuteCmd(5,p_fun8);


         ParameterTab[0].data_u64= 1;
         ParameterFlagTab[0] = flag_u64;

         ParameterTab[1].data_u32 = 2;
         ParameterFlagTab[1] = flag_u32;

         ParameterTab[2].data_u8= 3;
         ParameterFlagTab[2] = flag_u8;

         ParameterTab[3].data_u32 = 4;
         ParameterFlagTab[3] = flag_u32;

          ParameterTab[4].data_u64 = 5;
        ParameterFlagTab[4] = flag_u64;

          ParameterTab[5].data_u64 = 6;
       ParameterFlagTab[5] = flag_u64;

         AsmExecuteCmd(6,p_fun9);



         ParameterTab[0].data_u64= 1;
        ParameterFlagTab[0] = flag_u64;

        ParameterTab[1].data_u32 = 2;
        ParameterFlagTab[1] = flag_u32;


         ParameterTab[2].data_u8= 3;
         ParameterFlagTab[2] = flag_u8;

         ParameterTab[3].data_u32 = 4;
         ParameterFlagTab[3] = flag_u32;

       ParameterTab[4].data_u32 = 5;
        ParameterFlagTab[4] = flag_u32;

         ParameterTab[5].data_u64 = 6;
         ParameterFlagTab[5] = flag_u64;

         ParameterTab[6].data_u64 = 7;
         ParameterFlagTab[6] = flag_u64;

         AsmExecuteCmd(7,p_fun10);



       ParameterTab[0].data_u32= 1;
       ParameterFlagTab[0] = flag_u32;

       ParameterTab[1].data_u32 = 2;
       ParameterFlagTab[1] = flag_u32;


        ParameterTab[2].data_u32= 3;
        ParameterFlagTab[2] = flag_u32;

         ParameterTab[3].data_u32 = 4;
         ParameterFlagTab[3] = flag_u32;

         ParameterTab[4].data_d = 5;
         ParameterFlagTab[4] = flag_d;

         ParameterTab[5].data_u32 = 6;
         ParameterFlagTab[5] = flag_u32;

         ParameterTab[6].data_d = 7;
        ParameterFlagTab[6] = flag_d;

         AsmExecuteCmd(7,p_fun11);







       ParameterTab[0].data_u32= 1;
       ParameterFlagTab[0] = flag_u32;

       ParameterTab[1].data_u32 = 2;
       ParameterFlagTab[1] = flag_u32;


      ParameterTab[2].data_u32= 3;
      ParameterFlagTab[2] = flag_u32;

      ParameterTab[3].data_u32 = 4;
     ParameterFlagTab[3] = flag_u32;

       ParameterTab[4].data_u32 = 5;
       ParameterFlagTab[4] = flag_u32;

    ParameterTab[5].data_u64 = 6;
    ParameterFlagTab[5] = flag_u64;

      ParameterTab[6].data_d = 7;
      ParameterFlagTab[6] = flag_d;

      ParameterTab[7].data_u32 = 8;
      ParameterFlagTab[7] = flag_u32;

      ParameterTab[8].data_u64 = 9;
      ParameterFlagTab[8] = flag_u64;

      AsmExecuteCmd(9,p_fun12);




//     test_fun13(1,2,3,4,5,6,7,8,9);

     ParameterTab[0].data_u32= 1;
     ParameterFlagTab[0] = flag_u32;

     ParameterTab[1].data_u32 = 2;
     ParameterFlagTab[1] = flag_u32;


      ParameterTab[2].data_u32= 3;
      ParameterFlagTab[2] = flag_u32;

      ParameterTab[3].data_u32 = 4;
     ParameterFlagTab[3] = flag_u32;

      ParameterTab[4].data_u32 = 5;
      ParameterFlagTab[4] = flag_u32;

     ParameterTab[5].data_u32 = 6;
     ParameterFlagTab[5] = flag_u32;

     ParameterTab[6].data_u32 = 7;
     ParameterFlagTab[6] = flag_u32;

      ParameterTab[7].data_d = 8;
      ParameterFlagTab[7] = flag_d;

      ParameterTab[8].data_d = 9;
      ParameterFlagTab[8] = flag_d;

     AsmExecuteCmd(9,p_fun13);

//     test_fun14(1,2,3,4,5,6,7,8,9);


      ParameterTab[0].data_u32= 1;
      ParameterFlagTab[0] = flag_u32;

      ParameterTab[1].data_u32 = 2;
      ParameterFlagTab[1] = flag_u32;


       ParameterTab[2].data_u32= 3;
      ParameterFlagTab[2] = flag_u32;

      ParameterTab[3].data_u32 = 4;
     ParameterFlagTab[3] = flag_u32;

       ParameterTab[4].data_d = 5;
      ParameterFlagTab[4] = flag_d;

      ParameterTab[5].data_u32 = 6;
    ParameterFlagTab[5] = flag_u32;

      ParameterTab[6].data_u32 = 7;
      ParameterFlagTab[6] = flag_u32;

      ParameterTab[7].data_u32 = 8;
    ParameterFlagTab[7] = flag_u32;

     ParameterTab[8].data_d = 9;
     ParameterFlagTab[8] = flag_d;

      AsmExecuteCmd(9,p_fun14);


//   test_fun15(1,2,3,4,5,6,7,8,9);


   ParameterTab[0].data_u32= 1;
   ParameterFlagTab[0] = flag_u32;

   ParameterTab[1].data_u32 = 2;
   ParameterFlagTab[1] = flag_u32;

    ParameterTab[2].data_u32= 3;
    ParameterFlagTab[2] = flag_u32;

    ParameterTab[3].data_u32 = 4;
    ParameterFlagTab[3] = flag_u32;

    ParameterTab[4].data_d = 5;
    ParameterFlagTab[4] = flag_d;

    ParameterTab[5].data_d = 6;
    ParameterFlagTab[5] = flag_d;

    ParameterTab[6].data_u32 = 7;
    ParameterFlagTab[6] = flag_u32;

    ParameterTab[7].data_u32 = 8;
    ParameterFlagTab[7] = flag_u32;

    ParameterTab[8].data_u32 = 9;
    ParameterFlagTab[8] = flag_u32;

     AsmExecuteCmd(9,p_fun15);


//   test_fun16(1,2,3,4,5,6,7,8,9);


    ParameterTab[0].data_u32= 1;
   ParameterFlagTab[0] = flag_u32;

    ParameterTab[1].data_u32 = 2;
    ParameterFlagTab[1] = flag_u32;


    ParameterTab[2].data_u32= 3;
    ParameterFlagTab[2] = flag_u32;

    ParameterTab[3].data_u32 = 4;
    ParameterFlagTab[3] = flag_u32;

    ParameterTab[4].data_d = 5;
    ParameterFlagTab[4] = flag_d;

    ParameterTab[5].data_u32 = 6;
    ParameterFlagTab[5] = flag_u32;

     ParameterTab[6].data_u32 = 7;
     ParameterFlagTab[6] = flag_u32;

     ParameterTab[7].data_d = 8;
     ParameterFlagTab[7] = flag_d;

     ParameterTab[8].data_d = 9;
     ParameterFlagTab[8] = flag_d;

      AsmExecuteCmd(9,p_fun16);


//   test_fun17(1,2,3,4,5,6,7,8,9);

       ParameterTab[0].data_u32= 1;
       ParameterFlagTab[0] = flag_u32;

       ParameterTab[1].data_u32 = 2;
       ParameterFlagTab[1] = flag_u32;

       ParameterTab[2].data_u32= 3;
       ParameterFlagTab[2] = flag_u32;

       ParameterTab[3].data_u32 = 4;
       ParameterFlagTab[3] = flag_u32;

       ParameterTab[4].data_d = 5;
       ParameterFlagTab[4] = flag_d;

       ParameterTab[5].data_u8 = 6;
       ParameterFlagTab[5] = flag_u8;

       ParameterTab[6].data_u8 = 7;
      ParameterFlagTab[6] = flag_u8;

       ParameterTab[7].data_u8 = 8;
       ParameterFlagTab[7] = flag_u8;

       ParameterTab[8].data_d = 9;
      ParameterFlagTab[8] = flag_d;

      AsmExecuteCmd(9,p_fun17);

//     test_fun18(1,2,3,4,5,6,7,8,9,10);



       ParameterTab[0].data_u32= 1;
       ParameterFlagTab[0] = flag_u32;

       ParameterTab[1].data_u32 = 2;
       ParameterFlagTab[1] = flag_u32;

       ParameterTab[2].data_u32= 3;
       ParameterFlagTab[2] = flag_u32;

       ParameterTab[3].data_u32 = 4;
       ParameterFlagTab[3] = flag_u32;

       ParameterTab[4].data_d = 5;
       ParameterFlagTab[4] = flag_d;

       ParameterTab[5].data_u32 = 6;
       ParameterFlagTab[5] = flag_u32;

       ParameterTab[6].data_u32 = 7;
       ParameterFlagTab[6] = flag_u32;

       ParameterTab[7].data_u32= 8;
       ParameterFlagTab[7] = flag_u32;

      ParameterTab[8].data_u32 = 9;
      ParameterFlagTab[8] = flag_u32;

       ParameterTab[9].data_d = 10;
       ParameterFlagTab[9] = flag_d;
       AsmExecuteCmd(10,p_fun18);

   //      test_fun19(1,2,3,4,5,6,7,8,9,10);


        ParameterTab[0].data_u32= 1;
        ParameterFlagTab[0] = flag_u32;

        ParameterTab[1].data_u32 = 2;
        ParameterFlagTab[1] = flag_u32;

        ParameterTab[2].data_u32= 3;
        ParameterFlagTab[2] = flag_u32;

        ParameterTab[3].data_u32 = 4;
        ParameterFlagTab[3] = flag_u32;

        ParameterTab[4].data_u32 = 5;
        ParameterFlagTab[4] = flag_u32;

        ParameterTab[5].data_u32 = 6;
        ParameterFlagTab[5] = flag_u32;

        ParameterTab[6].data_u32 = 7;
        ParameterFlagTab[6] = flag_u32;

        ParameterTab[7].data_u32= 8;
        ParameterFlagTab[7] = flag_u32;

       ParameterTab[8].data_u32 = 9;
       ParameterFlagTab[8] = flag_u32;

       ParameterTab[9].data_d = 10;
       ParameterFlagTab[9] = flag_d;
      AsmExecuteCmd(10,p_fun19);




        ParameterTab[0].data_u32= 1;
        ParameterFlagTab[0] = flag_u32;

        ParameterTab[1].data_u32 = 2;
        ParameterFlagTab[1] = flag_u32;

        ParameterTab[2].data_u32= 3;
        ParameterFlagTab[2] = flag_u32;

        ParameterTab[3].data_u32 = 4;
        ParameterFlagTab[3] = flag_u32;

        ParameterTab[4].data_u32 = 5;
        ParameterFlagTab[4] = flag_u32;

        ParameterTab[5].data_d = 6;
        ParameterFlagTab[5] = flag_d;

        ParameterTab[6].data_u32 = 7;
        ParameterFlagTab[6] = flag_u32;

        ParameterTab[7].data_u64= 8;
        ParameterFlagTab[7] = flag_u64;

        ParameterTab[8].data_u32 = 9;
        ParameterFlagTab[8] = flag_u32;

      AsmExecuteCmd(9,p_fun20);


       ParameterTab[0].data_u32= 1;
       ParameterFlagTab[0] = flag_u32;

       ParameterTab[1].data_u32 = 2;
       ParameterFlagTab[1] = flag_u32;


       ParameterTab[2].data_u32= 3;
       ParameterFlagTab[2] = flag_u32;

       ParameterTab[3].data_u32 = 4;
       ParameterFlagTab[3] = flag_u32;

       ParameterTab[4].data_u32 = 5;
       ParameterFlagTab[4] = flag_u32;

       ParameterTab[5].data_u64= 6;
       ParameterFlagTab[5] = flag_u64;

       ParameterTab[6].data_u32 = 7;
       ParameterFlagTab[6] = flag_u32;

       ParameterTab[7].data_d= 8;
       ParameterFlagTab[7] = flag_d;

       ParameterTab[8].data_u32 = 9;
       ParameterFlagTab[8] = flag_u32;


      AsmExecuteCmd(9,p_fun21);



       ParameterTab[0].data_u32= 1;
       ParameterFlagTab[0] = flag_u32;

       ParameterTab[1].data_u32 = 2;
       ParameterFlagTab[1] = flag_u32;

       ParameterTab[2].data_u32= 3;
       ParameterFlagTab[2] = flag_u32;

       ParameterTab[3].data_u32 = 4;
       ParameterFlagTab[3] = flag_u32;

       ParameterTab[4].data_u32 = 5;
       ParameterFlagTab[4] = flag_u32;

       ParameterTab[5].data_d= 6;
       ParameterFlagTab[5] = flag_d;

       ParameterTab[6].data_u32 = 7;
       ParameterFlagTab[6] = flag_u32;

       ParameterTab[7].data_d= 8;
       ParameterFlagTab[7] = flag_d;

       ParameterTab[8].data_u32 = 9;
       ParameterFlagTab[8] = flag_u32;

    AsmExecuteCmd(9,p_fun22);




    test_fun23(1,2,3,4,5,6,7,8,9);  //num5 占用8字节 ，double型的值不通过压栈传递，实际上是test_fun24的类型

    ParameterTab[0].data_u32= 1;
    ParameterFlagTab[0] = flag_u32;

    ParameterTab[1].data_u32 = 2;
    ParameterFlagTab[1] = flag_u32;

    ParameterTab[2].data_u32= 3;
    ParameterFlagTab[2] = flag_u32;

    ParameterTab[3].data_u32 = 4;
    ParameterFlagTab[3] = flag_u32;

    ParameterTab[4].data_u32 = 5;
    ParameterFlagTab[4] = flag_u32;

    ParameterTab[5].data_d= 6;
    ParameterFlagTab[5] = flag_d;

    ParameterTab[6].data_d = 7;
    ParameterFlagTab[6] = flag_d;

    ParameterTab[7].data_u64= 8;
    ParameterFlagTab[7] = flag_u64;

    ParameterTab[8].data_u32 = 9;
    ParameterFlagTab[8] = flag_u32;

 AsmExecuteCmd(9,p_fun23);



    test_fun24(1,2,3,4,5,6); //num5 占用8字节


    ParameterTab[0].data_u32= 1;
    ParameterFlagTab[0] = flag_u32;

    ParameterTab[1].data_u32 = 2;
    ParameterFlagTab[1] = flag_u32;

    ParameterTab[2].data_u32= 3;
    ParameterFlagTab[2] = flag_u32;

    ParameterTab[3].data_u32 = 4;
    ParameterFlagTab[3] = flag_u32;

    ParameterTab[4].data_u32 = 5;
    ParameterFlagTab[4] = flag_u32;

    ParameterTab[5].data_u64= 6;
    ParameterFlagTab[5] = flag_u64;


 AsmExecuteCmd(6,p_fun24);


        test_fun25(1,2,3,4,5,6,7,8); //num7 占用8字节

        ParameterTab[0].data_u32= 1;
        ParameterFlagTab[0] = flag_u32;

        ParameterTab[1].data_u32 = 2;
        ParameterFlagTab[1] = flag_u32;

        ParameterTab[2].data_u32= 3;
        ParameterFlagTab[2] = flag_u32;

        ParameterTab[3].data_u32 = 4;
        ParameterFlagTab[3] = flag_u32;

        ParameterTab[4].data_u32 = 5;
        ParameterFlagTab[4] = flag_u32;

        ParameterTab[5].data_u32= 6;
        ParameterFlagTab[5] = flag_u32;

        ParameterTab[6].data_u32 = 7;
        ParameterFlagTab[6] = flag_u32;

        ParameterTab[7].data_u64= 8;
        ParameterFlagTab[7] = flag_u64;


     AsmExecuteCmd(8,p_fun25);


        test_fun26(1,2,3,4,5,6,7,8,9,10);//num9 占用8字节
        ParameterTab[0].data_u32= 1;
        ParameterFlagTab[0] = flag_u32;

        ParameterTab[1].data_u32 = 2;
        ParameterFlagTab[1] = flag_u32;

        ParameterTab[2].data_u32= 3;
        ParameterFlagTab[2] = flag_u32;

        ParameterTab[3].data_u32 = 4;
        ParameterFlagTab[3] = flag_u32;

        ParameterTab[4].data_u32 = 5;
        ParameterFlagTab[4] = flag_u32;

        ParameterTab[5].data_u32= 6;
        ParameterFlagTab[5] = flag_u32;

        ParameterTab[6].data_u32 = 7;
        ParameterFlagTab[6] = flag_u32;

        ParameterTab[7].data_u32= 8;
        ParameterFlagTab[7] = flag_u32;

        ParameterTab[8].data_u32 = 9;
        ParameterFlagTab[8] = flag_u32;

        ParameterTab[9].data_u64 = 10;
        ParameterFlagTab[9] = flag_u64;


     AsmExecuteCmd(10,p_fun26);


        test_fun27(1,2,3,4,5,6,7,8,9);//num8 占用8字节

           ParameterTab[0].data_u32= 1;
           ParameterFlagTab[0] = flag_u32;

           ParameterTab[1].data_u32 = 2;
           ParameterFlagTab[1] = flag_u32;

           ParameterTab[2].data_u32= 3;
           ParameterFlagTab[2] = flag_u32;

           ParameterTab[3].data_u32 = 4;
           ParameterFlagTab[3] = flag_u32;

           ParameterTab[4].data_u64 = 5;
           ParameterFlagTab[4] = flag_u64;

           ParameterTab[5].data_u32= 6;
           ParameterFlagTab[5] = flag_u32;

           ParameterTab[6].data_u32 = 7;
           ParameterFlagTab[6] = flag_u32;

           ParameterTab[7].data_u32= 8;
           ParameterFlagTab[7] = flag_u32;

           ParameterTab[8].data_u64 = 9;
           ParameterFlagTab[8] = flag_u64;

           AsmExecuteCmd(9,p_fun27);
}



//以下是外部shell函数

ADD_TO_EX_SHELL bool_t daa(u8 num1,u16 num2,u32 num3,u64 num4)
{

    u32 vaule;
   vaule = num1+num2+num3+num4;
   printf("test1   %d \n\r",vaule);

   return true;

}


ADD_TO_EX_SHELL  bool_t dbb(double num1,float num2,u8 num3,u64 num4,u32 num5,u64 num6)
{
    double test_data;
//  int test_data;
    test_data = num1+num2+num3+num4+num5+num6;
//  test_data = num3+num4+num5+num6;
    printf("test2    %f \n\r",test_data);
    return true;

}

ADD_TO_EX_SHELL  bool_t dcc(u8 num1,u8 num2,u8 num3,u8 num4,u8 num5,\
        u8 num6,u8 num7,u8 num8,u8 num9,u8 num10)
{

    u32 vaule;
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
      printf("test3   %d \n\r",vaule);

    return true;

}

ADD_TO_EX_SHELL  bool_t ddd(u8 num1,u8 num2,u8 num3,u8 num4,u8 num5)
{

    u32 vaule;
    vaule = num1+num2+num3+num4+num5;
      printf("test4   %d \n\r",vaule);

    return true;

}

ADD_TO_EX_SHELL  bool_t dee(u8 num1,u8 num2,u8 num3,u8 num4,u8 num5,\
        u8 num6,u8 num7,u8 num8,u8 num9)
{

    u32 vaule;
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9;
      printf("test5   %d \n\r",vaule);

    return true;

}

ADD_TO_EX_SHELL  bool_t dff(u16 num1,u16 num2,u16 num3,u16 num4,u16 num5,\
        u16 num6,u16 num7,u16 num8,u16 num9,u16 num10)
{

    u32 vaule;
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
      printf("test6  %d \n\r",vaule);

    return true;

}

ADD_TO_EX_SHELL  bool_t dgg(u32 num1,u32 num2,u32 num3,u32 num4,u32 num5,\
        u32 num6,u32 num7,u32 num8,u32 num9,u32 num10)
{

    u32 vaule;
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
      printf("test7  %d \n\r",vaule);

    return true;

}
ADD_TO_EX_SHELL  bool_t dhh(u64 num1,u64 num2,u64 num3,u64 num4,u64 num5,\
        u64 num6,u64 num7,u64 num8,u64 num9,u64 num10)
{

    u32 vaule;//////
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
      printf("test8  %d \n\r",vaule);

    return true;

}

ADD_TO_EX_SHELL  bool_t dii(float num1,float num2,float num3,float num4,float num5,\
        float num6,float num7,float num8,float num9,float num10)
{

    float vaule;
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
      printf("test9  %f \n\r",vaule);

    return true;

}
ADD_TO_EX_SHELL  bool_t djj(double num1,double num2,double num3,double num4,double num5,\
        double num6,double num7,double num8,double num9,double num10)
{

    double vaule;
    vaule = num1+num2+num3+num4+num5+num6+num7+num8+num9+num10;
      printf("test10  %f \n\r",vaule);

    return true;

}

ADD_TO_EX_SHELL  bool_t dkk(double num1,double num2,double num3,\
        double num4)
{

    double vaule;
    vaule = num1+num2+num3+num4;
      printf("test11  %f \n\r",vaule);

    return true;

}

ADD_TO_EX_SHELL bool_t dll(u64 num1,u32 num2,u64 num3,u32 num4,u64 num5,u32 num6)
{
    u32 vaule;
    vaule = num1+num2+num3+num4+num5+num6;
    printf("test12   %d \n\r",vaule);
    return true;
}















