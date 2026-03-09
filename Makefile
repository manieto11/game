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
BOX_DIR	  = ./box2d/src

# ---------------- Files ----------------
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

BOX_S = $(wildcard $(BOX_DIR)/*.c)
BOX_O = $(patsubst $(BOX_DIR)/%.c,$(OBJ_DIR)/%.a,$(BOX_S))

# ---------------- Flags ----------------
CXXFLAGS = -O2
LDFLAGS  = -Wl,-rpath,'$$ORIGIN'

CFLAGS = -O2 -std=gnu11

# C-specific flags
C_EXTRA_FLAGS = 

# ---------------- Libraries ----------------
LIBS     = -lraylib -lsteam_api
LIBDIRS  = -L. -L./SteamworksSDK/redistributable_bin/linux64
RUNTIME_LIBS = ./SteamworksSDK/redistributable_bin/linux64/libsteam_api.so

# ---------------- Targets ----------------
all: game

game: $(OBJS) $(BOX_O)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) $(BOX_O) $(LDFLAGS) $(LIBDIRS) $(LIBS) $(INC_DIR) -o $(BUILD_DIR)/game.AppImage
	@for f in $(RUNTIME_LIBS); do \
		if [ -f "$$f" ]; then cp -f "$$f" $(BUILD_DIR)/; fi; \
	done
	@echo Build complete: $(BUILD_DIR)/game.AppImage

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@

$(OBJ_DIR)/%.a: $(BOX_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(C_EXTRA_FLAGS) -I$(INC_DIR) -c $< -o $@

# ---------------- Utilities ----------------
clean:
	rm -rf obj $(BUILD_DIR)

run: game
	@cd $(BUILD_DIR) && ./game.AppImage

.PHONY: all clean run game
