#CPPFLAGS = -g -Wall -fPIC -shared -DRELEASE_C
CPPFLAGS = -g -Wall -fPIC -shared

INCLUDE= -I../ \
		 -I../../lib/glog/include

all: frequence.so clean
	mv frequence.so ../../conf/plugins/

frequence.so: freq.o
	g++ $(CPPFLAGS) $(INCLUDE) -o $@ $^

freq.o: freq.cc
	g++ $(CPPFLAGS) $(INCLUDE) -c -o $@ $^

clean:
	rm *.o
