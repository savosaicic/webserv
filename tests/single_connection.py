import socket
import threading
import time
import sys
import os

address = '127.0.0.1'

def test_connection(port):
    s = socket.socket()
    try:
        s.connect((address, port))
        time.sleep(1)
    except Exception as e:
        print("%s:%d:%s" % (address, port, e))
        os._exit(1)
    finally:
        s.close()

test_connection(int(sys.argv[1]))
