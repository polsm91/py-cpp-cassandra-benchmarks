#include <atomic>
#include <iostream>
#include <thread>

#include <unistd.h>
#include <time.h>

#include <cassandra.h>
#include <utility>

using namespace std;
int n_keys = 10000;
int max_par = 8;

const CassPrepared *prepared = NULL;
CassSession *session = cass_session_new();
bool finished = false;
atomic <int> count(0);
class FromTo{
public:
    int from;
    int to;

    FromTo(int f,int t){
        from=f;
        to=t;

    };
};

void call(CassFuture *future, void *ptr) {

    FromTo *range = (FromTo *) ptr;
  //  cout << "my range" << range->from << " " << range->to << endl;
         CassError rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        printf("%s %d\n", cass_error_desc(rc), range->from);
    }

    range->from += 1;
    if (range->from > range->to) {
  //      cout << "i finished" << endl;

        if(count.fetch_add(1)==max_par-1){
            finished=true;
        }
    //    cout << "count val: " << count << endl;
        return;
    }


    CassStatement *statement = cass_prepared_bind(prepared);
    cass_statement_bind_int32(statement, 0, range->from );

    CassFuture *query_future = cass_session_execute(session, statement);

    cass_statement_free(statement);
    cass_future_set_callback(query_future, call, range);
    cass_future_free(query_future);
}


int main(int argc, char *argv[]) {

    max_par = atoi(argv[1]);
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


        int chunk_size = n_keys/max_par;

        time_t t_start, t_end;
        time(&t_start);
        //printf("sending key %d\n", i);
        for (int n = 0; n<max_par; ++n) {
            FromTo *range=new FromTo(n*chunk_size+1,n*chunk_size+chunk_size);
            CassStatement *statement = cass_prepared_bind(prepared);
            cass_statement_bind_int32(statement, 0, range->from);

            CassFuture *query_future = cass_session_execute(session, statement);
            cass_statement_free(statement);

            cass_future_set_callback(query_future, call, range);
            cass_future_free(query_future);

        }

        //cout << "n_th: " << max_par << " " <<  << endl;


        while (!finished) ;


        time(&t_end);
        cout << difftime(t_end,t_start) << endl;
        /* Close the session */
        close_future = cass_session_close(session);
        cass_future_wait(close_future);
        cass_future_free(close_future);

    }

    cass_future_free(connect_future);
    cass_cluster_free(cluster);
    cass_session_free(session);
}