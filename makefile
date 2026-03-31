CC = g++

# a) build the module .o
a: circularBuffer.o util/validator.o util/random.o util/streams.o

circularBuffer.o: circularBuffer.cpp circularBuffer.h
	${CC} -c circularBuffer.cpp -o circularBuffer.o

util/validator.o: util/validator.cpp util/util.h
	${CC} -c util/validator.cpp -o util/validator.o

util/random.o: util/random.cpp util/util.h
	${CC} -c util/random.cpp -o util/random.o

util/streams.o: util/streams.cpp util/util.h
	${CC} -c util/streams.cpp -o util/streams.o

# b) run demo
b: circularBuffer.o util/validator.o util/random.o util/streams.o
	${CC} -DDEMO_MODE demo.cpp circularBuffer.o util/validator.o util/random.o util/streams.o -o demo.exe
	demo.exe

# c) run tests
c: circularBuffer.o util/validator.o util/random.o util/streams.o
	${CC} -DTEST_MODE test.cpp circularBuffer.o util/validator.o util/random.o util/streams.o -o test.exe
	test.exe

# g) clean and rebuild from scratch
g:
	del /f *.o *.exe util\*.o
	mingw32-make a