# Dockerfile
FROM ubuntu:22.04

RUN apt update && \
    apt install -y g++ cmake make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN mkdir -p build && cd build && cmake .. && make

CMD ["./build/toy-lexer"]