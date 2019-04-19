#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

def redirect(start, url): start('302 Found', [('Location', url)]); return []

u_js = b'''\
D=document;
log=function(s,s2){ console.log(s2?arguments:s); };
'''

ContentType = dict(
    py    = ('Content-Type', 'text/plain'),
    text  = ('Content-Type', 'text/plain'),
    html  = ('Content-Type', 'text/html'),
    js    = ('Content-Type', 'application/javascript'),
    wasm  = ('Content-Type', 'application/wasm'),
)

class App:
    
    def __call__(_, env, start):
        path = env['PATH_INFO'].replace('.','_').replace('/','GET_', 1)
        print("PATH:" + path)
        return getattr(_, path, _.not_found)(env, start)

    def error404(_, env, start):
        start('404 NOT FOUND', [ContentType['text']])
        return [ b'Not Found!\n' ]

    def not_found(_, env, start):
        try:    fil = open('.'+env['PATH_INFO'])
        except: return _.error404(env, start)
        ext = env['PATH_INFO'].split('.')[-1]
        start('200 OK', [ContentType.get(ext, ContentType['html'])])
        return fil

    def GET_(_, env, start):
        return redirect(start, '/index')

    def GET_hello(_, env, start):
        start('200 OK', [ContentType['text']])
        yield b'Hello, World\n' 

    def GET_u_js(_, env, start):
        start('200 OK', [ContentType['js']])
        yield u_js

    def GET_index(_, env, start):
        start('200 OK', [ContentType['html']])
        yield "INDEX1"

    def GET_index_html(_, env, start):
        start('200 OK', [ContentType['html']])
        yield "INDEX2"

if __name__ == '__main__':
    import bottle
    bottle.run(app=App())
