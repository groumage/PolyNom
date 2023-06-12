# Base image with a C/C++ development environment and CMake
FROM gcc:latest

# Set the working directory to /app
WORKDIR /app

# Copy the source code and CMakeLists.txt to the container
COPY . .

# Install GMP library and its development files
RUN apt-get update && \
    apt-get install -y libgmp-dev valgrind && \
    apt-get clean && \
    apt-get install -y build-essential cmake && \
    rm -rf /var/lib/apt/lists/*

# Configure, build, and test the project with CMake
RUN rm -rf build && \
    cmake -B build && \
    cmake --build build && \
    cmake --build build --target test

# Set the entry point to the main executable
# CMD ["sh", "./test.sh"]
