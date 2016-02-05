#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <tclap/CmdLine.h>

using namespace boost::asio::ip;
using namespace std;
using namespace TCLAP;

int main(int argc, char** argv) {
  /*if(argc != 4) {
    cerr << "Usage: halfwit <server> <channel> <username> <password>\n";
    return 1;
  }
  static const string SERVER = argv[1];
  static const string CHANNEL = argv[2];
  static const string USERNAME = argv[3];
  static const string PASSWORD = argv[4];*/
  try {

    CmdLine cmd("Halfwit - The dumb IRC client", ' ', "0.1");
    ValueArg<string> HostArg("s", "server", "Server to connect to", false, "127.0.0.1", "server", cmd);
    ValueArg<string> PortArg("p", "port", "Port to connect to", false, "6667", "port", cmd);
    ValueArg<string> ChannelArg("c", "channel", "Channel to join", false, "test", "channel", cmd);

    cmd.parse(argc, argv);    


    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query (HostArg.getValue(), PortArg.getValue());

    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while(error && endpoint_iterator != end) {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if(error)
      throw boost::system::system_error(error);

    string user_msg = "USER halfwitbot myhost server :halfwitbot\n";
    string nick_msg = "NICK halfwitbot\n";
    string join_msg = "JOIN #" + ChannelArg.getValue() + "\n";
    socket.write_some(boost::asio::buffer(user_msg), error);
    socket.write_some(boost::asio::buffer(nick_msg), error);
    socket.write_some(boost::asio::buffer(join_msg), error);

    if(error)
      throw boost::system::system_error(error);

    for(;;) {
      vector<char> buf(128);
      buf.clear();
      socket.read_some(boost::asio::buffer(buf), error);
      if(error == boost::asio::error::eof)
        break;
      else if(error)
        throw boost::system::system_error(error);

      const string first_four(buf.begin(), buf.begin() + 4);
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
