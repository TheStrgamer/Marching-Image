FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    cmake \
    g++ \
    make \
    libopencv-dev \
    tzdata \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir -p /app/output

RUN mkdir -p build && cd build && cmake .. && make

CMD ["./build/Colormap"]
