// owl_utils.h

#ifndef OWL_UTILS_H
#define OWL_UTILS_H

#ifdef WIN32
#include <winsock.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#endif

#include <iostream>

#include <errno.h>
#include <string.h>
#include <assert.h>

using namespace std;

struct bad_socket_read {
};

struct bad_socket_write {
};

inline void owl_delay(int msec)
{
#ifdef WIN32
  Sleep(msec);
#else
  timeval t = {msec/1000, (msec % 1000)*1000};
  select(0, 0, 0, 0, &t);
#endif
}

inline int owl_send(int s, const char *buf, size_t count)
{
  if(count == 0) return 0;

  int ret = send(s, buf, count, 0);

  if(ret < 0)
    {
#ifdef WIN32
      int err = WSAGetLastError();
      if(err == WSAEWOULDBLOCK) return 0;
      cerr << "send:  error=" << err << ". (" << s << ")" << endl;
      if(err == WSAECONNRESET) return -1;
      if(err == WSAECONNABORTED) return -1;
      if(err == WSAENOBUFS) return -1;
#else
      if(errno == EAGAIN) return 0;
      cerr << "send: " << strerror(errno) << ". (" << s << ")" << endl;
      if(errno == EPIPE) return -1;
      if(errno == ECONNRESET) return -1;
#endif
      return -1; //throw bad_socket_write();
    }

  return ret;
}

inline int owl_recv(int s, char *buf, size_t count)
{
  if(count == 0) return 0;

  int ret = recv(s, buf, count, 0);

  if(ret == 0)
    {
      cout << "Connection closed by foreign host. (" << s << ")" << endl;
      return -1;
    }
  if(ret == -1)
    {
#ifdef WIN32
      int err = WSAGetLastError();
      if(err == WSAEWOULDBLOCK) return 0;
      cerr << "recv: error=" << err << ". (" << s << ")" << endl;
      if(err == WSAECONNRESET) return -1;
      if(err == WSAECONNABORTED) return -1;
#else
      if(errno == EAGAIN) return 0;
      cerr << "recv: " << strerror(errno) << ". (" << s << ")" << endl;
      if(errno == ECONNRESET) return -1;
#endif
      return -1; //throw bad_socket_read();
    }

  return ret;
}

// retry wrapper
inline int owl_send(int s, const char *buf, size_t count, size_t retry)
{
  size_t t;
  int ret, size = 0;

  if(retry == 0) return owl_send(s, buf, count);

  for(t = 0; t <= retry && count > 0; t++)
    {
      ret = owl_send(s, buf, count);
      if(ret < 0) return size?size:ret;
      if(ret > 0)
	{
	  size += ret;
	  buf += ret;
	  count -= ret;
	}
      else owl_delay(1);
    }
  
  if(t > 1) cout << " retry#" << t << "[" << s << "] " << count+size << endl;

#if 0
  if(count > 0 && retry > 0)
    {
      cerr << "send(" << count+size << ") failed, terminating connection." << endl;
      //assert(!"send() failed after number of retrys");
      return -2;
    }
#endif

  return size;
}

// retry wrapper
inline int owl_recv(int s, char *buf, size_t count, size_t retry)
{
  size_t t;
  int ret, size = 0;

  if(retry == 0) return owl_recv(s, buf, count);

  for(t = 0; t <= retry && count > 0; t++)
    {
      ret = owl_recv(s, buf, count);
      if(ret < 0) return size?size:ret;
      if(ret > 0)
	{
	  size += ret;
	  buf += ret;
	  count -= ret;
	}
      else owl_delay(1);
    }

#if 0
  if(count > 0 && retry > 0)
    {
      cerr << "recv() failed, terminating connection." << endl;
      return -2;
    }
#endif

  return size;
}

// listen on a single read or write socket
inline int owl_select(int rsock, int wsock, int timeout)
{
  timeval tv = {0, timeout};
  fd_set rfds, wfds;
  FD_ZERO(&rfds);
  FD_ZERO(&wfds);
  if(rsock > 0) FD_SET(rsock, &rfds);
  if(wsock > 0) FD_SET(wsock, &wfds);

  int s = rsock > wsock ? rsock : wsock;
  if(s > 0) s += 1;

  int ret = select(s, &rfds, &wfds, 0, &tv);

  if(ret < 0)
    {
      if(errno == EINTR) return 0;
      cout << "select: " << errno << " " << strerror(errno) << endl;
      return ret;
    }

  if(ret > 0)
    assert(FD_ISSET(rsock, &rfds) || FD_ISSET(wsock, &wfds));

  return ret;
}

inline int owl_listen(int sock, int timeout)
{
  int new_sock = -1;
  int ret = owl_select(sock, 0, timeout);

  if(ret > 0)
    {
      new_sock = accept(sock, 0, 0);
      //if(new_sock > 0) cout << "accept=" << new_sock << endl;
      if(new_sock < 0) cerr << "error: accept: " << errno << " " << strerror(errno) << endl;
      assert(sock != 0 || !"accept should never return 0");
    }
  return new_sock;
}

#endif // OWL_UTILS_H
