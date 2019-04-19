#!/usr/bin/env python
# -*- coding: utf-8 -*-

def redirect(start, url): start('302 Found', [('Location', url)]); return []

u_js = b'''\
D=document;
log=function(s,s2){ console.log(s2?arguments:s); };
'''

ContentType = dict(
    text  = ('Content-Type', 'text/plain'),
    html  = ('Content-Type', 'text/html'),
    js    = ('Content-Type', 'application/javascript'),
)

class appy:
    @staticmethod
    def hello(start):
        start('200 OK', [ContentType['text']])
        yield b'Hello, World\n'
    @staticmethod
    def u_js(start):
        start('200 OK', [ContentType['js']])
        yield u_js
    @staticmethod
    def index(start):
        start('200 OK', [ContentType['html']])
        yield "INDEX1"
    @staticmethod
    def index_html(start):
        start('200 OK', [ContentType['html']])
        yield "INDEX2"
    @staticmethod
    def error404(start):
        start('200 OK', [ContentType['text']])
        yield b'Not Found!\n'

def application(environ, start):
    path = environ['PATH_INFO'][1:].replace('.','_')
    print("PATH", repr(path))
    if not path:
        return redirect(start, '/index')
    return getattr(appy, path, appy.error404)(start)

import bottle
bottle.run(app=application)
