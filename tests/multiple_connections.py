import socket
import threading
import time
import random 
import os
import sys

address = '127.0.0.1'

def test_connection(port = int(sys.argv[1])):
    s = socket.socket()
    try:
        s.connect((address, port))
        time.sleep(random.uniform(0.1, 5.2)) 
        s.send('GET /')
    except Exception as e:
        print("%s:%d:%s" % (address, port, e))
        os._exit(1)
    finally:
        s.close()

threads = []
for _ in range(10):
    t = threading.Thread(target=test_connection)
    t.start()
    threads.append(t)

for thread in threads:
    thread.join()
