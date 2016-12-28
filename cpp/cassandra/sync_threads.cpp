#include <atomic>
#include <iostream>
#include <thread>

#include <unistd.h>
#include <vector>
#include <cassandra.h>
#include <chrono>

using namespace std;
int n_keys = 10000;



thread *threads;
atomic<int> finished (0);
CassSession *session;
int n_threads = 1;
const CassPrepared *prepared;


void worker(int start, int end) {
    while (start<=end) {
        CassStatement* statement = cass_prepared_bind(prepared);
        cass_statement_bind_int32(statement,0, start);

        CassFuture *query_future = cass_session_execute(session, statement);
        cass_statement_free(statement);
        CassError rc = cass_future_error_code(query_future);

        if (rc != CASS_OK) {
            printf("cassandra-write-err-message\n");
            std::cerr << "cassandra-write-err-message\n";
            fprintf(stderr, "cassandra-write-err-message\n");
        }
        cass_future_free(query_future);
        start+=1;
    }
    ++finished;
}

void run_test()
{
    int start,end, chunk_size;
    chunk_size = n_keys/n_threads;
    thread threads[n_threads];
    time_t now,t_end;
    time(&now);

    for (int i = 0; i < n_threads; ++i) {
        start = i * chunk_size;
        end = start + chunk_size;
        threads[i] = thread(&worker, start,end);
    }
    while (finished < n_threads) ;
    for (int i = 0; i<n_threads; ++i) {
        threads[i].join();
    }
    time(&t_end);
    cout << "done" << endl;
    // cout << "n_th: " << n_threads << " " <<  << endl;
}



int main(int argc, char *argv[]) {
    n_threads = atoi(argv[1]);

    /* SETUP CASSANDRA DRIVER */
    CassFuture *connect_future = NULL;
    CassCluster *cluster = cass_cluster_new();
    session = cass_session_new();

    /* Add contact points */
    cass_cluster_set_contact_points(cluster, "minerva-5");

    /* Provide the cluster object as configuration to connect the session */
    connect_future = cass_session_connect_keyspace(session, cluster, "case18");
    CassError rc = cass_future_error_code(connect_future);
    cass_future_free(connect_future);
    CassFuture *prepare_future = cass_session_prepare(session, "SELECT * FROM case18.particle WHERE partid=?");
    prepared = cass_future_get_prepared(prepare_future);
    cass_future_free(prepare_future);


    /* ERROR BLOCK */
    if (rc != CASS_OK) {
        printf("it is not OK, man!\n");
        cerr << "cerr-message\n";
        fprintf(stderr, "stderr-message\n");
        cass_future_free(connect_future);
        return -1;
    } else {
        CassFuture *close_future = NULL;

        if (session==NULL) cout << "alr null" << endl;
        time_t t_start,t_end;
        time(&t_start);

        run_test();
        time(&t_end);

        cout << "done in " << difftime(t_end,t_start) << endl;
        /* Close the session */
        close_future = cass_session_close(session);
        cass_future_wait(close_future);
        cass_future_free(close_future);

    }

    cass_future_free(connect_future);
    cass_cluster_free(cluster);
    cass_session_free(session);
}