#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

using namespace boost::asio::ip;
using namespace std;

int main(int argc, char** argv) {
  if(argc != 3) {
    cerr << "Usage: halfwit <server> <channel>\n";
    return 1;
  }
  try {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query (argv[1], "6667");

    cout << "resolving " << argv[1] << endl;
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    cout << "connecting...";
    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while(error && endpoint_iterator != end) {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if(error)
      throw boost::system::system_error(error);
    cout << "connected!\n";

    string user_msg = "USER halfwitbot myhost server :halfwitbot\n";
    string nick_msg = "NICK halfwitbot\n";
    string join_msg = "JOIN #" + string(argv[2]) + "\n";
    socket.write_some(boost::asio::buffer(user_msg), error);
    socket.write_some(boost::asio::buffer(nick_msg), error);
    socket.write_some(boost::asio::buffer(join_msg), error);

    if(error)
      throw boost::system::system_error(error);

    for(;;) {
      vector<char> buf(128);
      socket.read_some(boost::asio::buffer(buf), error);
      if(error == boost::asio::error::eof)
        break;
      else if(error)
        throw boost::system::system_error(error);

      string first_four(buf.begin(), buf.begin() + 4);
      if(first_four.compare("PING") == 0) {
        string pong_msg = string("PONG") + string(buf.begin() + 4, buf.end());
        socket.write_some(boost::asio::buffer(pong_msg), error);
        if(error)
          throw boost::system::system_error(error);
      }
      

      for(auto i: buf)
        cout << i;
    }
  }
  catch(std::exception &e) {
    cerr << "error: " << e.what() << endl;
  }
}
