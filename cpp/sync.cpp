#include <atomic>
#include <iostream>
#include <thread>

#include <unistd.h>
#include <vector>
#include <cassandra.h>
#include <chrono>

using namespace std;
int n_keys = 10000;

/*

thread *threads;
atomic<int> finished (0);
CassSession *session = NULL;
int n_threads = 4;

 void worker(int start, int end) {
    cout << "in " << start << " " << end <<  endl;
    while (start<=end) {
        cass_statement_bind_int32(statement,0, start);
        cass_session_execute(session, statement);
        cass_statement_free(statement); start+=1;
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
*/

int main() {

    /* SETUP CASSANDRA DRIVER */
    CassFuture *connect_future = NULL;
    CassCluster *cluster = cass_cluster_new();
    CassSession *session = cass_session_new();

    /* Add contact points */
    cass_cluster_set_contact_points(cluster, "minerva-5");

    /* Provide the cluster object as configuration to connect the session */
    connect_future = cass_session_connect_keyspace(session, cluster, "case18");
    CassError rc = cass_future_error_code(connect_future);
    cass_future_free(connect_future);
    CassFuture *prepare_future = cass_session_prepare(session, "SELECT * FROM case18.particle WHERE partid=?");
    const CassPrepared *prepared = cass_future_get_prepared(prepare_future);
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

        //auto start = chrono::steady_clock::now();

        time_t t_start,t_end;
        time(&t_start);
        for (int i = 0; i < n_keys; ++i) {
            CassStatement *statement = cass_prepared_bind(prepared);
            cass_statement_bind_int32(statement, 0, i);
            CassFuture* query_future = cass_session_execute(session, statement);
            cass_statement_free(statement);
            CassError rc = cass_future_error_code(query_future);

            if (rc != CASS_OK) {
                printf("it is not OK, man!\n");
                cerr << "cerr-message\n";
                fprintf(stderr, "stderr-message\n");
            }
            cass_future_error_code(query_future);
            cass_future_free(query_future);
        }

        time(&t_end);

       // auto duration = (chrono::steady_clock::now() - start);
        //double seconds = double(duration.count()) * chrono::steady_clock::period::num / chrono::steady_clock::period::den;

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