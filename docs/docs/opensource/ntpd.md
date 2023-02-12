------

## NTP时钟同步流程

1. NTP客户端在T1时刻发送请求报文
2. 请求报文在T2时刻到达NTP服务端
3. NTP处理请求在T3时刻发送响应报文
4. 客户端在T4时刻接收到请求报文

Delay = ((T4-T1) - (T3-T2))/2

T4 + offset = T3 + Delay

## busybox中源码
