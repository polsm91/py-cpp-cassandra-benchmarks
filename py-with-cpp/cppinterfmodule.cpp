#include <Python.h>
#include "cppinterf.h"
#include <cassandra.h>
#include <atomic>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <atomic>
#define PRINT_DEBUG_MESSAGES

const CassPrepared *prepared = NULL;
CassSession *session = NULL;
CassCluster *cluster = NULL;


std::atomic <int> count(0);
class FromTo{
public:
    int from;
    int to;

    FromTo(int f,int t){
        from=f;
        to=t;

    };
};



void verify(CassFuture *future, void *ptr);

void call(void *ptr) {

    FromTo *range = (FromTo *) ptr;
    range->from += 1;
    if (range->from > range->to) {
        return;
    }
    CassStatement *statement = cass_prepared_bind(prepared);
    cass_statement_bind_int32(statement, 0, range->from );

    CassFuture *query_future = cass_session_execute(session, statement);

    cass_statement_free(statement);
    cass_future_set_callback(query_future, verify, range);
    cass_future_free(query_future);
}

#undef PRINT_DEBUG_MESSAGES

void verify(CassFuture *future, void *ptr) {

    CassError rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        printf("%s \n", cass_error_desc(rc));
    }

#ifdef PRINT_DEBUG_MESSAGES
    const CassResult* result = cass_future_get_result(future);

/* If there was an error then the result won't be available */
    if (result == NULL) {
        /* Handle error */
        std::cout << "Error async" << std::endl;
        return;
    }

    int row_n = 0;
    CassIterator* row_iterator = cass_iterator_from_result(result);
    while (cass_iterator_next(row_iterator)) ++row_n;
    cass_iterator_free(row_iterator);
    cass_result_free(result);
    std::cout << "Rows number: " << row_n << std::endl;
#endif
    call(ptr);
}



void close_cas() {
    cass_prepared_free(prepared);
    CassFuture *close_future = NULL;
/* IF SETUP SUCCESFUL */
    close_future = cass_session_close(session);
    cass_future_wait(close_future);
    cass_future_free(close_future);

/* ALWAYS */
    cass_cluster_free(cluster);
    cass_session_free(session);
}






void setup() {
    CassFuture *connect_future = NULL;
    cluster = cass_cluster_new();
    session = cass_session_new();

    /* Add contact points */
    cass_cluster_set_contact_points(cluster, "minerva-5");
    /* Provide the cluster object as configuration to connect the session */
    connect_future = cass_session_connect_keyspace(session, cluster, "case18");
    CassError rc = cass_future_error_code(connect_future);
    if (rc != CASS_OK) {
        std::cout << "error on setup" << std::endl;
    }
    cass_future_free(connect_future);
    CassFuture *prepare_future = cass_session_prepare(session, "SELECT * FROM case18.particle WHERE partid=?");
    prepared = cass_future_get_prepared(prepare_future);
    cass_future_free(prepare_future);
}

void worker(int start, int end) {
    FromTo *range=new FromTo(start,end);
    call(range);
}

void destroy_globals() {
    close_cas();
}



void init_globals() {
    if (0 != atexit(destroy_globals)) {
        std::cerr << "annoy::destroy_globals not installed" << std::endl;
        exit(-1);
    }
    setup();
}
