// owl_scan.cc
// udp broadcast based scanning

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "net_utils.h"
#include "owl_scan.h"
#include "owl_utils.h"
#include "timer.h"

using namespace std;

// timeout is in microseconds (usec)
int owl_scan(const char *address, int port, const char *msg, size_t timeout, OWLConnection *cons, size_t count)
{
  Timer timer;
  struct sockaddr out_addr;
  struct sockaddr_in in_addr;
  char buf[1024];
  int n = 0; // number of connections

  // connect
  socket_t sock = setup_connection(&out_addr, PF_INET, SOCK_DGRAM, address, port);
  if(sock < 0)
    {
      cerr << "error: setup_connection failed" << endl;
      return sock;
    }

  nonblock(sock);

  int broadcast = 1;
  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) < 0)
    {
      perror("setsockopt(BROADCAST)");
      exit(-1);
    }

  // broadcast
  int ret = udp_send(sock, &out_addr, msg, strlen(msg)+1);
  if(ret < 0) goto done;

  // listen
  for(size_t t = 0, t0 = timer.get(); t < timeout; t = timer.get()-t0)
    {
      // select here
      ret = owl_select(sock, 0, timeout-t);

      if(ret < 0) goto done;

      // timeout
      if(ret == 0) continue;
      
      ret = udp_recv(sock, &in_addr, buf, 1024);
      buf[ret] = 0;
      
      if(ret > 0 && in_addr.sin_family == AF_INET)
        {
          //cout << inet_ntoa(in_addr.sin_addr) << ": " << buf << endl;
          if(n < (int)count)
            {
              strncpy(cons[n].address, inet_ntoa(in_addr.sin_addr), 256);
              strncpy(cons[n].info, buf, 256);
              // find port
              char *port = strchr(cons[n].info, ':');
              if(port) strncat(cons[n].address, port, 256);
              n++;
            }
        }
    }

  // cleanup
 done:
  
  closesocket(sock);

  return n ? n : ret;
}

//// deprecated -- will be removed ////

namespace SCAN {

  struct Connection {
    
    std::string address;
    std::string info;

    Connection(const std::string &address="", const std::string info="") :
      address(address), info(info) { }
  };

  typedef std::vector<Connection> Connections;
  OWLAPI int owl_scan(const char *address, int port, const std::string &msg, Connections &cons, size_t timeout);

} // SCAN

// timeout is in microseconds (usec)
int SCAN::owl_scan(const char *address, int port, const string &msg, Connections &cons, size_t timeout)
{
  Timer timer;
  struct sockaddr out_addr;
  struct sockaddr_in in_addr;
  char buf[1024];

  // connect
  socket_t sock = setup_connection(&out_addr, PF_INET, SOCK_DGRAM, address, port);
  if(sock < 0)
    {
      cerr << "error: setup_connection failed" << endl;
      return sock;
    }

  nonblock(sock);

  int broadcast = 1;
  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) < 0)
    {
      perror("setsockopt(BROADCAST)");
      exit(-1);
    }

  // broadcast
  int ret = udp_send(sock, &out_addr, msg.c_str(), msg.size()+1);
  if(ret < 0) goto done;

  // listen
  for(size_t t = 0, t0 = timer.get(); t < timeout; t = timer.get()-t0)
    {
      // select here
      ret = owl_select(sock, 0, timeout-t);

      if(ret < 0) goto done;

      // timeout
      if(ret == 0) continue;
      
      ret = udp_recv(sock, &in_addr, buf, 1024);
      
      if(ret > 0 && in_addr.sin_family == AF_INET)
        {
          //cout << inet_ntoa(in_addr.sin_addr) << ": " << buf << endl;
          cons.push_back(Connection(string(inet_ntoa(in_addr.sin_addr)), string(buf, ret)));
        }
    }

  // cleanup
 done:
  
  closesocket(sock);

  return cons.size() ? cons.size() : ret;
}

////

#if 0 // test
int main(int argc, char **argv)
{
#ifdef WIN32
  winsock_init();
#endif
  
  Connections cons;
  string msg(argv[0]); // put whatever here
  char *addr = argc > 1 ? argv[1] : 0;

  cout << "sendto: " << (addr?addr:"broadcast") << endl;

  int ret = owl_scan(addr, 8999, msg, cons, 1000000);
  if(ret < 0)
    {
      cerr << "error: connection check failed: ret=" << ret << endl;
      return -1;
    }

  cout << "found " << cons.size() << " servers:" << endl;
  for(size_t i = 0; i < cons.size(); i++)
    {
      cout << cons[i].address << ": " << cons[i].info << endl;
    }
#ifdef WIN32
  winsock_cleanup();
#endif
}
#endif
