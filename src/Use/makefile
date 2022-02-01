LFLAGS= -L../../build 
CFLAGS=  -g -I../ -std=c++11 -Ofast -Wall -Wextra 
LIBS= -Bstatic -lSimd -lrt -Bdynamic -lpthread
OBJS= ${EXEC_NAME}.o ${EXEC_NAME}_gen.o
EXEC_NAME=detect
HFILES=../Simd/SimdParallel.hpp
all: ${OBJS} makefile
	${CXX} -Wl,${LFLAGS} -o ${EXEC_NAME} ${EXEC_NAME}.o ${LIBS}
	${CXX} -Wl,${LFLAGS} -o ${EXEC_NAME}_gen ${EXEC_NAME}_gen.o ${LIBS}

-include $(EXEC_NAME).d

%.o:%.cpp 
	${CXX} ${CFLAGS} -MMD -MP -c $<



clean:
	rm -rf *.o *d ${EXEC_NAME} 
