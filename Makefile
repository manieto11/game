# ---------------- Compiler ----------------
CXX = g++
CC  = gcc

# ---------------- Directories ----------------
SRC_DIR   = ./src
BUILD_DIR = builds
OBJ_DIR   = obj
INC_DIR	  = -I./include
INC_DIR	 += -I./box2d/include
INC_DIR	 += -I./SteamworksSDK/public
INC_DIR   += -I./glfw/include
INC_DIR   += -I./OpenAL/include
INC_DIR   += -I./Vulkan/Include
INC_DIR   += -I./glm/glm
BOX_DIR	  = ./box2d/src

# ---------------- Files ----------------
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

BOX_S = $(wildcard $(BOX_DIR)/*.c)
BOX_O = $(patsubst $(BOX_DIR)/%.c,$(OBJ_DIR)/%.a,$(BOX_S))

# Build GLFW from source to avoid depending on an external prebuilt glfw lib.
GLFW_S		= $(wildcard $(GLFW_DIR)/*.c)
GLFW_O		= $(patsubst $(GLFW_DIR)/%.c,$(OBJ_DIR)/glfw/%.o,$(GLFW_S))

# ---------------- Flags ----------------
CXXFLAGS = -O2
LDFLAGS  = -Wl,-rpath,'$$ORIGIN'

CFLAGS = -O2 -std=gnu11

GLFW_CFLAGS	= -D_GLFW_WIN32 -DUNICODE -D_UNICODE -I./glfw/src -I./glfw/deps

EXT_CFLAGS = $(GLFW_CFLAGS) $(OPENAL_CFLAGS) $(VULKAN_CFLAGS)
EXT_LIBS   = $(GLFW_LIBS) $(OPENAL_LIBS) $(VULKAN_LIBS)

# C-specific flags
C_EXTRA_FLAGS = 

# ---------------- Libraries ----------------
LIBS 		= -lopenal32 -lvulkan-1 -lsteam_api64 -lopengl32 -lgdi32 -lwinmm -luser32 -lkernel32 -lshell32 -lws2_32
LIBDIRS 	= -L. -L./SteamworksSDK/redistributable_bin/win64 -L./OpenAL/libs/Win64 -L./Vulkan/Lib

# ---------------- Targets ----------------
all: game

game: $(OBJS) $(BOX_O) $(GLFW_O)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) $(BOX_O) $(LDFLAGS) $(LIBDIRS) $(LIBS) -o $(BUILD_DIR)/game.AppImage
	@for f in $(RUNTIME_LIBS); do \
		if [ -f "$$f" ]; then cp -f "$$f" $(BUILD_DIR)/; fi; \
	done
	@echo Build complete: $(BUILD_DIR)/game.AppImage

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIR) $(EXT_CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.a: $(BOX_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(C_EXTRA_FLAGS) $(INC_DIR) $(EXT_CFLAGS) -c $< -o $@

# ---------------- Utilities ----------------
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

run: game
	@cd $(BUILD_DIR) && ./game.AppImage

.PHONY: all clean run game
