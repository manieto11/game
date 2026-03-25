# Run: make game

# ---------------- Host ----------------
ifeq ($(OS),Windows_NT)
HOST_OS := windows
SHELL := cmd.exe
.SHELLFLAGS := /C
else
HOST_OS := linux
endif

# ---------------- Toolchains ----------------
WINDOWS_CXX ?= x86_64-w64-mingw32-g++
WINDOWS_CC  ?= x86_64-w64-mingw32-gcc
LINUX_CXX   ?= g++
LINUX_CC    ?= gcc

ifeq ($(HOST_OS),windows)
DEFAULT_CXX := $(WINDOWS_CXX)
DEFAULT_CC  := $(WINDOWS_CC)
else
DEFAULT_CXX := $(LINUX_CXX)
DEFAULT_CC  := $(LINUX_CC)
endif

ifeq ($(origin CXX),default)
CXX := $(DEFAULT_CXX)
endif
ifeq ($(origin CXX),undefined)
CXX := $(DEFAULT_CXX)
endif
ifeq ($(origin CC),default)
CC := $(DEFAULT_CC)
endif
ifeq ($(origin CC),undefined)
CC := $(DEFAULT_CC)
endif

# ---------------- Directories ----------------
SRC_DIR    := ./src
BOX_DIR    := ./box2d/src
BUILD_ROOT := builds
OBJ_ROOT   := obj
TARGET_DIR := $(BUILD_ROOT)/$(HOST_OS)
TARGET_OBJ := $(OBJ_ROOT)/$(HOST_OS)
STEAM_APPID := $(BUILD_ROOT)/steam_appid.txt

# ---------------- Sources ----------------
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
BOX_SRCS := $(wildcard $(BOX_DIR)/*.c)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_OBJ)/%.o,$(SRCS))
BOX_OBJS := $(patsubst $(BOX_DIR)/%.c,$(TARGET_OBJ)/box2d/%.o,$(BOX_SRCS))

# ---------------- Flags ----------------
CXXFLAGS ?= -O2 -std=c++17
CFLAGS   ?= -O2 -std=gnu11
CPPFLAGS += -I./include -I./box2d/include -I./SteamworksSDK/public
LDFLAGS  ?=

# ---------------- Platform Config ----------------
RAYLIB_WINDOWS_INCLUDE ?= C:/raylib/src
RAYLIB_WINDOWS_LIBDIR  ?= C:/raylib/src

ifeq ($(HOST_OS),linux)
OUTPUT_NAME := game
LIBDIRS := -L. -L./SteamworksSDK/redistributable_bin/linux64
LIBS    := -lraylib -lsteam_api
RUNTIME_LIB := ./SteamworksSDK/redistributable_bin/linux64/libsteam_api.so
LDFLAGS += -Wl,-rpath,'$$ORIGIN'
else
OUTPUT_NAME := game.exe
LIBDIRS := -L. -L./SteamworksSDK/redistributable_bin/win64
LIBS    := -lraylib -lsteam_api64 -lopengl32 -lgdi32 -lwinmm -luser32 -lkernel32 -lshell32 -lws2_32
RUNTIME_LIB := ./SteamworksSDK/redistributable_bin/win64/steam_api64.dll
ifneq ($(strip $(RAYLIB_WINDOWS_INCLUDE)),)
CPPFLAGS += -I$(RAYLIB_WINDOWS_INCLUDE)
endif
ifneq ($(strip $(RAYLIB_WINDOWS_LIBDIR)),)
LIBDIRS += -L$(RAYLIB_WINDOWS_LIBDIR)
endif
endif

OUT_FILE := $(TARGET_DIR)/$(OUTPUT_NAME)

# ---------------- Host Helpers ----------------
ifeq ($(HOST_OS),windows)
to_win_path = $(subst /,\,$(1))
ensure_dir = @if not exist "$(call to_win_path,$(1))" mkdir "$(call to_win_path,$(1))"
remove_dir = @if exist "$(call to_win_path,$(1))" rmdir /S /Q "$(call to_win_path,$(1))"
remove_files = @if exist "$(call to_win_path,$(1))" del /Q "$(call to_win_path,$(1))"
copy_file_if_exists = @if exist "$(call to_win_path,$(1))" copy /Y "$(call to_win_path,$(1))" "$(call to_win_path,$(2))" >nul
ensure_steam_appid = @if exist "$(call to_win_path,$(STEAM_APPID))" (copy /Y "$(call to_win_path,$(STEAM_APPID))" "$(call to_win_path,$(1))\steam_appid.txt" >nul) else (echo 480> "$(call to_win_path,$(1))\steam_appid.txt")
else
ensure_dir = @mkdir -p "$(1)"
remove_dir = @rm -rf "$(1)"
remove_files = @rm -f $(1)
copy_file_if_exists = @if [ -f "$(1)" ]; then cp -f "$(1)" "$(2)"; fi
ensure_steam_appid = @if [ -f "$(STEAM_APPID)" ]; then cp -f "$(STEAM_APPID)" "$(1)/steam_appid.txt"; else printf '480\n' > "$(1)/steam_appid.txt"; fi
endif

# ---------------- Targets ----------------
all: game

game: $(OUT_FILE)
	@echo Build complete: $(OUT_FILE)

ifeq ($(HOST_OS),windows)
$(OUT_FILE): $(OBJS) $(BOX_OBJS)
	$(call ensure_dir,$(@D))
	$(CXX) $(OBJS) $(BOX_OBJS) $(LDFLAGS) $(LIBDIRS) $(LIBS) -o $@
	$(call copy_file_if_exists,$(RUNTIME_LIB),$(@D))
	$(call ensure_steam_appid,$(@D))
else
$(OUT_FILE): $(OBJS) $(BOX_OBJS)
	$(call ensure_dir,$(@D))
	$(CXX) $(OBJS) $(BOX_OBJS) $(LDFLAGS) $(LIBDIRS) $(LIBS) -o $@
	$(call copy_file_if_exists,$(RUNTIME_LIB),$(@D))
endif

$(TARGET_OBJ)/%.o: $(SRC_DIR)/%.cpp
	$(call ensure_dir,$(@D))
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TARGET_OBJ)/box2d/%.o: $(BOX_DIR)/%.c
	$(call ensure_dir,$(@D))
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

ifeq ($(HOST_OS),windows)
clean:
	$(call remove_files,$(TARGET_OBJ)/*.o)
	$(call remove_dir,$(TARGET_DIR))	
else
clean:
	$(call remove_files,$(TARGET_OBJ)/*.o)
	$(call remove_dir,$(TARGET_DIR))
endif

clean-all:
	$(call remove_dir,$(OBJ_ROOT))
	$(call remove_dir,$(BUILD_ROOT)/linux)
	$(call remove_dir,$(BUILD_ROOT)/windows)

ifeq ($(HOST_OS),windows)
run: game
	@cd /D "$(call to_win_path,$(TARGET_DIR))" && $(OUTPUT_NAME)
else
run: game
	@cd "$(TARGET_DIR)" && ./$(OUTPUT_NAME)
endif

help:
	@echo Usage: make game
	@echo Host OS: $(HOST_OS)
	@echo Goals: game, run, clean, clean-all, help
	@echo Override the native compiler with CXX=... CC=...

.PHONY: all game clean clean-all run help
