# ServiceKit

Using C++ to build micro-service based backend frameworks and tools.

## Getting started

### Download third-party libraries

```bash
./fetch_third_party.sh
```

### Build third-party libraries

```bash
./build_third_party.sh
```

### Build ServiceKit

```bash
./build_service_kit.sh
```

### Build samples

```bash
./build_sample.sh
```

### Run sample

Run registry center:
```bash
sample/bin/registry_center [--port=50050]
```

Run server side (service provider):
```bash
sample/bin/greeter_server [--registry_center=localhost:50050] [--port=50051]
```

Run client side (service consumer)
```bash
sample/bin/greeter_client [--registry_center=localhost:50050]
```
