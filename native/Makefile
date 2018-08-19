TOOLCHAIN=/media/disk2/bin/android-21-arm64
SYSROOT=${TOOLCHAIN}/sysroot

CC=${TOOLCHAIN}/bin/aarch64-linux-android-gcc
CPP=${TOOLCHAIN}/bin/aarch64-linux-android-g++
AS=${TOOLCHAIN}/bin/aarch64-linux-android-as
STRIP=${TOOLCHAIN}/bin/aarch64-linux-android-strip
CFLAGS=-O3 -fPIE -pie -g3 --sysroot=${SYSROOT}

all: so_info_get
so_info_get: so_info_get.c
	${CC} ${CFLAGS} -o so_info_get so_info_get.c

run:
	adb push so_info_get /data/data
	adb shell /data/data/so_info_get