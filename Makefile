# ---------------- Compilers ----------------
CXX_LINUX = g++
CXX_WIN   = x86_64-w64-mingw32-g++

# ---------------- Directories ----------------
SRC_DIR   = .
BUILD_DIR = builds
OBJ_LINUX = obj/linux
OBJ_WIN   = obj/windows

# ---------------- Files ----------------
SRCS      = $(wildcard $(SRC_DIR)/*.cpp)
OBJS_LINUX = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_LINUX)/%.o,$(SRCS))
OBJS_WIN   = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_WIN)/%.o,$(SRCS))

# ---------------- Flags ----------------
CXXFLAGS  = -O2
LINUX_LDFLAGS = -Wl,-rpath,'$$ORIGIN'
WIN_LDFLAGS = -fuse-ld=bfd

LINUX_LIBS = -lraylib -lsteam_api
WIN_LIBS   = -lraylib -lsteam_api64 -lopengl32 -lgdi32 -lwinmm -luser32 -lkernel32 -lshell32 -lws2_32

WIN_LIBDIRS = . /usr/x86_64-w64-mingw32/lib third_party/raylib/build-mingw
LINUX_LIBDIR = .

WIN_RUNTIME_LIBS = steam_api64.dll
LINUX_RUNTIME_LIBS = libsteam_api.so

# ---------------- Targets ----------------
all: game game_linux

# ---------- Windows ----------
game: $(OBJS_WIN)
	@mkdir -p $(BUILD_DIR)
	$(CXX_WIN) $(OBJS_WIN) $(WIN_LDFLAGS) $(addprefix -L,$(WIN_LIBDIRS)) $(WIN_LIBS) -o $(BUILD_DIR)/game.exe
	@for f in $(WIN_RUNTIME_LIBS); do \
		if [ -f "$$f" ]; then cp -f "$$f" $(BUILD_DIR)/; fi; \
	done

$(OBJ_WIN)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_WIN)
	$(CXX_WIN) $(CXXFLAGS) -c $< -o $@

# ---------- Linux ----------
game_linux: $(OBJS_LINUX)
	@mkdir -p $(BUILD_DIR)
	$(CXX_LINUX) $(OBJS_LINUX) $(LINUX_LDFLAGS) -L$(LINUX_LIBDIR) $(LINUX_LIBS) -o $(BUILD_DIR)/game.AppImage
	@for f in $(LINUX_RUNTIME_LIBS); do \
		if [ -f "$$f" ]; then cp -f "$$f" $(BUILD_DIR)/; fi; \
	done

$(OBJ_LINUX)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_LINUX)
	$(CXX_LINUX) $(CXXFLAGS) -c $< -o $@

# ---------------- Utilities ----------------
clean:
	rm -rf obj $(BUILD_DIR)
