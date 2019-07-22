# USBPIPE

>  实现USBPIPE基础通信功能

>   UPTP (USB Pipe Transfer Protocol) - USBPipe传输协议

---

<font color="red">注意: <br>
- 为了调试方便, 目前编译的程序没有使用交叉编译器编译. 直接在 ubuntu 上编译运行.
</font>

## 目录介绍

|        目录        | 功能 |
| :----------------: | :--: |
|   build   | 编译文件存放目录(不要上传仓库) |
|   libpipe   | usbpipe的源代码 |
| host        |  模拟主控(类似于客户端的角色)  |
| device | 模拟协处理器-接收响应(类似于服务器的角色) |
| sender | 模拟协处理器-只发送(类似于服务器的角色) |

---

## 编译方法

> 编译框架依赖cmake的, 编译前请先安装下cmake工具 

```shell
# 未生成 build 目录时, build.sh中默认指定使用debug方式编译
$ ./build.sh
# 生成 build 目录后
$ cd build/debug 
$ make usbpipe
```


## 交叉编译器和编译选项设置

顶级目录下的 CMakeLists.txt 中 修改

```cmake
# 指定编译器
set(CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-gcc")
set(CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/arm-hisiv500-linux/target/bin/arm-hisiv500-linux-g++")
```

```cmake
# 修改编译参数
set(C_FLAGS
-Dhi3521d
-DHICHIP=0x3521d100
-DHI_RELEASE
-DHI_XXXX
-ffunction-sections
-fdata-sections
-fPIC
-D_LINUX
-DHDT_LINUX
-Wall
-O3
-DNDEBUG
)
```

## 将 debug 编译改为 Release

```cmake
# 修改编译方式, build.sh 文件
BUILD_TYPE=${BUILD_TYPE:-debug} 将 debug 改为 release
)
```

## 模式转换

zw_usbpipe_base.h 文件中 
    #define PIPE_TEST - 使用管道进行调试
    将这行注释掉就是使用 usbpipe 方式进行交互了
