CXX=g++ -Wall -MMD -Werror -Wextra
LOGIN=xstola03_xpavli95
TARGET = traffic-simulation

SRCS	= $(shell find ./src -type f -name "*.cpp")
HEADS	= $(shell find ./src/include -type f -name "*.h")

OBJS	= $(SRCS:.cpp=.o)
DEPS	= $(OBJS:.o=.d)

# Simulation parameters
ONE_LANE_DATA_DIR = data/data_one_lane
TWO_LANE_DATA_DIR = data/data_two_lane
ROAD_LENGTH = 10000
SPEED_UP = 1000000000

all: $(TARGET)

run: all
	echo "Running one-lane simulation with: road length=400m; time=0.5h; speed-up=100x"
	./$(TARGET) one-lane --road_length 400 -t 0.5 --speed-up 100 2>data_one_lane.csv

	echo "Running two-lane simulation with: road length=400m; time=0.5h; two_lane_portion=40%; speed-up=100x"
	./$(TARGET) two-lane --road_length 400 -t 0.5 --speed-up 100 --two-lane-portion 40 2>data_two_lane.csv

# Dataset targets
dataset: data_one_lane data_two_lane

data_one_lane: all
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/1.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/2.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/3.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/4.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/5.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/6.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/7.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/8.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/9.csv
	./$(TARGET) one-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(ONE_LANE_DATA_DIR)/10.csv

data_two_lane: all
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/1.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/2.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/3.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/4.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/5.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/6.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/7.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/8.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/9.csv
	./$(TARGET) two-lane --road_length $(ROAD_LENGTH) -t 1 --speed-up $(SPEED_UP) 2>$(TWO_LANE_DATA_DIR)/10.csv


# Utility targets

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

tar: src LICENSE Makefile README.md
	tar -cvzf $(LOGIN).tar.gz $^

clean:
	rm src/*.o src/*.d $(TARGET)

.PHONY: clean all run tar dataset data_one_lane data_two_lane
