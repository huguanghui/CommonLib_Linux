# USBPIPE

>  实现USBPIPE基础通信功能

---

## 目录介绍

|        目录        | 功能 |
| :----------------: | :--: |
|   build   | 编译文件存放目录(不要上传仓库) |
|   libpipe   | usbpipe的源代码 |
| host        |  模拟主控(类似于客户端的角色)  |
| device | 模拟协处理器-接收响应(类似于服务器的角色) |
| sender | 模拟协处理器-只发送(类似于服务器的角色) |
| test_list | 测试util中封装链表的使用 |

---

## 编译方法

> 编译框架依赖cmake的, 编译前请先安装下cmake工具 

```shell
# 未生成 build 目录时
$ ./build
# 生成 build 目录后
$ cd build/debug 
$ make usbpipe
```

