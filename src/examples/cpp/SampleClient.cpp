#include <unistd.h>
#include <cassert>
#include <boost/shared_ptr.hpp>

#include "gen-cpp/DataHub.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

/**
 * Sample DataHub C++ Client
 *
 * @author anantb
 * @author stephentu
 * @date 11/07/2013
 *
 */

using namespace std;
using namespace boost;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace datahub;

int main () {
  try {
    shared_ptr<THttpClient> transport(new THttpClient("datahub.csail.mit.edu", 80, "/service"));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    DataHubClient client(protocol);

    transport->open();
    double version = client.get_version();
    cout << version << endl;

    ConnectionParams params = ConnectionParams();
    params.__set_user("anantb");
    params.__set_password("anant");

    Connection conn;
    client.open_connection(conn, params);

    ResultSet res;
    client.execute_sql(
      res, conn, "select * from anantb.test.demo", vector<string>());
    for(
        const auto* tuple_it = res.tuples.begin();
        tuple_it != res.tuples.end();
        ++tuple_it) {
      for(
          const auto* cell_it = (*tuple_it).cells.begin();
          cell_it != (*tuple_it).cells.end(); 
          ++cell_it) {
        std::cout << *cell_it << "\t";
      }
      std::cout << std::endl;
    }

    transport->close();
  } catch (TException &tx) {
    cout << "ERROR: " << tx.what();
  }

  return 0;
}
