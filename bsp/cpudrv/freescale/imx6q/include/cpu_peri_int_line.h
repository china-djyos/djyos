/* generated. */
#ifndef __CPU_PERI_INT_LINE_H__
#define __CPU_PERI_INT_LINE_H__

/* cortex-a9 interrupts*/
#define CN_INT_LINE_0    0
#define CN_INT_LINE_1    1
#define CN_INT_LINE_2    2
#define CN_INT_LINE_3    3
#define CN_INT_LINE_4    4
#define CN_INT_LINE_5    5
#define CN_INT_LINE_6    6
#define CN_INT_LINE_7    7
#define CN_INT_LINE_8    8
#define CN_INT_LINE_9    9
#define CN_INT_LINE_10    10
#define CN_INT_LINE_11    11
#define CN_INT_LINE_12    12
#define CN_INT_LINE_13    13
#define CN_INT_LINE_14    14
#define CN_INT_LINE_15    15
#define CN_INT_LINE_16    16
#define CN_INT_LINE_17    17
#define CN_INT_LINE_18    18
#define CN_INT_LINE_19    19
#define CN_INT_LINE_20    20
#define CN_INT_LINE_21    21
#define CN_INT_LINE_22    22
#define CN_INT_LINE_23    23
#define CN_INT_LINE_24    24
#define CN_INT_LINE_25    25
#define CN_INT_LINE_26    26
#define CN_INT_LINE_27    27
#define CN_INT_LINE_28    28
#define CN_INT_LINE_29    29
#define CN_INT_LINE_30    30
#define CN_INT_LINE_31    31

/* imx6q interrupts*/
#define CN_INT_LINE_IOMUXC               32             /* General-Purpose Register 1 from IOMUXC. Used to notify the cores on the exception condition while booting. */
#define CN_INT_LINE_DAP                  33             /* Debug Access Port interrupt request */
#define CN_INT_LINE_SDMA                 34             /* SDMA interrupt request from all channels */
#define CN_INT_LINE_VPU                  35             /* JPEG codec interrupt request */
#define CN_INT_LINE_SNVS                 36             /* PMIC power off request */
#define CN_INT_LINE_IPU                  37             /* IPU error interrupt request */
#define CN_INT_LINE_IPU1                 38             /* IPU1 sync interrupt request */
#define CN_INT_LINE_IPU2                 39             /* IPU2 error interrupt request */
#define CN_INT_LINE_IPU2                 40             /* IPU2 sync interrupt request */
#define CN_INT_LINE_GPU3D                41             /* GPU3D interrupt request */
#define CN_INT_LINE_R2D                  42             /* GPU2D R2D GPU2D general interrupt request */
#define CN_INT_LINE_V2D                  43             /* GPU2D V2D GPU2D(OpenVG) general interrupt request */
#define CN_INT_LINE_VPU                  44             /* VPU interrupt request */
#define CN_INT_LINE_APBH_Bridge_DMA      45             /* Logical OR of APBH-Bridge-DMA channels 0-3 completion and error interrupts */
#define CN_INT_LINE_EIM                  46             /* EIM interrupt request */
#define CN_INT_LINE_BCH                  47             /* BCH operation complete interrupt */
#define CN_INT_LINE_GPMI                 48             /* GPMI operation timeout error interrupt */
#define CN_INT_LINE_DTCP                 49             /* DTCP interrupt request */
#define CN_INT_LINE_VDOA                 50             /* Logical OR of VDOA interrupt requests */
#define CN_INT_LINE_SNVS                 51             /* SRTC consolidated interrupt */
#define CN_INT_LINE_SNVS                 52             /* SRTC security interrupt */
#define CN_INT_LINE_CSU                  53             /* CSU interrupt request 1. Indicates to the processor that one or more alarm inputs were asserted */
#define CN_INT_LINE_uSDHC1               54             /* uSDHC1 (Enhanced SDHC) interrupt request */
#define CN_INT_LINE_uSDHC2               55             /* uSDHC2 (Enhanced SDHC) interrupt request */
#define CN_INT_LINE_uSDHC3               56             /* uSDHC3 (Enhanced SDHC) interrupt request */
#define CN_INT_LINE_uSDHC4               57             /* uSDHC4 (Enhanced SDHC) interrupt request */
#define CN_INT_LINE_UART1                58             /* UART1 interrupt request */
#define CN_INT_LINE_UART2                59             /* UART2 interrupt request */
#define CN_INT_LINE_UART3                60             /* UART3 interrupt request */
#define CN_INT_LINE_UART4                61             /* UART4 interrupt request */
#define CN_INT_LINE_UART5                62             /* UART5 interrupt request */
#define CN_INT_LINE_eCSPI1               63             /* eCSPI1 interrupt request */
#define CN_INT_LINE_eCSPI2               64             /* eCSPI2 interrupt request */
#define CN_INT_LINE_eCSPI3               65             /* eCSPI3 interrupt request */
#define CN_INT_LINE_eCSPI4               66             /* eCSPI4 interrupt request */
#define CN_INT_LINE_eCSPI5               67             /* eCSPI5 interrupt request */
#define CN_INT_LINE_I2C1                 68             /* I2C1 interrupt request */
#define CN_INT_LINE_I2C2                 69             /* I2C2 interrupt request */
#define CN_INT_LINE_I2C3                 70             /* I2C3 interrupt request */
#define CN_INT_LINE_SATA                 71             /* SATA interrupt request */
#define CN_INT_LINE_USB_1                72             /* USB Host 1 interrupt request */
#define CN_INT_LINE_USB_2                73             /* USB Host 2 interrupt request */
#define CN_INT_LINE_USB_3                74             /* USB Host 3 interrupt request */
#define CN_INT_LINE_USB_OTG              75             /* USB OTG interrupt request */
#define CN_INT_LINE_USB_PHY              76             /* UTMI0 interrupt request */
#define CN_INT_LINE_USB_PHY              77             /* UTMI1 interrupt request */
#define CN_INT_LINE_SSI1                 78             /* SSI1 interrupt request */
#define CN_INT_LINE_SSI2                 79             /* SSI2 interrupt request */
#define CN_INT_LINE_SSI3                 80             /* SSI3 interrupt request */
#define CN_INT_LINE_TEMP                 81             /* Temperature Monitor Temperature Sensor (temperature greater than threshold) interrupt */
#define CN_INT_LINE_ASRC                 82             /* ASRC interrupt request */
#define CN_INT_LINE_ESAI                 83             /* ESAI interrupt request */
#define CN_INT_LINE_SPDIF                84             /* SPDIF interrupt */
#define CN_INT_LINE_MLB150               85             /* MLB error interrupt request */
#define CN_INT_LINE_PMU                  86             /* Brownout of the 1.1, 2.5, and 3.0 analog regulators occurred. */
#define CN_INT_LINE_GPT                  87             /* Logical OR of GPT rollover interrupt line, input capture 1 and 2 lines, output compare 1, 2, and 3 interrupt lines */
#define CN_INT_LINE_EPIT1                88             /* EPIT1 output compare interrupt */
#define CN_INT_LINE_EPIT2                89             /* EPIT2 output compare interrupt */
#define CN_INT_LINE_GPIO1_INT7           90             /* INT7 interrupt request */
#define CN_INT_LINE_GPIO1_INT6           91             /* INT6 interrupt request */
#define CN_INT_LINE_GPIO1_INT5           92             /* INT5 interrupt request */
#define CN_INT_LINE_GPIO1_INT4           93             /* INT4 interrupt request */
#define CN_INT_LINE_GPIO1_INT3           94             /* INT3 interrupt request */
#define CN_INT_LINE_GPIO1_INT2           95             /* INT2 interrupt request */
#define CN_INT_LINE_GPIO1_INT1           96             /* INT1 interrupt request */
#define CN_INT_LINE_GPIO1_INT0           97             /* INT0 interrupt request */
#define CN_INT_LINE_GPIO1_0_15           98             /* Combined interrupt indication for GPIO1 signals 0 - 15 */
#define CN_INT_LINE_GPIO1_16_31          99             /* Combined interrupt indication for GPIO1 signals 16 - 31 */
#define CN_INT_LINE_GPIO2_0_15           100            /* Combined interrupt indication for GPIO2 signals 0 - 15 */
#define CN_INT_LINE_GPIO2_16_31          101            /* Combined interrupt indication for GPIO2 signals 16 - 31 */
#define CN_INT_LINE_GPIO3_0_15           102            /* Combined interrupt indication for GPIO3 signals 0 - 15 */
#define CN_INT_LINE_GPIO3_16_31          103            /* Combined interrupt indication for GPIO3 signals 16 - 31 */
#define CN_INT_LINE_GPIO4_0_15           104            /* Combined interrupt indication for GPIO4 signals 0 - 15 */
#define CN_INT_LINE_GPIO4_16_31          105            /* Combined interrupt indication for GPIO4 signals 16 - 31 */
#define CN_INT_LINE_GPIO5_0_15           106            /* Combined interrupt indication for GPIO5 signals 0 - 15 */
#define CN_INT_LINE_GPIO5_16_31          107            /* Combined interrupt indication for GPIO5 signals 16 - 31 */
#define CN_INT_LINE_GPIO6_0_15           108            /* Combined interrupt indication for GPIO6 signals 0 - 15 */
#define CN_INT_LINE_GPIO6_16_31          109            /* Combined interrupt indication for GPIO6 signals 16 - 31 */
#define CN_INT_LINE_GPIO7_0_15           110            /* Combined interrupt indication for GPIO7 signals 0 - 15 */
#define CN_INT_LINE_GPIO7_16_31          111            /* Combined interrupt indication for GPIO7 signals 16 - 31 */
#define CN_INT_LINE_WDOG1                112            /* WDOG1 timer reset interrupt request */
#define CN_INT_LINE_WDOG2                113            /* WDOG2 timer reset interrupt request */
#define CN_INT_LINE_KPP                  114            /* Key Pad interrupt request */
#define CN_INT_LINE_PWM1                 115            /* Cumulative interrupt line for PWM1. Logical OR of rollover, compare, and FIFO waterlevel crossing interrupts" */
#define CN_INT_LINE_PWM2                 116            /* Cumulative interrupt line for PWM2. Logical OR of rollover, compare, and FIFO waterlevel crossing interrupts." */
#define CN_INT_LINE_PWM3                 117            /* Cumulative interrupt line for PWM3. Logical OR of rollover, compare, and FIFO waterlevel crossing interrupts." */
#define CN_INT_LINE_PWM4                 118            /* Cumulative interrupt line for PWM4. Logical OR of rollover, compare, and FIFO waterlevel crossing interrupts." */
#define CN_INT_LINE_CCM                  119            /* CCM interrupt request 1 */
#define CN_INT_LINE_CCM                  120            /* CCM interrupt request 2 */
#define CN_INT_LINE_GPC                  121            /* GPC interrupt request 1 */
#define CN_INT_LINE_122                  122            /* Reserved Reserved */
#define CN_INT_LINE_SRC                  123            /* SRC interrupt request */
#define CN_INT_LINE_CPU                  124            /* L2 interrupt request */
#define CN_INT_LINE_CPU                  125            /* Parity Check error interrupt request */
#define CN_INT_LINE_CPU                  126            /* Performance Unit interrupt */
#define CN_INT_LINE_CPU                  127            /* CTI trigger outputs interrupt */
#define CN_INT_LINE_SRC                  128            /* Combined CPU WDOG interrupts (4x) out of SRC */
#define CN_INT_LINE_129                  129            /* Reserved Reserved */
#define CN_INT_LINE_130                  130            /* Reserved Reserved */
#define CN_INT_LINE_131                  131            /* Reserved Reserved */
#define CN_INT_LINE_MIPI_CSI_1           132            /* CSI interrupt request 1 */
#define CN_INT_LINE_MIPI_CSI_2           133            /* CSI interrupt request 2 */
#define CN_INT_LINE_MIPI_DSI             134            /* DSI interrupt request */
#define CN_INT_LINE_MIPI_HSI             135            /* HSI interrupt request */
#define CN_INT_LINE_SJC                  136            /* SJC interrupt from the General-Purpose register */
#define CN_INT_LINE_CAAM_0               137            /* CAAM job ring 0 interrupt */
#define CN_INT_LINE_CAAM_1               138            /* CAAM job ring 1 interrupt */
#define CN_INT_LINE_139                  139            /*  Reserved Reserved */
#define CN_INT_LINE_ASC1                 140            /* ASC1 interrupt request */
#define CN_INT_LINE_ASC2                 141            /* ASC2 interrupt request */
#define CN_INT_LINE_FLEXCAN1             142            /* FLEXCAN1 combined interrupt. Logical OR of ini_int_busoff, ini_int_error, ipi_int_mbor, ipi_int_rxwarning, ipi_int_txwarning and ipi_int_wakein. Combined interrupt of ini_int_busoff,ini_int_error,ipi_int_mbor,ipi_int_txwarning and ipi_int_waken */
#define CN_INT_LINE_FLEXCAN2             143            /* FLEXCAN2 combined interrupt. Logical OR of ini_int_busoff, ini_int_error, ipi_int_mbor, ipi_int_rxwarning, ipi_int_txwarning and ipi_int_wakein. */
#define CN_INT_LINE_144                  144            /* Reserved Reserved */
#define CN_INT_LINE_145                  145            /* Reserved Reserved */
#define CN_INT_LINE_146                  146            /* Reserved Reserved */
#define CN_INT_LINE_HDMI_x               147            /* HDMI video and CEC control interrupt request. */
#define CN_INT_LINE_HDMI_xx              148            /* HDMI CEC dedicated wake-up interrupt. */
#define CN_INT_LINE_MLB150               149            /* Channels [31:0] interrupt requests. Channels [63:32] interrupt requests are available on IRQ #158, unless the MLB150_ACTL[SMX] bit is set, in which case those channels are muxed into this IRQ. */
#define CN_INT_LINE_ENET                 150            /* 150 */
#define CN_INT_LINE_ENET_TIMER           151            /* MAC 0 1588 Timer interrupt [synchronous] request */
#define CN_INT_LINE_PCIe_req1            152            /* PCIe interrupt request 1 (intd/msi_ctrl_int) */
#define CN_INT_LINE_PCIe_req2            153            /* PCIe interrupt request 2 (intc) */
#define CN_INT_LINE_PCIe_req3            154            /* PCIe interrupt request 3 (intb) */
#define CN_INT_LINE_PCIe_req4            155            /* PCIe interrupt request 4 (inta) */
#define CN_INT_LINE_DCIC1                156            /* Logical OR of DCIC1 interrupt requests */
#define CN_INT_LINE_DCIC2                157            /* Logical OR of DCIC2 interrupt requests */
#define CN_INT_LINE_MLB150_xx            158            /* Logical OR of channel[63:32] interrupt requests */
#define CN_INT_LINE_PMU                  159            /* Brownout of the core, GPU, and chip digital regulators occurred. */

#define CN_INT_LINE_LAST	159

#endif //__CPU_PERI_INT_LINE_H__
