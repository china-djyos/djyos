#ifndef CPU_PERI_IIC_H_
#define CPU_PERI_IIC_H_

//IIC控制器编号宏定义
#define CN_I2C1     0
#define CN_I2C2     1
#define CN_I2C3     2
#define CN_I2C_NUM  3

#define CN_IIC_SPEED_50KHZ       (50*1000)
#define CN_IIC_SPEED_100KHZ      (100*1000)
#define CN_IIC_SPEED_200KHZ      (200*1000)
#define CN_IIC_SPEED_300KHZ      (300*1000)
#define CN_IIC_SPEED_400KHZ      (400*1000)

//定义总线等待超时时间
#ifndef CONFIG_I2C_MBB_TIMEOUT
#define CONFIG_I2C_MBB_TIMEOUT  100000
#endif

//定义读写超时时间
#ifndef CONFIG_I2C_TIMEOUT
#define CONFIG_I2C_TIMEOUT  10000
#endif

//定义读写操作比特位
#define I2C_READ_BIT  1
#define I2C_WRITE_BIT 0

#endif /* CPU_PERI_IIC_H_ */
