#!/bin/sh

gcc amessage_serialize.c amessage.pb-c.c -o amessage_serial -I/root/install/include -L/root/install/lib -lprotobuf-c
