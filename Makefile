obj-m := myled.o

myled.ko: myled.c                                    #←<ターゲット>: <必要なファイル>
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 modules #注意　tab, 「`」

clean:
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 clean
# myled_robotsystem
