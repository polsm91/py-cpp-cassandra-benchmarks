#creates a given number of processes (pass by argument)
#and performs synchronous calls to cassandra based on the range of keys assigned
#the main process waits actively until all of them have completed the job

from multiprocessing import Pool, Value
from cassandra.cluster import Cluster
import sys

counter = None
finished = Value('b', False)
prepared = None
session = None

max_par = int(sys.argv[1])
num_keys = 10000


def init(c):
    global counter, session, prepared
    counter = c
    cluster = Cluster()
    cluster = Cluster(["minerva-5"])
    session = cluster.connect("case18")
    query = "SELECT * FROM case18.particle WHERE partid=?"
    prepared = session.prepare(query)


def worker_mem_shared(key, limit):
    for k in xrange(key, limit):
        #        print 'key', k
        result = session.execute(prepared, [k])
    with counter.get_lock():
        counter.value += 1
    if counter.value == max_par:
        global finished;
        finished.value = True


if __name__ == '__main__':
    counter = Value('i', 0)
    chunk_size = num_keys / max_par
    pool = Pool(processes=max_par, initializer=init, initargs=(counter,))
    multiple_results = [pool.apply_async(worker_mem_shared, (i * chunk_size + 1, (i + 1) * chunk_size)) for i in
                        range(max_par)]

    while not finished.value:
        pass
