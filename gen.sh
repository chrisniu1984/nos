#!/bin/bash

NAME=`basename $0`

if [ -z $1 ]; then
    # 编译setup
    make -C setup; if [ $? -ne 0 ]; then exit -1; fi
    
    # 编译kernel
    make -C kernel; if [ $? -ne 0 ]; then exit -1; fi
    
    ############################################################
    # 根据编译成功的setup和kernel的大小，填充boot.inc中的常量
    
    # 获得setup和kernel的大小
    SETUP_SIZE=`wc -c setup/setup | cut -d' ' -f1`
    KERNEL_SIZE=`wc -c kernel/kernel | cut -d' ' -f1`
    
    # floppy 中 sector 的大小
    FDSEC_UNIT=512
    
    # 计算setup在floppy中的sector及其number
    FDSEC_SETUP=2
    FDSEC_SETUP_NUM=$((${SETUP_SIZE}/ ${FDSEC_UNIT} + 1))
    
    # 计算setup在floppy中的sector及其number
    FDSEC_KERNEL=$((${FDSEC_SETUP}+ ${FDSEC_SETUP_NUM} + 1))
    FDSEC_KERNEL_NUM=$((${KERNEL_SIZE}/ ${FDSEC_UNIT} + 1))
    
    echo $FDSEC_SETUP
    echo $FDSEC_SETUP_NUM
    echo $FDSEC_KERNEL
    echo $FDSEC_KERNEL_NUM
    
    # 把相应的值转为16进制串
    #HEX_FDSEC_SETUP=2
    HEX_FDSEC_SETUP_NUM=`printf "0x%.2x\n" $FDSEC_SETUP_NUM`
    HEX_FDSEC_KERNEL=`printf "0x%.2x\n" $FDSEC_KERNEL`
    HEX_FDSEC_KERNEL_NUM=`printf "0x%.2x\n" $FDSEC_KERNEL_NUM`
    
    # 替换并生成boot.inc
    sed -e "s/#FDSEC_SETUP_NUM#/${HEX_FDSEC_SETUP_NUM}/g" \
        -e "s/#FDSEC_KERNEL#/${HEX_FDSEC_KERNEL}/g" \
        -e "s/#FDSEC_KERNEL_NUM#/${HEX_FDSEC_KERNEL_NUM}/g" \
        boot/boot.inc.tmpl > boot/boot.inc
    
    # 编译 boot
    make -C boot; if [ $? -ne 0 ]; then exit -1; fi
    
    # 写入到bochs/a.img
    dd if=/dev/zero of=bochs/a.img bs=1474560 count=1
    dd if=boot/boot of=bochs/a.img bs=${FDSEC_UNIT} count=1 conv=notrunc seek=0
    dd if=setup/setup of=bochs/a.img bs=${FDSEC_UNIT} count=${FDSEC_SETUP_NUM} conv=notrunc seek=$(($FDSEC_SETUP-1))
    dd if=kernel/kernel of=bochs/a.img bs=${FDSEC_UNIT} count=${FDSEC_KERNEL_NUM} conv=notrunc seek=$(($FDSEC_KERNEL-1))
    make -C bochs

elif [ $1 == "clean" ]; then
    make -C boot clean
    make -C setup clean
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
