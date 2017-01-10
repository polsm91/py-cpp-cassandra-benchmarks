# Python and C++ benchmarks for Apache Cassandra
Tests written to evaluate interaction strategies with Apache Cassandra using Python or C++. 

Single threading, multithreading and process level parallelism are implemented as well as different methodologies to communicate with Apache Cassandra like synchronous and asynchronous calls chained as callbacks. Moreover, Swing is used as an interface between Python and C++.

Here below are reported the results obtained after conducting several experiments:

**Data set:** 10000 keys
**Query:** "SELECT * FROM case18.particle WHERE partid=?"
**Runs each test:** 3
Command: /usr/bin/time --format '%E %U %S ' ./test) &>> out_test
Intel® Xeon® Processor L5630 (16 threads, 4 cpu)


    time format: minutes:seconds.centisecond


1. **Python:**

1.1. Single thread synchronous calls:

real(s)	 |   user(s)	|   sys(s)
------------ |------------ |------------
81.08    |   30.01   |	1.61000


1.2. Single thread, multiple callback chains:


#parallel_calls	|   real(s)	|   user(s)	|   sys(s)
------------ |------------ |------------ |------------
1	|   88.53	|   86.98	|   7.86667
2	|   53.04	|   51.89	|   7.35000
4	|   36.90	|   35.92	|   5.94667
8	|   28.18	|   27.22	|   4.25000
16	|   25.84	|   25.26	|   3.27667
32	|   26.03	|   25.30	|   2.91333
64	|   26.37	|   25.65	|   3.15000
128	|   27.67	|   27.05	|   3.09000

1.3 Multiple threads, synchronous calls


#threads	|   real(s)	|   user(s)	|   sys(s)
------------ |------------ |------------ |------------
1	|   87.97    |	86.04	|  8.52333
2	|   55.98	|   53.80	|   8.16333
4	|   40.70   |   39.10	|   6.71333
8	|   35.27	|   33.78	|   5.95000
16	|   33.88	|   32.24	|   5.93000
32	|   35.02	|   33.29	|   6.16667
64	|   34.95	|   33.17	|   6.50667
128	|   36.10	|   33.93	|   6.64667
256	|   38.81	|   36.32	|   7.14667


   1.4 Multi processes, synchronous calls

#processes	|   real(s)	|   user(s)	|   sys(s)
------------ |------------ |------------ |------------
1	|   77.58    |	104.40	|   1.55000
2	|   39.65	|   66.10	|   1.66000
4	|   21.02	|   50.38	|   1.82667
8	|   11.40	|   44.33	|   2.38333
16	|   6.49	|   42.60	|   3.04000
32	|   4.67	|   45.68	|   4.08000
64	|   4.43	|   49.38	|   4.86000
128	|   5.09	|   55.04	|   6.63667
256	|   6.39	|   68.77	|   10.46667


2. **C++**

2.1 Single thread synchronous calls

real(s)	|   user(s)	|   sys(s)
------------ |------------ |------------
56.62   |	3.11    |	1.44000



2.2 Single thread, single callback chain

real(s)	|   user(s)	|   sys(s)
------------ |------------ |------------
54.64   |	56.91667	|   1.26000



2.3 Single thread, multiple callback chains

#parallel_calls	|   real(s)	|   user(s)	|   sys(s)
------------ |------------ |------------ |------------
1   |   54.69	|   57.19	|   1.11333
2	|   27.34	|   29.60	|   1.06000
4	|   14.15	|   16.26	|   0.93000
8	|   7.41	|   9.39	|   0.96000
16	|   4.00	|   5.78	|   0.94667
32	|   3.10	|   4.55	|   0.87667
64	|   3.06	|   4.62	|   0.93000
128	|   2.78	|   4.32	|   0.86667
256	|   2.66	|   4.15	|   0.90000


2.4 Multiple threads, synchronous calls


#num_th	|   real(s)	|   user(s)	|   sys(s)
------------ |------------ |------------ |------------
1	|56.00	|	58.73	|   1.31333
2	|28.00	|	30.73	|   1.13667
4	|14.33	|	16.85	|   1.12333
8	|7.00	|	9.75	|   1.04667
16	|3.67	|	6.12	|   1.08333
32	|2.67	|	4.60	|   1.10000
64	|3.00	|	4.91	|   1.01000
128	|2.67	|	4.53	|   1.07000
256	|3.00	|	4.40	|   1.07000


3. **Python and C++**


3.1 Python threads execute C++ callbacks chains

#num_th |   real(s)     |   user(s)     |   sys(s)
------------ |------------ |------------ |------------
1	|55.26	|65.31	|18.30667
2	|28.09	|32.62	|11.95667
4	|14.43	|17.03	|7.78333
8	|7.62	|9.63	|4.76667
16	|4.14	|5.94	|3.17667
32	|2.97	|4.65	|2.38667
64	|3.25	|4.81	|2.73333
128	|3.11	|4.68	|2.63667
256	|6.61	|7.92	|5.40000




