# ---------------- Compiler ----------------
CXX = g++

# ---------------- Directories ----------------
SRC_DIR   = ./src
BUILD_DIR = builds
OBJ_DIR   = obj
INC_DIR	  = ./include

# ---------------- Files ----------------
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# ---------------- Flags ----------------
CXXFLAGS = -O2
LDFLAGS  = -Wl,-rpath,'$$ORIGIN'

# ---------------- Libraries ----------------
LIBS     = -lraylib -lsteam_api
LIBDIRS  = -L. -L./steam
RUNTIME_LIBS = steam/libsteam_api.so

# ---------------- Targets ----------------
all: game

game: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBDIRS) $(LIBS) -o $(BUILD_DIR)/game.AppImage
	@for f in $(RUNTIME_LIBS); do \
		if [ -f "$$f" ]; then cp -f "$$f" $(BUILD_DIR)/; fi; \
	done
	@echo Build complete: $(BUILD_DIR)/game.AppImage

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# ---------------- Utilities ----------------
clean:
	rm -rf obj $(BUILD_DIR)

run: game
	@cd $(BUILD_DIR) && ./game.AppImage

.PHONY: all clean run game
