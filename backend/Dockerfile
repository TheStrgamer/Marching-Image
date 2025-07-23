FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    cmake \
    g++ \
    make \
    libopencv-dev \
    libboost-all-dev \
    libasio-dev \
    nlohmann-json3-dev \
    git \
    tzdata \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir -p /app/output
RUN git clone https://github.com/CrowCpp/crow.git external/crow


RUN mkdir -p build && cd build && cmake .. && make

CMD ["./build/Colormap"]
