#!/bin/sh

for initscript in ./S[0-9][0-9]*
do
        if [ -x $initscript ] ;
        then
                echo "[RCS]: $initscript"
                $initscript
        fi
done