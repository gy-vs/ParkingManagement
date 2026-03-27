FROM --platform=linux/amd64 debian:bullseye-slim

WORKDIR /app

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    libreadline-dev \
    && rm -rf /var/lib/apt/lists/*

COPY . .

RUN mkdir -p build && cd build && \
    cmake .. && \
    make

WORKDIR /app/build

CMD ["./parking_lot"]
