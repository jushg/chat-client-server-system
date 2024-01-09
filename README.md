# chat-client-server-system

# Client Server Chat Systems

This is a simplified implementation of a client server chat system, with round time trip calculation, as well as server reconnection once the client terminate.

## Compiler and architecture support

This project uses `CMake`, and have been tested for stable run on `MacOS` and `Linux(Ubuntu)`.
Usage of C++11 features

## Quick Start


First, clone and build

```sh
# Wherever you cloned this codebase to
$ cmake .
$ make 
```

Then, start the interactive engine client

```sh
$ ./chat_system
```

On startup, you will be ask to input whether to start as server or client. Input `0` to start as client, `1` to start as server. If client, specify the ip address and port to connect to, if server specify the port to listen on. Examples:

Client Startup:

```sh
0 127.0.0.1 12345
```

Server Startup:

```sh
1 12345
```
Do note that each server will only interact with 1 client at a time.
