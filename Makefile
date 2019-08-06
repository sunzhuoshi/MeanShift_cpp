
CXX = g++
CXXFLAGS = -O3

$ISPC_OBJS=
ifeq "$(USE_ISPC)" ""
 USE_ISPC=0
endif

ifeq "$(USE_ISPC)" "1"
 ISPC_OBJS=ispc/core_ispc.o
endif

CXXFLAGS += -DUSE_ISPC=$(USE_ISPC)

all: MeanShift

ispc/core_ispc.o:
	make -C ispc all

test: all
	./MeanShift

clean:
	rm -rf MeanShift
	rm -rf *.o
	make -C ispc clean

MeanShift: $(ISPC_OBJS) cpp_test.o MeanShift.o 
	$(CXX) -o $@ $(CXXFLAGS) $+
