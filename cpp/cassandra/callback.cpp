#include <atomic>
#include <iostream>
#include <thread>

#include <unistd.h>
#include <time.h>

#include <cassandra.h>


using namespace std;
int n_keys = 10000;

const CassPrepared *prepared = NULL;
CassSession *session = cass_session_new();
bool finished = false;

void call(CassFuture *future, void *ptr) {

    int *key = (int *) ptr;
    CassError rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        printf("%s %d\n", cass_error_desc(rc), *key);
    }
    //printf("%d\n",*key);
    *key = *key + 1;
    if (*key > n_keys) {
        finished = true;
        return;
    }


    CassStatement *statement = cass_prepared_bind(prepared);
    cass_statement_bind_int32(statement, 0, *key);

    CassFuture *query_future = cass_session_execute(session, statement);

    cass_statement_free(statement);
    cass_future_set_callback(query_future, call, key);
    cass_future_free(query_future);
}


int main() {

    /* SETUP CASSANDRA DRIVER */
    CassFuture *connect_future = NULL;
    CassCluster *cluster = cass_cluster_new();

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

        /*int start,end, chunk_size;
        chunk_size = n_keys/n_threads;
        thread threads[n_threads];*/
        time_t t_start, t_end;
        time(&t_start);
        int i = 1;
        //printf("sending key %d\n", i);
        CassStatement *statement = cass_prepared_bind(prepared);
        cass_statement_bind_int32(statement, 0, i);


        CassFuture *query_future = cass_session_execute(session, statement);

        cass_statement_free(statement);

        cass_future_set_callback(query_future, call, &i);

        cass_future_free(query_future);

        //cout << "n_th: " << n_threads << " " <<  << endl;


        while (not finished) ;


        time(&t_end);
        cout << "done" << difftime(t_end,t_start) << endl;
        /* Close the session */
        close_future = cass_session_close(session);
        cass_future_wait(close_future);
        cass_future_free(close_future);

    }

    cass_future_free(connect_future);
    cass_cluster_free(cluster);
    cass_session_free(session);
}