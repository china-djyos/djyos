//用户注释不可以 “InitCode”或“Configure”开头
//InitCode 创建工程时，如下代码将被IDE copy到初始化函数中，不需要配置的，可以空着。——本行不可删改
    ModuleInstall_FAT(CFG_MOUNT_POINT, CFG_MOUNT_DEV, CFG_OPTIONS);
//Configure 可配置配置常量的默认值，真实值在APP的工程目录中。将在IDE中图形化显示，允许用户图形化配置。——本行 不可删改
#define CFG_MOUNT_POINT         ""      // FAT文件系统安装目录
#define CFG_MOUNT_DEV           ""      // FAT文件系统所在设备路径
#define CFG_OPTIONS             0       // FAT文件系统文件安装逻辑, 1（安装前格式化整个文件系统）；2（使用时才安装）；4（文件系统不存在在新建）；8（文件系统只读）                                       
                                        
                                        

