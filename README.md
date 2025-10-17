# ToyC 词法分析器
使用 C++ 编写的词法分析器，用于解析 ToyC 语言。

# 构建与运行

## 构建镜像
docker build -t toy-lexer .
**每次修改源码后都需重新构建镜像**

## 快速测试单行代码
echo 'int a = 1;' | docker run -i toy-lexer

## 管道测试
cat sample.c | docker run -i toy-lexer

## 重定向测试
docker run -i toy-lexer < sample.c

