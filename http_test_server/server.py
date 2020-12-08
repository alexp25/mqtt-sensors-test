#!/usr/bin/env python

import socket
import time
from flask import Flask
from flask import render_template, send_file, session, Response, request, make_response
from flask import jsonify
import json

import datetime
from threading import Thread
from dateutil.parser import parse
import gevent
from gevent import monkey
import sys,traceback
monkey.patch_time()

from flask_sockets import Sockets
from gevent import pywsgi
from geventwebsocket.handler import WebSocketHandler


#app
async_mode = 'gevent'

app = Flask(__name__, static_folder="dist")

sockets = Sockets(app)

# set the secret key.  keep this really secret:
app.secret_key = 'A0Zr98j/3yX R~XHH!jmN]LWX/,?RT'


@sockets.route('/echo')
def echo_socket(ws):
    while not ws.closed:
        message = ws.receive()
        print('ws/echo message: ' + message)
        ws.send(message)

@sockets.route('/')
def example_socket(ws):
    while not ws.closed:
        message = ws.receive()
        if not message is None:
            print('ws/ message: ' + message)
            ws.send(message)

@app.route('/')
def mypisite(name = None):
    return "No website"

@app.route('/data', methods=['GET','POST'])
def data():
    if request.method == 'GET':
        return jsonify({'result': True, 'data': ''})
    if request.method == 'POST':
        return jsonify({'result': True})

if __name__ == '__main__':    
    print('server started')
    server = pywsgi.WSGIServer(('0.0.0.0', 8000), app, handler_class=WebSocketHandler)
    server.serve_forever()

   
        


