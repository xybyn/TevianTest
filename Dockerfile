FROM vookimedlo/ubuntu-qt:5.15_gcc_focal as build

RUN apt-get update && \
    apt-get install -y cmake && \
    apt-get install -y git
WORKDIR /app/qt5
RUN git clone git://code.qt.io/qt/qt5.git
WORKDIR /app/qt5/qt5
RUN ./init-repository
RUN ./configure -developer-build
RUN cmake --build .
ADD ./src /app/src
ADD CMakeLists.txt /app
WORKDIR /app/build

RUN cmake .. && \
    cmake --build .

FROM ubuntu:latest

WORKDIR /app

COPY --from=build /app/build/TevianTest .
ENTRYPOINT ["./TevianTest"]
