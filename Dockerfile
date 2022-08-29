FROM ubuntu:latest as build
RUN apt-get update && \
    apt-get install -y g++ && \
    apt-get install -y cmake && \
    apt-get install -y git

WORKDIR /app
ADD ./src /app/src
ADD CMakeLists.txt /app
RUN apt-get install -y qtbase5-dev 
RUN apt-get install -y qt5-qmake



WORKDIR /app/build

RUN cmake .. && \
    cmake --build .



WORKDIR /app

ENTRYPOINT ["./build/TevianTest"]
