@mainpage

[TOC]

# 使用dnsmasq搭建DNS服务器

## 搭建环境

- Virtualbox + Ubuntu
- dnsmasq

## 安装

```shell
# apt方式安装
$ sudo apt-get install dnsmasq
# 服务开启
$ service dnsmasq start
```

## 配置

> 配置文件位置/etc/dnsmasq.conf

### DNS服务器添加域名和IP的映射表

> 在配置文件中搜索address=,找到相关的配置位置,按照下面格式,添加想要映射的关系表. 

```c++
# 添加domain到IP的映射
address=/usdev.zviewcloud.com/192.168.6.95
```

注意: 如果想要映射关系立即生效,请重启服务

## 使用

```shell
$ service dnsmasq start             # 启动服务
$ service dnsmasq restart           # 重启服务
$ service dnsmasq stop              # 停止服务
$ service dnsmasq force-reload      # 强行加载
$ service dnsmasq status            # 查看运行状态和log
$ chkconfig dnsmasq on              # dnsmasq设为开机自启
```

开放了防火墙端口有时无法解析域名

```shell
$ service iptables stop             # 关闭防火墙
$ chkconfig iptables off            #关闭防火墙的开机自启
```

## P2P相关的地址表

测试zwhosts时在/etc/dnsmasq.conf添加如下的域名映射

```c
# Add domains which you want to force to an IP address here.
# The example below send any host in double-click.net to a local
# web-server.
address=/double-click.net/127.0.0.1
address=/usdev.zviewcloud.com/47.88.33.190
address=/szdev.zviewcloud.com/119.23.131.217
address=/szdev.aftx.net/120.76.126.62
address=/erdev.zviewcloud.com/47.91.88.40
address=/bjdev.aftx.net/123.57.156.223
address=/hzdev.aftx.net/121.43.96.85
```

## 相关连接

> 由于目前只需要简单使用到DNS服务器中的域名解析服务就可以了,dnsmasq的一些其他特性功能还未进一步使用和研究.如果后续需要使用其他特性功能,再来完善该文档.

- [DNS服务器的理论](<https://www.cnblogs.com/heiye123/articles/7687922.html>)
