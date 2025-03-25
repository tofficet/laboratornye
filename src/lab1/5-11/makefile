# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra

# Исходные файлы
SRCS := main.cpp priority_queue.cpp
OBJS := $(SRCS:.cpp=.o)
HEADERS := priority_queue.h node.h

# Имя исполняемого файла
TARGET := priority_queue

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)