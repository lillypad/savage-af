# Savage-AF

Savage-AF is a modular CnC Generator used to overload IDS and IPS Systems.

__NOTE:__ In very early development help if you like.

# Description
Savage-AF will allow you to create modular CnC traffic using `lisp` or `python` which than can be run threaded for mass distraction. This is possible because the sockets library does not care about receiving data back as the socket is closed immediately after data is sent.

# Dependencies
- [`OpenSSL`](https://www.openssl.org/)
- [`cmake`](https://cmake.org)
- [`cffi`](https://cffi.readthedocs.io/)

# Building
```bash
$ make
```

# To Do
- Add SSL Support
- Domain / IP Logic for http_send
- add multithreading
