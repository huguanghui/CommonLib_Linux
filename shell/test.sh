#!/bin/sh

# for initscript in ./S[0-9][0-9]*
# do
#         if [ -x $initscript ] ;
#         then
#                 echo "[RCS]: $initscript"
#                 $initscript
#         fi
# done

#set -x

# i=1.0.0.1

# echo -n > test.conf

# if [ "$i" != "1.0.0.0" ];then
#     echo nameserver $i >> test.conf
# fi

#array_name=(value0 value1 value2 value3)

str="1.0.0.0 1.0.0.1 1.0.1.0 1.1.0.0"

echo $str

for i in $str
do
    echo $i
done
