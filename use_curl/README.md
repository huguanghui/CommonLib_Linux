[TOC]

# Curl

## curl命令的使用

```shell
# 将本地的file.txt文件以二进制形式上传到https://bashupload.com/name.txt
curl https://bashupload.com/name.txt --data-binary @file.txt
# 下载文件 不加-o是下载数据到终端,被写入到stdout 
curl https://bashupload.com/m0_OA/name.txt -o README.md
```

## curl库的使用