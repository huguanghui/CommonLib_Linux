## 定时器的使用

### 1. 基于timer_create的SIGEV_THREAD方式的定时器

> 参考 timer_test/test3.c 文件

优点: 系统接口,不用封装可以直接使用
缺点: 当处理函数耗时高于定时接口有异常问题(对于全局变量处理,内存处理有double free风险)


