DIR=$(shell pwd)
BIN_DIR=$(DIR)/bin
LIB_DIR=$(DIR)/lib
SRC_DIR=$(DIR)/src
PROTO_DIR=$(DIR)/proto

INCLUDE=	-I./src/rapidjson \
			-I./proto \
			-I./src \
			-I/usr/local/include \
			-I./lib/glog/include

LDFLAGS=	-L/usr/local/lib -lfcgi \
			-L./lib/glog/lib -lglog -Wl,-rpath,./lib/glog/lib

CFLAGS = -Wall -g -fPIC  -L/usr/local/lib64 -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -lprotobuf -lpthread -ldl -lssl -std=c++11 -I/usr/local/include -pthread

CC = g++  

all: pb plugin_client clean

pb: ./proto/ofo_main.proto
	protoc  -I ./proto/ --cpp_out=${PROTO_DIR} $^
	protoc -I ./proto/ --grpc_out=${PROTO_DIR} --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin $^


plugin_client: ofo_main.grpc.pb.o ofo_main.pb.o cgi.o
	${CC} $^ -o $@ ${CFLAGS} ${LDFLAGS}

ofo_main.grpc.pb.o:
	${CC} ${CFLAGS} -c -o $@ ${PROTO_DIR}/ofo_main.grpc.pb.cc

ofo_main.pb.o:
	${CC} ${CFLAGS} -c -o $@ ${PROTO_DIR}/ofo_main.pb.cc

cgi.o:
	${CC} ${CFLAGS} -c -o $@ cgi.cc ${INCLUDE} ${LDFLAGS}

clean:  
	rm -f *.o 
	rm ./proto/*pb*
