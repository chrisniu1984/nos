#!/bin/bash

NAME=`basename $0`

FD_SEC_UNIT=512

if [ -z $1 ]; then
    # 编译kernel
    make -C kernel clean
    make -C kernel; [ $? -ne 0 ] && exit -1
    
    KERNEL_BYTES=`wc -c kernel/kernel | cut -d' ' -f1`
    KERNEL_SEC_CNT=$((${KERNEL_BYTES}/${FD_SEC_UNIT} + 1))
    
    echo $KERNEL_BYTES
    echo $KERNEL_SEC_CNT
    
    # 把相应的值转为16进制串
    HEX_KERNEL_SEC_CNT=`printf "0x%.2x\n" $KERNEL_SEC_CNT`
    
    # 替换并生成boot.inc
    sed -e "s/#FDSEC_KERNEL_NUM#/${HEX_KERNEL_SEC_CNT}/g" boot/boot.inc.tmpl > boot/boot.inc
    
    # 编译 boot
    make -C boot; if [ $? -ne 0 ]; then exit -1; fi
    
    # 写入到bochs/a.img
    dd if=/dev/zero of=bochs/a.img bs=1474560 count=1
    dd if=boot/boot of=bochs/a.img bs=${FD_SEC_UNIT} count=1 conv=notrunc seek=0
    dd if=kernel/kernel of=bochs/a.img bs=${FD_SEC_UNIT} count=${KERNEL_SEC_CNT} conv=notrunc seek=1

    make -C bochs

elif [ $1 == "clean" ]; then
    make -C boot clean
    make -C kernel clean
    make -C bochs clean

elif [ $1 == "help" ]; then
    function show()
    {
        echo "  " $@
    }
    show $NAME
    show $NAME clean
    show $NAME help
fi
