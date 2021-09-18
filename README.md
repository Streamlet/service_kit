# ServiceKit

旨在用 C++ 技术实现服务端微服务框架

## 构建步骤

### 下载第三方依赖

```bash
./fetch_third_party.sh
```

### 编译第三方库

```bash
./build_third_party.sh
```

### 编译示例程序

```bash
./build_sample.sh
```

### 运行示例程序

注册中心：
```bash
registry_center/cmake/build/registry_center [--port=50050]
```
服务端
```bash
sample/helloworld/cmake/build/greeter_server [--registry_center=localhost:50050] [--port=50051]
```
客户端
```bash
sample/helloworld/cmake/build/greeter_client [--registry_center=localhost:50050]
```
