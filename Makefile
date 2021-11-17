CC := g++
LD := ld
SOURCES := $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)

SOFLAGS = -shared
CXXFLAGS += -fPIC -g
CXXFLAGS += $(INC_PATH)

TARGET_EXE := lc_solution

.PHONY: all clean

all: $(TARGET_LIB) $(TARGET_EXE)

all:

$(TARGET_EXE): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

$(TARGET_LIB): $(OBJECTS)
	$(CC) $(SOFLAGS) -o $@ $^ $(LDFLAGS)

%.d: %.cpp
	$(CC) $(CXXFLAGS) -M $< > $@

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET_EXE) $(TARGET_LIB)

