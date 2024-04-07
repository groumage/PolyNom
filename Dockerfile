FROM gcc:9.5.0

WORKDIR /app

RUN mkdir -p /app/cmake
COPY cmake /app/cmake/
RUN mkdir -p /app/build
RUN mkdir -p /app/lib
COPY lib /app/lib/ 
RUN mkdir -p /app/tests
COPY tests /app/tests/ 
COPY CMakeLists.txt /app/
COPY client.h /app/
COPY client.c /app/
COPY main.c /app/
COPY test.sh /app/

RUN apt-get update
RUN apt-get install -y cmake
RUN apt install -y pari-gp

RUN mkdir -p build && cd build && cmake .. && make && cd ..
