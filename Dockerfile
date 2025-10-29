# Dockerfile
FROM ubuntu:22.04

# 设置非交互式安装
ENV DEBIAN_FRONTEND=noninteractive

# 安装开发所需的所有工具
RUN apt update && \
    apt install -y \
    git \
    g++ \
    cmake \
    make \
    gdb \
    vim \
    && \
    rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /workspaces/toy-lexer

# 容器启动后保持交互式 shell
CMD ["bash"]