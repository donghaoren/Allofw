// net_utils.h
// network utilities

#ifndef NET_UTILS_H
#define NET_UTILS_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define SOL_TCP IPPROTO_TCP

#ifdef WIN32
#include <winsock.h>
#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
#endif

/* notes

struct sockaddr {
  sa_family_t sa_family; // address family, AF_xxx
  char sa_data[14];      //  14 bytes of protocol address
};

struct sockaddr_in {
  sa_family_t sin_family;  // address family, AF_xxx
  in_port_t sin_port;      // Port number
  struct in_addr sin_addr; // Internet address
}

struct in_addr {
  __u32   s_addr;
};

*/

typedef int socket_t;

// local functions
void nonblock(int s);
socket_t listen_tcp(unsigned short port);
socket_t listen_udp(unsigned short port);
socket_t connect_tcp(const char *netaddress, int port);
socket_t connect_udp(const char *netaddress, int port);
socket_t setup_listening(struct sockaddr *addr, int domain, int type, unsigned short port);
socket_t setup_connection(struct sockaddr *addr, int domain, int type, const char *netaddress, int port);
int udp_recv(socket_t sock, struct sockaddr_in *addr, char *buf, size_t count);
int udp_send(socket_t sock, struct sockaddr *addr, const char *buf, size_t count);

#ifndef WIN32
// this seems to be the cleanest way to deal with winsock
#define closesocket(s) close(s)
#endif

#ifdef WIN32
inline void winsock_init()
{
  WSADATA wsadata;
  int ret = WSAStartup(2, &wsadata);
  //cout << "WSAStartup=" << ret << endl;
  if(ret)
    {
      perror("WSAStartup FAILED");
      exit(0);
    }
}

inline void winsock_cleanup()
{
  int ret = WSACleanup();
  //cout << "WSACleanup=" << ret << endl;
}
#endif

inline void nonblock(int fd)
{
#ifdef WIN32
  ULONG argp = 1;
  if(ioctlsocket(fd, FIONBIO, &argp))
  {
    perror("ioctlsocket");
    exit(-1);
  }
#else
  int flags = fcntl(fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  if(fcntl(fd, F_SETFL, flags) < 0) 
    {
      perror("fcntl");
      exit(-1);
    }
#endif
}

// enable close-on-exec flag
inline void nonexec(int fd)
{
#ifndef WIN32
  if(fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
    {
      perror("fcntl");
      exit(-1);
    }
#endif
}

inline socket_t listen_tcp(unsigned short port)
{
  struct sockaddr addr;
  return setup_listening(&addr, PF_INET, SOCK_STREAM, port);
}

inline socket_t listen_udp(unsigned short port)
{
  struct sockaddr addr;
  return setup_listening(&addr, PF_INET, SOCK_DGRAM, port);
}

inline socket_t connect_tcp(const char *netaddress, int port)
{
  struct sockaddr addr;
  return setup_connection(&addr, PF_INET, SOCK_STREAM, netaddress, port);
}

inline socket_t connect_udp(const char *netaddress, int port)
{
  struct sockaddr addr;
  return setup_connection(&addr, PF_INET, SOCK_DGRAM, netaddress, port);
}

inline unsigned int lookup_host(const char *netaddress)
{
  struct in_addr *net_addr;
  struct hostent *host;

  host = gethostbyname(netaddress);

  if(!host)
    {
#ifdef WIN32
      fprintf(stderr, "%s: error %d\n", netaddress, WSAGetLastError());
#else
      fprintf(stderr, "%s: %s\n", netaddress, hstrerror(errno));
#endif
      return 0;
    }

  net_addr = (struct in_addr*)*host->h_addr_list;

  return net_addr->s_addr;
}

inline int getbroadcast(socket_t sock, int &net_addr)
{
#ifdef WIN32  
  net_addr = htonl(INADDR_BROADCAST);
  return 1;
#else
  struct ifreq ifr = {{ "eth0" }};

  if(ioctl(sock, SIOCGIFBRDADDR, &ifr) == -1)
    {
      fprintf(stderr, "getbroadcast: %s: %s\n", ifr.ifr_name, strerror(errno));
      return -1;
    }
  
  if(ifr.ifr_broadaddr.sa_family != AF_INET)
    {
      fprintf(stderr, "getbroadcast: unsupported address family\n");
      return -1;
    }
  
  net_addr = ((struct sockaddr_in*)&ifr.ifr_broadaddr)->sin_addr.s_addr;
  
  return 1;
#endif
}

inline int gethostip(char *name, size_t len)
{
  int ret = gethostname(name, len);
  if(ret != 0) return -1;

  struct hostent *host = gethostbyname(name);
  if(host == 0) return -1;

  char *ptr = inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
  if(ptr == 0) return -1;

  strncpy(name, ptr, len);

  return 1;
}

// set net addresss and port of addr
// return sizeof(sockaddr_in)
inline int set_addr_in(struct sockaddr *addr, int net_address, int port)
{
  struct sockaddr_in *addr_in = (struct sockaddr_in*)addr;
  int addrlen = sizeof(struct sockaddr_in);

  memset((char*)addr_in, 0, addrlen);

  addr_in->sin_family = AF_INET;
  addr_in->sin_port = port;
  addr_in->sin_addr.s_addr = net_address;

  return addrlen;
}

inline socket_t setup_listening(struct sockaddr *addr, int domain, int type, unsigned short port)
{
  socket_t listening_socket = -1;
  int addrlen;
  int reuse_addr = 1;
  int nodelay = (type == SOCK_STREAM ? 1 : 0);
  int ret;
  
  // create socket of domain and type
  listening_socket = socket(domain, type, 0);
  if(listening_socket < 0)
    {
      perror("socket");
      exit(-1);
    }

  nonexec(listening_socket);

  // create approprate sockaddr data
  if(domain == PF_INET)
    addrlen = set_addr_in(addr, INADDR_ANY, htons(port));
  else
    {
      fprintf(stderr, "setup_listening: unsupported address family\r\n");
      exit(-1);
    }

  // reuse the address
  if(setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR,
		(char*)&reuse_addr, sizeof(reuse_addr)) == -1)
    {
      perror("setsockopt");
      exit(0);
    }

  // no delay
  if(nodelay && setsockopt(listening_socket, SOL_TCP, TCP_NODELAY, 
			   (char*)&nodelay, sizeof(nodelay)) == -1)
    {
      perror("setsockopt");
      exit(-1);
    }

  // bind socket to INADDR_ANY:port
  ret = ::bind(listening_socket, addr, addrlen);
  if(ret < 0)
    {
      perror("bind");
      closesocket(listening_socket);
      return -1;
    }

  if(type == SOCK_STREAM)
    {
      // listen on socket
      listen(listening_socket, 5);
    }

  return listening_socket;
}

inline socket_t setup_connection(struct sockaddr *addr, int domain, int type, const char *netaddress, int port)
{
  socket_t sock;
  int addrlen;
  int ret;
  int net_addr = -1;
  int nodelay = (type == SOCK_STREAM ? 1 : 0);
  int keepalive = 0;

  // create socket
  sock = socket(domain, type, 0);
  if(sock == -1)
    {
      perror("socket");
      exit(-1);
    }

  // conver netaddress sting into a numerical network address
  if(netaddress)
    {
      net_addr = lookup_host(netaddress);
      if(net_addr == -1)
        {
          fprintf(stderr, "error: invalid host address\n");
          closesocket(sock);
          return -1;
        }
    }
  else if(getbroadcast(sock, net_addr) == -1)
    {
      fprintf(stderr, "error: could not get broadcast address\n");
      closesocket(sock);
      return -1;
    }

  // create approprate sockaddr data
  if(domain == PF_INET)
    addrlen = set_addr_in(addr, net_addr, htons(port));
  else
    {
      fprintf(stderr, "unknown domain\n");
      closesocket(sock);
      return -1;
    }
  
  nonexec(sock);

  // no delay
  if(nodelay && setsockopt(sock, SOL_TCP, TCP_NODELAY, 
			   (char*)&nodelay, sizeof(nodelay)) == -1)
    {
      perror("setsockopt");
      exit(-1);
    }

  if(keepalive && setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, 
			     (char*)&keepalive, sizeof(keepalive)) == -1)
    {
      perror("setsockopt");
      exit(-1);
    }
  
  if(type == SOCK_STREAM)
    {
      ret = connect(sock, addr, addrlen);
      if(ret < 0) 
        {
#ifdef WIN32
          fprintf(stderr, "connect to %s: %d\n", netaddress, WSAGetLastError());
#else
          fprintf(stderr, "connect to %s: %s\n", netaddress, strerror(errno));
#endif

          closesocket(sock);
          return ret;
        }
      
      return sock;      
    }
  
  if(type == SOCK_DGRAM)
    {
#if 0
      ret = connect(sock, addr, addrlen);
      if(ret < 0)
        {
          fprintf(stderr, "connect to %s: %s\n", netaddress, strerror(errno));
          closesocket(sock);
          return ret;
        }
#endif
      return sock;
    }

  closesocket(sock);
  return -1;
}

inline int udp_recv(socket_t sock, struct sockaddr_in *addr, char *buf, size_t count)
{
#ifdef WIN32
  int addrlen = sizeof(struct sockaddr_in);
#else
  socklen_t addrlen = sizeof(struct sockaddr_in);
#endif

  int ret = recvfrom(sock, buf, count, 0, (struct sockaddr*)addr, &addrlen);
  if(ret < 0)
    {
#ifdef WIN32
      int err = WSAGetLastError();
      if(err == WSAEWOULDBLOCK) return 0;
#else
      if(errno == EAGAIN) return 0;
#endif
      perror("recvfrom");
      return ret;
    }

  return ret;
}

inline int udp_send(socket_t sock, struct sockaddr *addr, const char *buf, size_t count)
{
#ifdef WIN32
  int addrlen = sizeof(struct sockaddr);
#else
  socklen_t addrlen = sizeof(struct sockaddr);
#endif

  int ret = sendto(sock, buf, count, 0, addr, addrlen);

  if(ret < 0)
    {
#ifdef WIN32
      int err = WSAGetLastError();
      if(err == WSAEWOULDBLOCK) return 0;
#else
      if(errno == EAGAIN) return 0;
#endif
      perror("sendto");
      return ret;
    }
  return ret;
}

#endif // NET_UTILS_H
