#!/usr/bin/env python

import json
import cffi

ffi = cffi.FFI()

ffi.cdef("int http_send(char *domain, int port, char *data);")

C = ffi.dlopen("./build/libsavage-af.so")


def http_send(domain, port, data):
    domain = domain.encode('ascii')
    data = data.encode('ascii')
    C.http_send(domain, port, data)


def http_headers(headers):
    try:
        data = ""
        for key, value in headers.items():
            data += str(key) + ': ' + str(value) + '\r\n'
        return data
    except Exception as e:
        raise e


def http_data(method, path, headers, payload):
    try:
        data = ''
        methods = [
            'GET',
            'POST',
            'DELETE',
            'CONNECT',
            'PUT',
            'OPTIONS',
            'HEAD'
        ]
        if method in methods:
            data += method
            data += ' '
            data += path
            data += ' '
            data += 'HTTP/1.0 '
            data += '\r\n'
            data += http_headers(headers)
            data += payload
        return data
    except Exception as e:
        raise e

if __name__ == "__main__":
    http_send("example.com", 80, http_data('GET', '/index.html', {'test':123}, "payload"))
