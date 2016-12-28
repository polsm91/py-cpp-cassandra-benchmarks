#creates a given number of parallel asynchronous calls (pass by argument)
#and makes asynchronous calls to cassandra processing the elements stored in queue
#the main thread waits actively until they finish


from cassandra.cluster import Cluster
import logging
import Queue
import itertools
import sys


cluster = Cluster()
cluster = Cluster(["minerva-5"])
session = cluster.connect("case18")
query = "SELECT * FROM case18.particle WHERE partid=?"
prepared = session.prepare(query)

num_keys = 10000
max_par = int(sys.argv[1])
finished = False
pending = itertools.count()

def log_error(item):
    logging.error("Operation failed: %s", item)

def call(args=None):
    if keys.empty():
        global finished, pending
        completed = pending.next()
        if completed == max_par - 1:
            finished = True
        return
    future = session.execute_async(prepared, [keys.get()])
    future.add_callbacks(call, log_error)

keys = Queue.Queue()
[keys.put(x) for x in xrange(num_keys)]
for i in xrange(max_par):
    call()
while not finished:
    pass