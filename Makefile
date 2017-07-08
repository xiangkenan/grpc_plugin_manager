DIR=$(shell pwd)
SRC=${DIR}/src
PROTO=${DIR}/proto
LIB=${DIR}/lib

CC=g++

CFLAG= -std=c++11 -L/usr/local/lib64 -Wl,--no-as-needed -lgrpc++_reflection -Wl,--no-as-needed -lprotobuf -lpthread -ldl

INCLUDE =  	-I${PROTO} \
	 		-I${LIB}/glog/include \
		  	-I${LIB}/libxml2/include

LDFLAGS =	-L${LIB}/glog/lib -lglog -Wl,-rpath,./lib/glog/lib \
			-L${LIB}/libxml2/lib -lxml2 -Wl,-rpath,./lib/libxml2/lib \
			-lpthread

all: build_dir pb manager clinet clean

build_dir:
	rm -fr ${DIR}/bin
	rm -fr ${DIR}/log
	mkdir ${DIR}/bin
	mkdir ${DIR}/log

manager: ofo_main.pb.o ofo_main.grpc.pb.o main_server.o core.o plugin_manager.o
	${CC} $^ -o $@ ${CFLAG} ${INCLUDE} ${LDFLAGS}

pb: ./proto/ofo_main.proto
	protoc  -I ./proto/ --cpp_out=${PROTO} $^
	protoc -I ./proto/ --grpc_out=${PROTO} --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin $^

clinet: ofo_main.pb.o ofo_main.grpc.pb.o test_clinet.o
	${CC} $^ -o $@ ${CFLAG} ${INCLUDE} ${LDFLAGS}

ofo_main.pb.o:
	${CC} ${CFLAG} -c -o $@ ${PROTO}/ofo_main.pb.cc

ofo_main.grpc.pb.o:
	${CC} ${CFLAG} -c -o $@ ${PROTO}/ofo_main.grpc.pb.cc

core.o:
	${CC} ${CFLAG} ${INCLUDE} ${LDFLAGS} -c -o $@ ${SRC}/core.cc

plugin_manager.o:
	${CC} ${CFLAG} ${INCLUDE} ${LDFLAGS} -c -o $@ ${SRC}/plugin_manager.cc

main_server.o:
	${CC} ${CFLAG} ${INCLUDE} ${LDFLAGS} -c -o $@ ${SRC}/main_server.cc

test_clinet.o:
	${CC} ${CFLAG} ${INCLUDE} ${LDFLAGS} -c -o $@ ${SRC}/test_clinet.cc

clean:
	rm proto/*pb*
	rm -fr *.o
	mv manager ./bin
	mv clinet ./bin
