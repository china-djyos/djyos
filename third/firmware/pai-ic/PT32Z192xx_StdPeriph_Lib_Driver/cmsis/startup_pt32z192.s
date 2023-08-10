;/*-----------------------------------------------------------------------------------------------------------
;  File Name        : startup.s
;  Version          : V1.00
;  Date             : 05/07/2019
;  Description      : Startup code.
;-----------------------------------------------------------------------------------------------------------*/
;/* <<< Use Configuration Wizard in Context Menu >>>                                                        */

; Amount of memory (in bytes) allocated for Stack and Heap
; Tailor those values to your application needs
;//   <o> Stack Size (in Bytes, must 8 byte aligned) <0-16384:8>
Stack_Size          EQU     1024

                    AREA    STACK, NOINIT, READWRITE, ALIGN = 3
Stack_Mem           SPACE   Stack_Size
__initial_sp

;//   <o>  Heap Size (in Bytes) <0-16384:8>
Heap_Size           EQU     100

                    AREA    HEAP, NOINIT, READWRITE, ALIGN = 3
__heap_base
Heap_Mem            SPACE   Heap_Size
__heap_limit

                    PRESERVE8
                    THUMB

;*******************************************************************************
; Fill-up the Vector Table entries with the exceptions ISR address
;*******************************************************************************
                    AREA    RESET, DATA, READONLY
                    EXPORT  __Vectors
                    EXPORT  __Vectors_End
                    EXPORT  __Vectors_Size
						
__Vectors           DCD  __initial_sp                       ; Top address of Stack
                    DCD  Reset_Handler                      ; Reset Handler
                    DCD  NMI_Handler                        ; NMI Handler
                    DCD  HardFault_Handler                  ; Hard Fault Handler
                    DCD  MemManage_Handler                  ; MPU Fault Handler 
                    DCD  BusFault_Handler                   ; Bus Fault Handler
                    DCD  UsageFault_Handler                 ; Usage Fault Handler
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  SVC_Handler                        ; SVC Handler
                    DCD  DebugMon_Handler                   ; Debug Monitor Handler
                    DCD  0                                  ; Reserved
                    DCD  PendSV_Handler                     ; PendSV Handler
                    DCD  SysTick_Handler                    ; SysTick Handler

                    ; External Interrupt Handler
                    DCD  WDG_Handler						;  16+ 0:  WDG_Handler
                    DCD  0									;  16+ 1:  Not Used
                    DCD  0									;  16+ 2:  Not Used
                    DCD  RTC_Handler						;  16+ 3:  RTC_Handler
                    DCD  FLASH_Handler						;  16+ 4:  FLASH_Handler
                    DCD  RCC_Handler						;  16+ 5:  RCC_Handler
                    DCD  PA_Handler							;  16+ 6:  PA_Handler
                    DCD  PB_Handler							;  16+ 7:  PB_Handler
                    DCD  PC_Handler							;  16+ 8:  PC_Handler
                    DCD  PD_Handler							;  16+ 9:  PD_Handler
                    DCD  0									;  16+ 10: Not Used
                    DCD  DMA_Handler						;  16+ 11: DMA_Handler
                    DCD  0                                  ;  16+ 12: Not Used
                    DCD  0                                  ;  16+ 13: Not Used
                    DCD  0                                  ;  16+ 14: Not Used
                    DCD  0                                  ;  16+ 15: Not Used
                    DCD  0                                  ;  16+ 16: Not Used
                    DCD  0                                  ;  16+ 17: Not Used
                    DCD  ADC_Handler
                    DCD  0                                  ;  16+ 19: Not Used
                    DCD  0                                  ;  16+ 20: Not Used
                    DCD  0                                  ;  16+ 21: Not Used
                    DCD  0                                  ;  16+ 22: Not Used
                    DCD  0                                  ;  16+ 23: Not Used
                    DCD  TMR0_Handler                       ;  16+ 24: TMR0_Handler
                    DCD  TMR1_Handler                       ;  16+ 25: TMR1_Handler
                    DCD  0                                  ;  16+ 26: Not Used
                    DCD  0                                  ;  16+ 27: Not Used
                    DCD  TMR2_Handler                       ;  16+ 28: TMR2_Handler
                    DCD  TMR3_Handler                       ;  16+ 29: TMR3_Handler
                    DCD  TMR4_Handler                       ;  16+ 30: TMR4_Handler
                    DCD  I2C_Handler                        ;  16+ 31: I2C_Handler
                    DCD  0                                  ;  16+ 32: Not Used
                    DCD  0                                  ;  16+ 33: Not Used
                    DCD  0                                  ;  16+ 34: Not Used
                    DCD  SPI0_Handler                       ;  16+ 35: SPI0_Handler 
                    DCD  SPI1_Handler                       ;  16+ 36: SPI1_Handler
                    DCD  UART0_Handler                      ;  16+ 37: UART0_Handler
                    DCD  UART1_Handler                      ;  16+ 38: UART1_Handler
                    DCD  UART2_Handler                      ;  16+ 39: UART2_Handler
                    DCD  0	
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  USB11_Handler
                    DCD  0
                    DCD  QSPI_Handler
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  0
                    DCD  TMR5_Handler
                    DCD  0		
	                DCD  AES_Handler					
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors
	
	            AREA    |.text|, CODE, READONLY
					
; Reset handler routine
Reset_Handler       PROC
                    EXPORT  Reset_Handler                   [WEAK]
                    IMPORT  __main
                    IMPORT  SystemInit
					LDR     R0, =SystemInit
                    BLX     R0
                    LDR     R0, =__main
                    BX      R0
                    ENDP

; Dummy Exception Handlers
NMI_Handler         PROC
                    EXPORT  NMI_Handler                     [WEAK]
                    B       .
                    ENDP

HardFault_Handler   PROC
                    EXPORT  HardFault_Handler               [WEAK]
                    B       .
                    ENDP

MemManage_Handler   PROC
                    EXPORT  MemManage_Handler               [WEAK]
                    B       .
                    ENDP

BusFault_Handler   PROC
                    EXPORT  BusFault_Handler                [WEAK]
                    B       .
                    ENDP

UsageFault_Handler  PROC
                    EXPORT  UsageFault_Handler              [WEAK]
                    B       .
                    ENDP


SVC_Handler         PROC
                    EXPORT  SVC_Handler                     [WEAK]
                    B       .
                    ENDP

DebugMon_Handler    PROC
                    EXPORT  DebugMon_Handler                [WEAK]
                    B       .
                    ENDP

PendSV_Handler      PROC
                    EXPORT  PendSV_Handler                  [WEAK]
                    B       .
                    ENDP

SysTick_Handler     PROC
                    EXPORT  SysTick_Handler                 [WEAK]
                    B       .
                    ENDP


Default_Handler     PROC
                    EXPORT  WDG_Handler                     [WEAK]
                    EXPORT  RTC_Handler                     [WEAK]
                    EXPORT  FLASH_Handler                   [WEAK]
                    EXPORT  RCC_Handler                     [WEAK]
					EXPORT  PA_Handler                      [WEAK]
                    EXPORT  PB_Handler                      [WEAK]
                    EXPORT  PC_Handler                      [WEAK]
                    EXPORT  PD_Handler                      [WEAK]						
                    EXPORT  DMA_Handler                     [WEAK]
                    EXPORT  ADC_Handler                     [WEAK]
                    EXPORT  TMR0_Handler                    [WEAK]
                    EXPORT  TMR1_Handler                    [WEAK]
                    EXPORT  TMR2_Handler                    [WEAK]						
                    EXPORT  TMR3_Handler                    [WEAK]
                    EXPORT  TMR4_Handler                    [WEAK]
                    EXPORT  I2C_Handler	                    [WEAK]
                    EXPORT  SPI0_Handler                    [WEAK]
                    EXPORT  SPI1_Handler                    [WEAK]
                    EXPORT  UART0_Handler                   [WEAK]
                    EXPORT  UART1_Handler                   [WEAK]
                    EXPORT  UART2_Handler                   [WEAK]
                    EXPORT  USB11_Handler                   [WEAK]
                    EXPORT  QSPI_Handler                    [WEAK]
                    EXPORT  TMR5_Handler                    [WEAK]						
					EXPORT  AES_Handler	                    [WEAK]
          

WDG_Handler
RTC_Handler
FLASH_Handler
RCC_Handler
PA_Handler
PB_Handler
PC_Handler
PD_Handler
DMA_Handler
ADC_Handler
TMR0_Handler
TMR1_Handler
TMR2_Handler
TMR3_Handler
TMR4_Handler
I2C_Handler
SPI0_Handler
SPI1_Handler
UART0_Handler
UART1_Handler
UART2_Handler
USB11_Handler
QSPI_Handler
TMR5_Handler
AES_Handler

                    B       .
                    ENDP

                    ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                    IF      :DEF:__MICROLIB

                    EXPORT  __initial_sp
                    EXPORT  __heap_base
                    EXPORT  __heap_limit

                    ELSE

                    IMPORT  __use_two_region_memory
                    EXPORT  __user_initial_stackheap
__user_initial_stackheap

                    LDR     R0, =  Heap_Mem
                    LDR     R1, = (Stack_Mem + Stack_Size)
                    LDR     R2, = (Heap_Mem + Heap_Size)
                    LDR     R3, = Stack_Mem
                    BX      LR

                    ALIGN

                    ENDIF

                    END
