CXX=g++ -Wall -MMD -Werror -Wextra -g # TODO remove -g
CXXFLAGS= -std=c++20
LOGIN=xstola03_xpavli95

SRCS	= $(shell find ./src -type f -name "*.cpp")
HEADS	= $(shell find ./src/include -type f -name "*.h")

OBJS	= $(SRCS:.cpp=.o)

DEPS	= $(OBJS:.o=.d)

all: traffic_simulation

traffic_simulation: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

tar: src LICENSE Makefile README.md
	tar -cvzf $(LOGIN).tar.gz $^

.PHONY: clean all tar

clean:
	rm src/*.o traffic_simulation

