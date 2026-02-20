# ---------------- Compiler ----------------
CXX = g++

# ---------------- Directories ----------------
SRC_DIR   = .
BUILD_DIR = builds
OBJ_DIR   = obj/linux

# ---------------- Files ----------------
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# ---------------- Flags ----------------
CXXFLAGS = -O2
LDFLAGS  = -Wl,-rpath,'$$ORIGIN'

# ---------------- Libraries ----------------
LIBS     = -lraylib -lsteam_api
LIBDIRS  = -L.
RUNTIME_LIBS = libsteam_api.so

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
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------------- Utilities ----------------
clean:
	rm -rf obj $(BUILD_DIR)

run: game
	@cd $(BUILD_DIR) && ./game.AppImage

.PHONY: all clean run game
