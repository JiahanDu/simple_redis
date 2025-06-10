# Simple_Redis — A Simplified Redis Implementation in C

## Overview

MiniRedis is a simplified version of Redis written in C, designed to run on a Windows laptop using Ubuntu (WSL).  
It supports core Redis functionality with a custom-built TCP server and client that communicate using the Redis Serialization Protocol (RESP).

## Features

- **TCP Server**  
  Implements a Redis-compatible server that accepts client connections and processes commands serialized with RESP.

- **TCP Client**  
  A command-line client capable of sending commands to MiniRedis or an official Redis server, using RESP-compliant messages.

- **Custom Hashmap**  
  A hashmap implementation supporting dynamic resizing and key-value storage, optimized for Redis-like operations.

- **Skiplist-Based Sorted Lists**  
  Sorted sets (ZSET) functionality implemented with skiplists for efficient insertion, deletion, and range queries.

## Usage

- Run the server to listen for incoming Redis commands.
- Use the client to send commands to the server or connect to the official redis server.
- The client and server exchange data using RESP, ensuring compatibility with official Redis clients and servers.

## Development

- Written entirely in C.
- Runs on Ubuntu subsystem on Windows (WSL).

