#TARGET=--target=avx2-i32x8
TARGET=--target=avx2-i64x4
#TARGET=--target=sse4-i32x8
#TARGET=--target=sse4-i32x4

#IFLAGS+=-O0 -g
IFLAGS=-O3 --arch=x86-64

all:
	ispc $(TARGET) $(IFLAGS) core.ispc -h core_ispc.h -o core_ispc.o 

clean:
	rm -rf *.h *.o