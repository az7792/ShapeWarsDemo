这是一个基于C++的简单Linux TCP网络库，使用了事件驱动的单Reactor模型，并在此基础上实现了一个echo服务器

## 项目结构
```
.
├── include
│   ├── Acceptor.h
│   ├── Channel.h
│   └── ......
├── src
│   ├── Acceptor.cpp
│   ├── Channel.cpp
│   └── ......
├── makefile
├── main.cpp
└── client.cpp
```
## 关键类和功能

1. `EventLoop`

`EventLoop` 类管理事件循环，使用 `epoll` 来监控多个 `Channel` 的事件，并在事件发生时调用相应的回调函数。

2. `Channel`

`Channel` 类封装了与文件描述符（fd）相关的事件处理逻辑。持有事件回调函数（如读取、写入和错误处理）。通过调用 `enableReading` 和 `enableWriting` 等方法，可以注册感兴趣的事件。

3. `TcpConnection`

`TcpConnection` 类表示与客户端的 TCP 连接，维护本地和对端的地址信息，提供发送数据的接口。还处理与 `Channel` 的交互，确保正确的读写事件被处理

4. `Acceptor`

`Acceptor` 类负责接受来自客户端的连接请求。通过创建新的 `TcpConnection` 实例来处理每个连接。

5. `InetAddress`

`InetAddress` 类封装了 IP 地址和端口信息。

6. `Socket`

`Socket` 类封装了底层的 socket 操作，包括创建、连接、发送和接收数据等。

7. `Epoll`

`Epoll` 类封装了 epoll 的具体实现，提供事件监控和管理多个 `Channel` 的能力。它处理注册、更新和删除事件，并通过 `wait` 方法返回触发的`Channel`。

8. `Logger`

`Logger` 类用于记录日志信息，支持不同的日志级别（ DEBUG、INFO 等）。

## Echo服务器

### 使用说明

1. **构建**:  

   ```bash
   make
   ```

2. **启动服务器**:
   在终端中运行：

   ```bash
   ./main
   ```

   默认服务器监听在 `127.0.0.1:7792`

3. **启动客户端**:
   在另一个终端中运行：

   ```bash
   ./client
   ```

   输入 "exit" 退出客户端