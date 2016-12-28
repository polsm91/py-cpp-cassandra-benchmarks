#creates a given number of threads (pass by argument) and assigns an equal portion of the workload to each one
#the main thread waits actively until they finish

from cassandra.cluster import Cluster
import threading
import sys
import itertools

cluster = Cluster()
cluster = Cluster(["minerva-5"])
session = cluster.connect("case18")
query = "SELECT * FROM case18.particle WHERE partid=?"
prepared = session.prepare(query)

num_keys = 10000
max_parallelism = int(sys.argv[1])


count = itertools.count() #starts at 0
finished = False

def call(keys):
    global finished
    for k in keys:
        result = session.execute(prepared, [k])
        # next returns value and increments subsequently
    if count.next() == max_parallelism-1:
        finished = True


ths = []
k_to_thread=num_keys/max_parallelism
for i in xrange(0,num_keys,k_to_thread):
    ths.append(threading.Thread(target=call,kwargs = {'keys':list(range(i,i+k_to_thread))}))
for th in ths:
    th.start()
while not finished:
    pass
for th in ths:
    th.join()