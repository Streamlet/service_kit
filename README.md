# C++ Micro Service Framework

旨在用 C++ 技术实现服务端微服务框架

## 基本结构
```
+-----------------+
| registry center |
+-----------------+
    ^           ^
    | register  | query service
    | (grpc)    | (grpc)
    |           |
+--------+      |
| server |      |
+--------+      |
    ^           |
    | biz api   |
    | (grpc)    |
    |           |
+--------+      |
| client |------+
+--------+
```

## 基本设计

框架定义或实现：
1. 可直接运行的注册中心实体
2. server、client 与注册中心的通讯协议

使用方定义或实现：
1. server 与 client 之间的通讯协议
2. 可运行的 server、client 实体
