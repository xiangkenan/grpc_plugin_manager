#CPPFLAGS = -g -Wall -fPIC -shared -DRELEASE_C
CPPFLAGS = -g -Wall -fPIC -shared

INCLUDE= -I../ \
		 -I../../lib/glog/include

all: freq.so clean
	mv freq.so ../../conf/plugins/

freq.so: freq.o
	g++ $(CPPFLAGS) $(INCLUDE) -o $@ $^

freq.o: freq.cc
	g++ $(CPPFLAGS) $(INCLUDE) -c -o $@ $^

clean:
	rm *.o
