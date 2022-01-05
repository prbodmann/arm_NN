LFLAGS= -L../../build 
CFLAGS= -g -I../ -std=c++11 -Ofast
LIBS= -Bstatic -lSimd -lrt -Bdynamic -lpthread
OBJS= ${EXEC_NAME}.o
EXEC_NAME=detect
HFILES=../Simd/SimdParallel.hpp
all: ${EXEC_NAME}

${EXEC_NAME}: ${OBJS} makefile
	${CXX} -Wl,${LFLAGS} -o ${EXEC_NAME} ${OBJS} ${LIBS}

-include $(EXEC_NAME).d

%.o:%.cpp 
	${CXX} ${CFLAGS} -MMD -MP -c $<



clean:
	rm -rf *.o *d ${EXEC_NAME} 
