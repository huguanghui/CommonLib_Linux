#!/bin/sh

gcc -O2 amessage_deserial.c amessage.pb-c.c -o amessage_deserial -I/root/install/include -L/root/install/lib -lprotobuf-c
