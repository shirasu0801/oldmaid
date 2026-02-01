# Makefile for Old Maid (Babanuki) Game

# Compiler
CXX = em++

# Source files
SOURCES = src/card.cpp src/player.cpp src/game.cpp src/bindings.cpp

# Output
OUTPUT = build/game.js

# Compiler flags
CXXFLAGS = -O2 -std=c++17

# Emscripten flags
EMFLAGS = -s WASM=1 \
          -s MODULARIZE=0 \
          -s EXPORT_ES6=0 \
          -s ENVIRONMENT='web' \
          -s ALLOW_MEMORY_GROWTH=1 \
          --bind

# Default target
all: $(OUTPUT)

# Build target
$(OUTPUT): $(SOURCES)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(EMFLAGS) $(SOURCES) -o $(OUTPUT)
	@echo "Build complete: $(OUTPUT)"

# Clean target
clean:
	rm -rf build/*
	@echo "Clean complete"

# Development build with debug info
debug: CXXFLAGS = -g -std=c++17
debug: EMFLAGS += -s ASSERTIONS=1
debug: $(OUTPUT)

# Serve locally (requires Python)
serve:
	@echo "Starting local server at http://localhost:8080"
	@cd web && python -m http.server 8080

.PHONY: all clean debug serve
