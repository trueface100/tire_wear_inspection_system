CC = g++
CFLAGS = -std=c++11 -I/usr/include -I/usr/local/include/opencv4
LIBS = -lwiringPi `pkg-config --cflags --libs opencv4`

INCLUDEPATHS := -I${HOME}/workspace/tensorflow
LDFLAGS := -pthread -ltensorflow-lite -lflatbuffers -ldl
LDPATH := -L${HOME}/workspace/tensorflow/tensorflow/lite/tools/make/gen/bbb_armv7l/lib\
	  -L${HOME}/workspace/tensorflow/tensorflow/lite/tools/make/downloads/flatbuffers/build
SRCS = main.cpp lcd.cpp ultrasonic.cpp rgb.cpp servo.cpp
OBJS = $(SRCS:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -o $@ $^ $(LDPATH) $(LDFLAGS) $(LIBS)
%.o: %.cpp
	
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -c $< -o $@ 

clean:
	rm -f $(OBJS) $(EXECUTABLE)
