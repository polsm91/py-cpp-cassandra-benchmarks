#creates a given number of threads (pass by argument) and assigns an equal portion of the workload to each one
#the main thread waits actively until they finish


import threading
import sys
import itertools
import cppinterf as cppth

from multiprocessing.pool import ThreadPool

num_keys = 10000
max_parallelism = int(sys.argv[1])


count = itertools.count() #starts at 0
finished = False

def pyworker(start,end,count):
    cppth.worker(start,end)
    while cppth.running() != 0:
	pass
    global finished
    if count.next() == max_parallelism-1:
        finished=True

#pool = ThreadPool(max_parallelism)

#k_to_thread=num_keys/max_parallelism
#for i in xrange(0,num_keys,k_to_thread):
#    pool.apply_async(pyworker, args=(i,i+k_to_thread,))

#print'now close'
#pool.close()

#while (not finished): 
#    pass
#print 'now wait'
#pool.join()

ths = []
k_to_thread=num_keys/max_parallelism
for i in xrange(0,num_keys,k_to_thread):
    ths.append(threading.Thread(target=pyworker,args=(i,i+k_to_thread,count)))
for th in ths:
    th.start()

while not finished:
    pass

for th in ths:
    th.join()
