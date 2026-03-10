.PHONY: all clean

PROJECT_NAME       ?= DataVisualizer
RAYLIB_PATH        ?= C:/raylib/raylib
COMPILER_PATH      ?= C:/raylib/w64devkit/bin

PLATFORM           ?= PLATFORM_DESKTOP
BUILD_MODE         ?= DEBUG

CC = g++

CFLAGS += -Wall -std=c++17 -D_DEFAULT_SOURCE -Wno-missing-braces

ifeq ($(BUILD_MODE),DEBUG)
	CFLAGS += -g -O0
else
	CFLAGS += -s -O1
endif

ifeq ($(OS),Windows_NT)
	PLATFORM_OS=WINDOWS
	export PATH := $(COMPILER_PATH):$(PATH)
endif

ifeq ($(PLATFORM_OS),WINDOWS)
	LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -lole32 -lcomdlg32
	
	ifeq ($(BUILD_MODE),RELEASE)
		CFLAGS += -Wl,--subsystem,windows
	endif
endif

INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -Ilib
LDFLAGS = -L. -L$(RAYLIB_PATH)/src

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SRC = $(call rwildcard, ./, *.cpp)
OBJS = $(SRC:.cpp=.o)

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(PROJECT_NAME).exe $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

clean:
	rm -f /s /q *.o *.exe
	@echo "Finished cleaning"