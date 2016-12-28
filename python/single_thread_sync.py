# Synchronous calls to Cassandra
# session.execute blocks until it has completed the query and the result is returned


from cassandra.cluster import Cluster
import itertools

cluster = Cluster()
cluster = Cluster(["minerva-5"])
session = cluster.connect("case18")
query = "SELECT * FROM case18.particle WHERE partid=?"
prepared = session.prepare(query)
num_keys = 10000


pending = itertools.count()
for key in xrange(num_keys):
    result = session.execute(prepared, [key])
