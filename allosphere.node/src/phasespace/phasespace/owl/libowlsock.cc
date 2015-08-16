// libowlsock.cc
// network enabled OWL library
// OWL v1.3

#define O_LARGEFILE (0)

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

#ifdef WIN32
#include <io.h>
#endif

#include "owl.h"
#include "owl_images.h"
#include "owl_planes.h"
#include "owl_rpd.h"
#include "owl_protocol.h"
#include "owl_math.h"
#include "owl_utils.h"
#include "net_utils.h"
#include "buffer.h"
#include "c3d_file.h"
#include "timer.h"

using namespace std;

#define DBG(msg)// msg

Timer timer;

struct OWLFloats {
  typedef float type;
  OWLenum pname;
  int frame;
  vector<float> data;
  OWLFloats(OWLenum pname, int frame) : pname(pname), frame(frame) { }
};

struct OWLIntegers {
  typedef int type;
  OWLenum pname;
  int frame;
  vector<int> data;
  OWLIntegers(OWLenum pname, int frame) : pname(pname), frame(frame) { }
};

struct OWLString {
  typedef char type;
  OWLenum pname;
  int frame;
  vector<char> data;
  OWLString(OWLenum pname, int frame) : pname(pname), frame(frame) { }
};

#define OPERATOR_EQ(T) bool operator==(const T &a, const T &b) { return a.pname == b.pname; }

OPERATOR_EQ(OWLFloats);
OPERATOR_EQ(OWLIntegers);
OPERATOR_EQ(OWLString);


typedef list<int> Errors;
typedef list< vector<OWLMarker> > Markers;
typedef list< vector<OWLRigid> > Rigids;
typedef vector<OWLCamera> Cameras;
typedef list<OWLFloats> Floats;
typedef list<OWLIntegers> Integers;
typedef list<OWLString> Strings;
typedef list< vector<OWLImage> > Images;
typedef list< vector<OWLDetectors> > Detectors;
typedef list< vector<OWLPlane> > Planes;

struct _RPDClient : public buffer<char> {

  socket_t sock;
  int fd;
  int _mode;

  int _write, _read, _send, _recv, _max_size;

  _RPDClient(size_t buffer_size, size_t erase_delay) :
    buffer<char>(buffer_size, erase_delay), sock(-1), fd(-1), _mode(0),
    _write(0), _read(0), _send(0), _recv(0), _max_size(0) { }
  ~_RPDClient() { Close(); }

  int Open(const char *servername, const char *filename, int mode);
  void Disconnect();
  void Close();
  int Send(size_t retry=0, size_t txsize=0);
  int Recv();
};

struct RecordState {
  size_t startTime;
  size_t index;
  float freq;

  vector< vector<OWLMarker> > frames;

  RecordState()
    : startTime(0), index(0), freq(0)
  {}

  int Init(const char *server)
  {
    Reset();
    cout << "loading c3d file; " << server << endl;
    int ret = load_c3d(server, frames);

    if(ret > 0)
      {
        ret = frequency_c3d(server, &freq);
        index = 0;
        startTime = timer;

        cout << "c3d loaded: frames=" << frames.size() << " @ " << freq << "Hz" << endl;
      }

    return ret;
  }

  void Reset()
  {
    startTime = index = 0;
    freq = 0;
    frames.clear();
  }

  bool frameAvailable()
  {
#if 0
    size_t now = timer;
    size_t totalTime = now - startTime;
    size_t frameTime = (size_t)((1 / freq) * 1000000);
    size_t totalFramesDesired = totalTime / frameTime;

    return index < totalFramesDesired;
#else
    return ( index < ((timer.get() - startTime) / (((1.0 / freq) * 1000000.0))) );
#endif
  }
};

struct _OWLState {

  // client state
  int init;
  int flags;

  float pose[7];
  float scale;
  float m[16];

  // pose stack
  // [0] core transform
  // [1] client transform
  float p_stack[2][7];

  int frameNumber;
  int exiting;
  OWLenum lastAck;

  Errors errors;
  Markers markers;
  Rigids rigids;
  Cameras cameras;
  Floats floats;
  Integers integers;
  Strings strings;
  Images images;
  Detectors detectors;
  Planes planes;

  // file support
  RecordState recState;

  _OWLState()
  {
    InitClientState();
  }

  operator void*() { return init > 0 ? (void*)(-1) : (void*)0; }
  int operator!() { return init > 0 ? 0 : 1; }

  void InitClientState()
  {
    init = 0;
    flags = 0;

    for(int i = 0; i < 7; i++)
      pose[i] = p_stack[0][i] = p_stack[1][i] = 0;
    pose[3] = p_stack[0][3] = p_stack[1][3] = 1;
    scale = 1;
    convert_pm(pose, m);

    frameNumber = 0;
    exiting = 0;
    lastAck = 0;

    errors.clear();
    markers.clear();
    rigids.clear();
    cameras.clear();
    floats.clear();
    integers.clear();
    strings.clear();
    images.clear();
    detectors.clear();
    planes.clear();
    recState.Reset();
  }

  int IsEmpty()
  {
    int n = 0
      + markers.size()
      + rigids.size()
      + images.size()
      + detectors.size()
      + planes.size()
      ;
    return n == 0;
  }
};

class _OWLClient {
public:

  enum { MAX_DATA_SIZE = 1024*64, BUF_SIZE = 0x400000 };

  int sock;
  int broadcast;

  struct sockaddr_in addr;

  _OWLHeader *in_header;
  _OWLHeader *out_header;

  std::buffer<char> in_buffer;

  _OWLClient() : sock(-1), in_buffer(BUF_SIZE)
  {
    in_header = (_OWLHeader*)new char[MAX_DATA_SIZE];
    out_header = (_OWLHeader*)new char[MAX_DATA_SIZE];

#ifdef WIN32
    winsock_init();
#endif
  }

  ~_OWLClient()
  {
    delete[] in_header;
    delete[] out_header;
#ifdef WIN32
    winsock_cleanup();
#endif
  }

  _OWLHeader *Out(packet_t type, int size)
  {
    extern _OWLState _owl;
    assert(size + sizeof(_OWLHeader) < MAX_DATA_SIZE);
    out_header->type = type;
    out_header->size = size;
    out_header->frame = _owl.frameNumber;

    return out_header;
  }

  void Close()
  {
    if(sock == -1) return;
    cout << "closing socket: " << sock << endl;
    closesocket(sock);
    sock = -1;
    in_buffer.clear();
  }

  void Send()
  {
    if(sock == -1) return;
    int ret;

    if(out_header->size == 0) { cerr << "header->size == 0" << endl; return; }

    ret = owl_send(sock, (char*)out_header, sizeof(_OWLHeader) + out_header->size, 10);

    if(ret != (int)(out_header->size+sizeof(_OWLHeader)))
      {
	cerr << "send(" << (int)(out_header->size+sizeof(_OWLHeader)) << ") failed." << endl;
      }

    assert(ret == (int)(out_header->size+sizeof(_OWLHeader)));
  }

  // new networking code (buffered)
  int Recv()
  {
    int ret = 0;

    if(broadcast && sock > -1)
      {
        ret = udp_recv(sock, &addr, in_buffer.end(), in_buffer.available());
        //if(ret) cout << "udp_recv=" << ret << " of " << in_buffer.available() << endl;
        if(ret < 0) Close();
        if(ret > 0) in_buffer += ret;
      }
    else if(sock > -1)
      {
	ret = owl_recv(sock, in_buffer.end(), in_buffer.available());
	//if(ret) cout << "owl_recv=" << ret << " of " << in_buffer.available() << endl;
	if(ret < 0)
	  {
	    // don't try to recover data left in buffer(s), too much effort
	    void owl_done();
	    owl_done();

	    return ret;
	  }
	if(ret > 0) in_buffer += ret;
      }
    else if(in_buffer.size() == 0)
      {
	return -1;
      }

    if(in_buffer.size() <= sizeof(_OWLHeader)) return 0;

    _OWLHeader *h = (_OWLHeader*)in_buffer.begin();

    assert(h->size >= 0);

    size_t count = sizeof(_OWLHeader) + h->size;

    if(count > in_buffer.size()) return 0;

    memcpy((char*)in_header, (char*)h, count);

    in_buffer -= count;

    return in_header->size;
  }
};

OWLAPI size_t _rpd_chunk_size = 0x10000;

_OWLState _owl;
_OWLClient _client;
_OWLClient _broadcast;

int add_c3dmarkers();
int add_markers(_OWLHeader *p);
int add_rigids(_OWLHeader *p);
int add_cameras(_OWLHeader *p);
int add_config(_OWLHeader *p);
int add_floats(_OWLHeader *p);
int add_integers(_OWLHeader *p);
int add_string(_OWLHeader *p);
int add_images(_OWLHeader *p);
int add_detectors(_OWLHeader *p);
int add_planes(_OWLHeader *p);
void add_float(OWLenum pname,  const float *param, size_t count, int reuse=0);
void add_integer(OWLenum pname, const int *param, size_t count, int reuse=0);
void add_string(OWLenum pname, const char *param, size_t count, int reuse=0);


#define INT(data) ((int*)(data))
#define FLOAT(data) ((float*)(data))
#define ENUM(data) ((OWLenum*)(data))

int _owl_get_sock()
{
  if(!_owl) return -1;

  return _client.sock;
}

void owl_done()
{
  if(_owl.init) cout << "owl done" << endl;
  _owl.init = 0;

  _client.Close();
  _broadcast.Close();
}

// main packet handler
// return value:
//  success: 0
//  error:   <0 (-errno)
int owl_read()
{
  int ret;

  // delay exiting
  if(_owl.exiting)
    {
      if(_owl.IsEmpty())
	{
	  _owl.errors.push_back(0x1); // ERROR
          add_string(OWL_STATUS_STRING, "disconnected", 0);

	  owl_done();
	}
      return -1;
    }

  // file reading
  if(_owl.flags & OWL_FILE)
    {
      ret = 0;

      while(_owl.recState.frameAvailable())
        {
          ret = add_c3dmarkers();

          if(ret < 0)
            {
              _owl.exiting = 1;
              break;
            }

          _owl.recState.index++;
        }

      return ret < 0 ? ret : 0;
    }

  // socket reading
  int count = 0;
  do {
    int owl_parse(_OWLHeader *p);

    count = 0;

    // read _client
    ret = _client.Recv();

    //if(ret != 0) cout << "read: " << ret << endl;

    if(ret < 0)
      {
        // errno does not (always?) get set on recv()==0
        _owl.errors.push_back(0x1); // errno ERROR
        return ret;
      }

    if(ret > 0)
      {
        count += ret;
        ret = owl_parse(_client.in_header);
        if(ret < 0) break;
      }

    // read _broadcast
    if(_broadcast.sock > -1 && _broadcast.broadcast)
      {
        ret = _broadcast.Recv();

        //if(ret != 0) cout << "read: " << ret << endl;

        if(ret < 0) return ret;

        if(ret > 0)
          {
            count += ret;
            ret = owl_parse(_broadcast.in_header);
            if(ret < 0) break;
          }
      }

  } while(count);

  if(ret < 0) _owl.errors.push_back(ret);

  return ret < 0 ? ret : 0;
}

int owl_parse(_OWLHeader *p)
{
  int ret = 0;

  if(p->type > pNone && p->type < pLast)
    {
      if(p->frame > _owl.frameNumber) _owl.frameNumber = p->frame;
    }
  else
    {
      cerr << "error: owl_read: unknown packet type: " << p->type << endl;
      return 0;
    }

  if(p->type == pInit)
    {
      DBG(cout << "pInit" << endl);
      int flags = *INT(p->data+0);
      size_t version = *ENUM(p->data+4);

      assert(_owl.init == 0);

      cout << "server version: "
           << ((version&0xFF0000)>>16) << "."
           << ((version&0x00FF00)>>8) << "."
           << ((version&0x0000FF)>>0) << endl;

      size_t rversion = OWL_PROTOCOL_VERSION;
      if(version != rversion)
        {
          cerr << "warning: server version does not match client version: "
               << ((rversion&0xFF0000)>>16) << "."
               << ((rversion&0x00FF00)>>8) << "."
               << ((rversion&0x0000FF)>>0) << endl;
        }

      if(_owl.flags & OWL_SLAVE)
        {
          _owl.flags = flags | OWL_SLAVE;
        }
      else if(_owl.flags != flags)
        {
          _owl.errors.push_back(OWL_INVALID_VALUE);
          owlDone();
          return -1;
        }
      _owl.init = 1;
    }
  else if(p->type == pDone)
    {
      DBG(cout << "pDone" << endl);

      _owl.exiting = 1;

      return -1;
    }
  else if(p->type == pError)
    {
      int error = *INT(p->data+0);
      DBG(cout << "pError: " << hex << error << dec << endl);

      _owl.errors.push_back(error);
    }
  else if(p->type == pMarkers)
    {
      DBG(cout << "pMarkers: " << p->size/sizeof(_OWLMarkerData) << endl);
      if(p->size > 0) ret = add_markers(p);
    }
  else if(p->type == pRigids)
    {
      DBG(cout << "pRigids: " << p->size/sizeof(_OWLRigidData) << endl);
      ret = add_rigids(p);
    }
  else if(p->type == pCameras)
    {
      DBG(cout << "pCameras: " << p->size/sizeof(_OWLCameraData) << endl);
      ret = add_cameras(p);
    }
  else if(p->type == pConfig)
    {
      DBG(cout << "pConfig: " << p->size/sizeof(_OWLConfigData) << endl);
      ret = add_config(p);
    }
  else if(p->type == pFloatv)
    {
      DBG(cout << "pFloatv: " << p->size/4 << endl);
      ret = add_floats(p);
    }
  else if(p->type == pIntegerv)
    {
      DBG(cout << "pIntegerv: " << p->size/4 << endl);
      ret = add_integers(p);
    }
  else if(p->type == pString)
    {
      DBG(cout << "pString: " << p->size << endl);
      ret = add_string(p);
    }
  else if(p->type == pAck)
    {
      DBG(cout << "pAck: 0x" << hex << *ENUM(p->data+0) << dec << endl);
      _owl.lastAck = *ENUM(p->data+0);
      // do nothing
    }
  else if(p->type == pImages)
    {
      DBG(cout << "pImages: " << p->size/sizeof(_OWLImageData) << endl);
      ret = add_images(p);
    }
  else if(p->type == pDetectors)
    {
      DBG(cout << "pDetectors: " << p->size/sizeof(_OWLDetectorsData) << endl);
      ret = add_detectors(p);
    }
  else if(p->type == pPlanes)
    {
      DBG(cout << "pPlanes: " << p->size/sizeof(_OWLPlaneData) << endl);
      ret = add_planes(p);
    }
  else
    {
      cerr << "error: owl_read: unhandled packet type: 0x"
           << hex << p->type << dec << endl;
    }

  return ret;
}

template <int N, class A, class B>
void copy_v(const A *a, B *b)
{
  for(int i = 0; i < N; i++) b[i] = a[i];
}

void copy_marker(OWLMarker &m, const _OWLMarkerData &d, int frame)
{
  m.id = d.id;
  m.frame = frame;
  m.x = d.pos[0];
  m.y = d.pos[1];
  m.z = d.pos[2];
  m.cond = d.cond;
  m.flag = d.flag;
}

void copy_rigid(OWLRigid &r, const _OWLRigidData &d, int frame)
{
  r.id = d.id;
  r.frame = frame;
  copy_v<7>(d.pose, r.pose);
  r.cond = d.cond;
  r.flag = d.flag;
}

void copy_camera(OWLCamera &c, const _OWLCameraData &d)
{
  c.id = d.id;
  copy_v<7>(d.pose, c.pose);
  c.cond = d.cond;
  c.flag = d.flag;
}

void copy_images(OWLImage &img, const _OWLImageData &d, int frame)
{
  img.id = d.id;
  img.frame = frame;
  img.camera = d.camera;
  img.detector = d.detector;
  img.width = d.width;
  img.flag = d.flag;

  img.pos = d.pos;
  img.amp = d.amp;
}

void copy_detectors(OWLDetectors &det, const _OWLDetectorsData &d, int frame)
{
  assert(sizeof(d.detectors) == sizeof(uint_t)*8);
  det.id = d.id;
  det.frame = frame;
  for(int i = 0; i < 8; i++)
    det.detectors[i] = d.detectors[i];
}

void copy_plane(OWLPlane &p, const _OWLPlaneData &d, int frame)
{
  p.id = d.id;
  p.camera = d.camera;
  p.frame = frame;
  p.plane[0] = d.plane[0];
  p.plane[1] = d.plane[1];
  p.plane[2] = d.plane[2];
  p.plane[3] = d.plane[3];
  p.cond = d.cond;
  p.flag = d.flag;
}

// 0: server transform
// 1: client transform
void set_transform(const float *p, int n)
{
  if(p && n > -1 && n < 2) copy_v<7>(p, _owl.p_stack[n]);

  // scale server transform
  float p0[7]; copy_v<7>(_owl.p_stack[0], p0);
  for(size_t i = 0; i < 3; i++) p0[i] *= _owl.scale;

  mult_pp(_owl.p_stack[1], p0, _owl.pose);

  convert_pm(_owl.pose, _owl.m);
}

int add_c3dmarkers()
{
  if(!_owl.recState.frames.size()) return -1;

  if(_owl.recState.index >= _owl.recState.frames.size()) return -1;

  vector<OWLMarker> markers(_owl.recState.frames[_owl.recState.index]);

  _owl.markers.push_back(markers);

  if(markers.size())  _owl.frameNumber = markers[0].frame;

  return markers.size();
}

int add_markers(_OWLHeader *p)
{
  int n = p->size/sizeof(_OWLMarkerData);

  assert(n < 1024);

  if(_owl.markers.size() > 1024*4)
    {
      //cerr << "error: owl_read: marker storage exceeded" << endl;
      return -1;
    }

  _owl.markers.push_back(vector<OWLMarker>());

  _OWLMarkerData *data = (_OWLMarkerData*)p->data;
  vector<OWLMarker> &markers = _owl.markers.back();

  markers.resize(n);
  for(int i = 0; i < n; i++)
    copy_marker(markers[i], data[i], p->frame);

  return n;
}

int add_rigids(_OWLHeader *p)
{
  int n = p->size/sizeof(_OWLRigidData);

  assert(n < 1024);

  if(_owl.rigids.size() > 1024)
    {
      //cerr << "error: owl_read: rigid storage exceeded" << endl;
      return -1;
    }

  _owl.rigids.push_back(vector<OWLRigid>());

  _OWLRigidData *data = (_OWLRigidData*)p->data;
  vector<OWLRigid> &rigids = _owl.rigids.back();

  rigids.resize(n);
  for(int i = 0; i < n; i++)
    copy_rigid(rigids[i], data[i], p->frame);

  return n;
}

int add_cameras(_OWLHeader *p)
{
  int n = p->size/sizeof(_OWLCameraData);

  assert(n < 256);

  _OWLCameraData *data = (_OWLCameraData*)p->data;

  _owl.cameras.resize(n);
  for(int i = 0; i < n; i++)
    copy_camera(_owl.cameras[i], data[i]);

  return 0;
}

int add_images(_OWLHeader *p)
{
  int n = p->size/sizeof(_OWLImageData);

  assert(n < 16384);

  if(_owl.images.size() > 1024)
    {
      //cerr << "error: owl_read: images storage exceeded" << endl;
      return -1;
    }

  _owl.images.push_back(vector<OWLImage>());

  _OWLImageData *data = (_OWLImageData*)p->data;
  vector<OWLImage> &images = _owl.images.back();

  images.resize(n);
  for(int i = 0; i < n; i++)
    copy_images(images[i], data[i], p->frame);

  return n;
}

int add_detectors(_OWLHeader *p)
{
  int n = p->size/sizeof(_OWLDetectorsData);

  assert(n < 1024);

  if(_owl.detectors.size() > 1024)
    {
      //cerr << "error: owl_read: detectors storage exceeded" << endl;
      return -1;
    }

  _owl.detectors.push_back(vector<OWLDetectors>());

  _OWLDetectorsData *data = (_OWLDetectorsData*)p->data;
  vector<OWLDetectors> &detectors = _owl.detectors.back();

  detectors.resize(n);
  for(int i = 0; i < n; i++)
    copy_detectors(detectors[i], data[i], p->frame);

  return n;
}

int add_planes(_OWLHeader *p)
{
  int n = p->size/sizeof(_OWLPlaneData);

  assert(n < 1024);

  if(_owl.planes.size() > 1024)
    {
      //cerr << "error: owl_read: plane storage exceeded" << endl;
      return -1;
    }

  _owl.planes.push_back(vector<OWLPlane>());

  _OWLPlaneData *data = (_OWLPlaneData*)p->data;
  vector<OWLPlane> &planes = _owl.planes.back();

  planes.resize(n);
  for(int i = 0; i < n; i++)
    copy_plane(planes[i], data[i], p->frame);

  return n;
}

int add_config(_OWLHeader *p)
{
  int n = p->size/sizeof(_OWLConfigData);

  assert(n < 256);

#if 0
  _OWLConfigData *data = (_OWLConfigData*)p->data;

  cout << "Config: trackers=" << n << endl;
  for(int i = 0; i < n; i++)
    {
      cout << " id=" << data[i].id
	   << " type=0x" << hex << data[i].type << dec
	   << " count=" << data[i].marker_count
	   << endl;
    }
#endif

  return 0;
}

int add_floats(_OWLHeader *p)
{
  int n = p->size;

  assert(n < 1024);

  if(_owl.floats.size() > 1024)
    {
      //cerr << "error: owl_read: float storage exceeded" << endl;
      return -1;
    }

  OWLenum pname = *ENUM(p->data+0);
  int reuse = 0;

  if(pname == OWL_FREQUENCY) reuse = 1;
  if(pname == OWL_TRANSFORM)
    {
      set_transform(FLOAT(p->data+4), 0);
      reuse = 1;
    }
  DBG(cout << " add_float " << hex << pname << dec << " " << (p->size-4)/sizeof(float) << endl);

  add_float(pname, FLOAT(p->data+4), (p->size-4)/sizeof(float), reuse);

  return n;
}

int add_integers(_OWLHeader *p)
{
  int n = p->size;

  assert(n < 1024);

  if(_owl.integers.size() > 1024)
    {
      //cerr << "error: owl_read: integer storage exceeded" << endl;
      return -1;
    }

  OWLenum pname = *ENUM(p->data+0);
  int reuse = 0;

  if(pname == OWL_STREAMING) reuse = 1;
  if(pname == OWL_BROADCAST) reuse = 1;
  if(pname == OWL_INTERPOLATION) reuse = 1;
  if(pname == OWL_MARKERS) reuse = 1;
  if(pname == OWL_RIGIDS) reuse = 1;
  if(pname == OWL_IMAGES) reuse = 1;
  if(pname == OWL_DETECTORS) reuse = 1;
  if(pname == OWL_PLANES) reuse = 1;

  DBG(cout << " add_integer " << hex << pname << dec << " " << (p->size-4)/sizeof(int) << endl);

  add_integer(pname, INT(p->data+4), (p->size-4)/sizeof(int), reuse);

  return n;
}

int add_string(_OWLHeader *p)
{
  int n = p->size;

  assert(n < 1024+4);

  if(_owl.strings.size() > 1024+4)
    {
      //cerr << "error: owl_read: string storage exceeded" << endl;
      return -1;
    }

  OWLenum pname = *ENUM(p->data+0);
  int reuse = 0;

  add_string(pname, (char*)p->data+4, p->size-4, reuse);

  return n;
}

/// sets

template <class T>
void add_param(list<T> &l, OWLenum pname, const void *param, size_t count, int reuse)
{
  if(reuse)
    {
      typename list<T>::iterator i = find(l.begin(), l.end(), T(pname, 0));
      if(i != l.end())
	{
	  i->data.resize(count);
	  memcpy(&*i->data.begin(), param, count * sizeof(typename T::type));
	  return;
	}
    }

  l.push_back(T(pname, _owl.frameNumber));

  T &n = l.back();
  n.data.resize(count);
  memcpy(&*n.data.begin(), param, count * sizeof(typename T::type));
  /// !!! use insert
}

void add_float(OWLenum pname, const float *param, size_t count, int reuse)
{
  add_param(_owl.floats, pname, param, count, reuse);
}

void add_integer(OWLenum pname, const int *param, size_t count, int reuse)
{
  add_param(_owl.integers, pname, param, count, reuse);
}

void add_string(OWLenum pname, const char *param, size_t count, int reuse)
{
  if(param && count == 0) count = strlen(param);
  add_param(_owl.strings, pname, param, count, reuse);
}

void send_init(int flags)
{
  _OWLHeader *p = _client.Out(pInit, 8);

  _owl.flags = flags;
  *INT(p->data+0) = flags;
  *ENUM(p->data+4) = OWL_PROTOCOL_VERSION;

  _client.Send();
}

void send_request(size_t type)
{
  _OWLHeader *p = _client.Out(pRequest, 4);

  *ENUM(p->data+0) = type;

  _client.Send();
}


void owl_send_data(packet_t type, OWLenum pname, const void *data, size_t bytes)
{
  _OWLHeader *p = _client.Out(type, 4+bytes);

  *ENUM(p->data+0) = pname;
  if(bytes && data) memcpy(p->data+4, data, bytes);

  _client.Send();
}

void owl_send_data(packet_t type, int number, OWLenum pname, const void *data, size_t bytes)
{
  _OWLHeader *p = _client.Out(type, 8+bytes);

  *INT(p->data+0) = number;
  *ENUM(p->data+4) = pname;
  if(bytes && data) memcpy(p->data+8, data, bytes);

  _client.Send();
}


// return value:
//  success: flags (or flags from the server if slave)
//  error:   -1
int owlInit(const char *server, int flags)
{
  if(_owl)
    {
      // already in use
      _owl.errors.push_back(OWL_INVALID_OPERATION);
      return -1;
    }

  _owl.InitClientState();

  float f = 0;
  add_float(OWL_FREQUENCY, &f, 1, 1);

  int n = OWL_DISABLE;
  add_integer(OWL_STREAMING, &n, 1, 1);
  add_integer(OWL_BROADCAST, &n, 1, 1);

  n = 0;
  add_integer(OWL_INTERPOLATION, &n, 1, 1);

  n = OWL_ENABLE;
  add_integer(OWL_MARKERS, &n, 1, 1);
  add_integer(OWL_RIGIDS, &n, 1, 1);
  add_integer(OWL_IMAGES, &n, 1, 1);
  add_integer(OWL_DETECTORS, &n, 1, 1);
  add_integer(OWL_PLANES, &n, 1, 1);

  // owl file reading mode
  if(flags & OWL_FILE)
    {
      int ret = _owl.recState.Init(server);
      if(ret > 0)
        {
          _owl.init = 1;
          _owl.flags = flags;

          return _owl.flags;
        }
      else
        return ret;
    }

  // separate server name into name:port
  char name[1024] = "localhost";
  int port = 0;

  if(server)
    {
      char *ptr = (char*)strchr(server, ':');
      int size = ptr ? ptr - server : strlen(server);

      if(size) memcpy(name, server, size), name[size] = 0;
      if(ptr) sscanf(ptr, ":%d", &port);
    }

  _client.sock = connect_tcp(name, 8000+port);

  if(_client.sock == -1)
    {
      // can't connect to server
#ifndef WIN32
      _owl.errors.push_back(-errno);
#else
      _owl.errors.push_back(-WSAGetLastError());
#endif
      cerr << "error: connect_tcp" << endl;
      return -1;
    }

  DBG(cout << "owl connect: " << _client.sock << endl);

  nonblock(_client.sock);

  send_init(flags);
  cerr << "owl init : send_init" << endl;

  int ret = 0;
  int c = 1;
  while(_owl.init == 0)
    {
      ret = owl_read();
      cerr << "owl init : owl_read = " << ret << endl;
      if(ret < 0) {
        cerr << "error: owl_read" << endl;
          return -1;
      }

      if(_owl.errors.size() > 0) return -1;

      owl_delay(1);

      if(c++ > 30000) {
        cerr << "too much retries..." << endl;
        return -2;
      }
    }


  // broadcast listener socket
  {
    _broadcast.sock = listen_udp(8500+port);
    if(_broadcast.sock < 0)
      {
        cerr << "error: setup_listening(SOCK_DGRAM): " << strerror(errno) << endl;
      }
    else
      {
        nonblock(_broadcast.sock);
        DBG(cout << "broadcast listener: " << _broadcast.sock << endl);
      }
  }
  cout << "owlInit done" << endl;

  return _owl.flags;
}

void owlDone(void)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  // slave does not send out pDone packet
  if((_owl.flags & OWL_SLAVE) == 0)
    {
      //cout << "done" << endl;
      _OWLHeader *p = _client.Out(pDone, 4);
      *INT(p->data+0) = 0;

      _client.Send();

      // wait for server to get the command
      owl_delay(10);
    }

  owl_done();

  //cout << "owlDone" << endl;
}

/* Sets */

void owlSetFloat(OWLenum pname, float param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pSetFloatv, pname, &param, 4);
}

void owlSetInteger(OWLenum pname, int param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pSetIntegerv, pname, &param, 4);

  if(pname == OWL_BROADCAST)
    {
      _broadcast.broadcast = (param == OWL_ENABLE);
    }
}

void owlSetFloatv(OWLenum pname, const float *param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  if(pname == OWL_TRANSFORM)
    {
      owl_send_data(pSetFloatv, pname, param, 7*4);
      send_request(OWL_REQUEST_CAMERAS);
    }
  else
    owl_send_data(pSetFloatv, pname, param, 4);
}

void owlSetIntegerv(OWLenum pname, const int *param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pSetIntegerv, pname, param, 4);
}

void owlSetString(OWLenum pname, const char *str)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pSetString, pname, str, strlen(str));
}

/* Tracker */

void owlTracker(int tracker, OWLenum pname)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pTracker, tracker, pname, 0, 0);
}

void owlTrackerf(int tracker, OWLenum pname, float param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pTrackerfv, tracker, pname, &param, 4);
}

void owlTrackeri(int tracker, OWLenum pname, int param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pTrackeriv, tracker, pname, &param, 4);
}

void owlTrackerfv(int tracker, OWLenum pname, const float *param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  if(pname == OWL_SET_FILTER)
    owl_send_data(pTrackerfv, tracker, pname, param, 4*4);
  else
    owl_send_data(pTrackerfv, tracker, pname, param, 4);
}

void owlTrackeriv(int tracker, OWLenum pname, const int *param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pTrackeriv, tracker, pname, param, 4);
}


/* Marker */

void owlMarker(int marker, OWLenum pname)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pMarker, marker, pname, 0, 0);
}

void owlMarkerf(int marker, OWLenum pname, float param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pMarkerfv, marker, pname, &param, 4);
}

void owlMarkeri(int marker, OWLenum pname, int param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pMarkeriv, marker, pname, &param, 4);
}

void owlMarkerfv(int marker, OWLenum pname, const float *param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  if(pname == OWL_SET_POSITION)
    {
      float pos[3];
      for(int i = 0; i < 3; i++) pos[i] = param[i] * _owl.scale;

      owl_send_data(pMarkerfv, marker, pname, pos, 12);
    }
  else
    owl_send_data(pMarkerfv, marker, pname, param, 4);
}

void owlMarkeriv(int marker, OWLenum pname, const int *param)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  owl_send_data(pMarkeriv, marker, pname, param, 4);
}


/// client ///
void owlScale(float scale)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }
  _owl.scale = scale;
  set_transform(0, -1);
}

// pose: pos, rot -- [x y z], [s x y z]
void owlLoadPose(const float *pose)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return; }

  set_transform(pose, 1);
}


/// server -> client ///

int owlGetStatus(void)
{
  if(!_owl) return 0;

  if(_owl.flags & OWL_FILE) return 1;

  _owl.lastAck = 0;
  send_request(OWL_REQUEST_ACK);

  int ret = 0;
  while(_owl.lastAck != pRequest)
    {
      ret = owl_read();

      if(ret < 0) return 0;

      if(_owl.errors.size() > 0) return 0;

      owl_delay(1);
    }

  return _owl.errors.size() == 0;
}


int owlGetError(void)
{
  if(_owl)
    {
      owl_read();
    }

  if(_owl.errors.size() > 0)
    {
      int e = _owl.errors.front();
      _owl.errors.pop_front();
      return e;
    }

  return OWL_NO_ERROR;
}


// return values for all owlGet functions:
//   success:  number of elements (zero indicates absence of data)
//   error:   <0 (-errno)
int owlGetMarkers(OWLMarker *markers, uint_t count)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return -1; }

  owl_read();

  size_t n = 0;

  if(_owl.markers.size() > 0)
    {
      vector<OWLMarker> &m = _owl.markers.front();

      n = m.size();

      if(n > count) n = count;

      for(size_t i = 0; i < n; i++)
	{
	  markers[i] = m[i];

	  if(markers[i].cond > 0)
	    {
	      // scale
	      m[i].x *= _owl.scale;
	      m[i].y *= _owl.scale;
	      m[i].z *= _owl.scale;

	      // transform
	      mult_mv3_v3(_owl.m, &m[i].x, &markers[i].x);
	    }
	}

      _owl.markers.pop_front();
    }

  return n;
}

int owlGetRigids(OWLRigid *rigid, uint_t count)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return -1; }

  owl_read();

  size_t n = 0;

  if(_owl.rigids.size() > 0)
    {
      vector<OWLRigid> &r = _owl.rigids.front();

      n = r.size();

      if(n > count) n = count;

      for(size_t i = 0; i < n; i++)
	{
	  rigid[i] = r[i];

	  if(rigid[i].cond > 0)
	    {
	      // scale and apply pose
	      rigid[i].pose[0] *= _owl.scale;
	      rigid[i].pose[1] *= _owl.scale;
	      rigid[i].pose[2] *= _owl.scale;

	      float p[7];  copy_v<7>(rigid[i].pose, p);

	      mult_pp(_owl.pose, p, rigid[i].pose);
	    }
	}

      _owl.rigids.pop_front();
    }

  return n;
}

// this is the old form of get rigids, for backwards compatibility
#undef owlGetRigid
extern "C"
int owlGetRigid(OWLRigid *rigid, uint_t count)
{
  return owlGetRigids(rigid, count);
}

int owlGetCameras(OWLCamera *cameras, uint_t count)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return -1; }

  owl_read();

  size_t n = 0;

  for(n = 0; n < _owl.cameras.size() && n < count; n++)
    {
      cameras[n].id = _owl.cameras[n].id;

      copy_v<7>(_owl.cameras[n].pose, cameras[n].pose);

      cameras[n].cond = _owl.cameras[n].cond;

      if(cameras[n].cond > 0)
	{
	  // scale and apply pose
	  cameras[n].pose[0] *= _owl.scale;
	  cameras[n].pose[1] *= _owl.scale;
	  cameras[n].pose[2] *= _owl.scale;

	  float p[7];  copy_v<7>(cameras[n].pose, p);

	  mult_pp(_owl.pose, p, cameras[n].pose);
	}
    }

  return n;
}


int owlGetImages(OWLImage *images, uint_t count)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return -1; }

  owl_read();

  size_t n = 0;

  if(_owl.images.size() > 0)
    {
      vector<OWLImage> &p = _owl.images.front();

      n = p.size();

      if(n > count) n = count;

      for(size_t i = 0; i < n; i++) images[i] = p[i];

      _owl.images.pop_front();
    }

  return n;
}

int owlGetDetectors(OWLDetectors *detectors, uint_t count)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return -1; }

  owl_read();

  size_t n = 0;

  if(_owl.detectors.size() > 0)
    {
      vector<OWLDetectors> &p = _owl.detectors.front();

      n = p.size();

      if(n > count) n = count;

      for(size_t i = 0; i < n; i++) detectors[i] = p[i];

      _owl.detectors.pop_front();
    }

  return n;
}

int owlGetPlanes(OWLPlane *planes, uint_t count)
{
  if(!_owl) { _owl.errors.push_back(OWL_INVALID_OPERATION); return -1; }

  owl_read();

  size_t n = 0;

  if(_owl.planes.size() > 0)
    {
      vector<OWLPlane> &p = _owl.planes.front();

      n = p.size();

      if(n > count) n = count;

      for(size_t i = 0; i < n; i++)
	{
	  planes[i] = p[i];

	  if(planes[i].cond > 0)
	    {
	      extern void mult_mitv_v(const float *a, const float *b, float *ab);

	      // scale
	      p[i].plane[3] *= _owl.scale;

	      /// transform
	      mult_mitv_v(_owl.m, p[i].plane, planes[i].plane);
	    }
	}

      _owl.planes.pop_front();
    }

  return n;
}

int owlGetFloatv(OWLenum pname, float *param)
{
  if(_owl) owl_read();

  Floats::iterator i = find(_owl.floats.begin(), _owl.floats.end(), OWLFloats(pname, 0));
  size_t n = 0;

  if(i != _owl.floats.end())
    {
      n = i->data.size();
      memcpy(param, &*i->data.begin(), n*sizeof(float));
    }

  if(pname == OWL_FREQUENCY);
  else if(pname == OWL_TRANSFORM);
  else if(pname == OWL_CALIB_ERROR)
    {
      if(i != _owl.floats.end()) _owl.floats.erase(i--);
    }
  else
    {
      assert(i == _owl.floats.end());
      _owl.errors.push_back(OWL_INVALID_ENUM);
      return -1;
    }

  return n;
}

int owlGetIntegerv(OWLenum pname, int *param)
{
  if(_owl) owl_read();

  Integers::iterator i = find(_owl.integers.begin(), _owl.integers.end(), OWLIntegers(pname, 0));
  size_t n = 0;

  if(i != _owl.integers.end())
    {
      n = i->data.size();
      memcpy(param, &*i->data.begin(), n*sizeof(int));
    }

  if(pname == OWL_STREAMING);
  else if(pname == OWL_BROADCAST);
  else if(pname == OWL_INTERPOLATION);
  else if(pname == OWL_MARKERS);
  else if(pname == OWL_RIGIDS);
  else if(pname == OWL_IMAGES);
  else if(pname == OWL_DETECTORS);
  else if(pname == OWL_PLANES);
  else if(pname == OWL_BUTTONS)
    {
      if(i != _owl.integers.end()) _owl.integers.erase(i--);
    }
  else if(pname == OWL_TIMESTAMP)
    {
      if(i != _owl.integers.end()) _owl.integers.erase(i--);
    }
  else if(pname == OWL_FRAME_NUMBER)
    {
      *param = _owl.frameNumber;
      n = 1;
    }
  else
    {
      assert(i == _owl.integers.end());
      _owl.errors.push_back(OWL_INVALID_ENUM);
      return -1;
    }

  return n;
}

int owlGetString(OWLenum pname, char *str)
{
  if(_owl) owl_read();

  Strings::iterator i = find(_owl.strings.begin(), _owl.strings.end(), OWLString(pname, 0));
  size_t n = 0;

  if(i != _owl.strings.end())
    {
      n = i->data.size();
      if(n > 1023) n = 1023;
      memcpy(str, &*i->data.begin(), n);
      str[n] = 0;
    }

  if(pname == OWL_NO_ERROR)
    n = sprintf(str, "No Error");
  else if(pname == OWL_INVALID_VALUE)
    n = sprintf(str, "Invalid Value");
  else if(pname == OWL_INVALID_ENUM)
    n = sprintf(str, "Invalid Enum");
  else if(pname == OWL_INVALID_OPERATION)
    n = sprintf(str, "Invalid Operation");
  else if(pname == OWL_VERSION)
    {
      int v = OWL_PROTOCOL_VERSION;
      n = sprintf(str, "%d.%d.%d SOCKET", (v&0xFF0000)>>16, (v&0x00FF00)>>8, (v&0x0000FF)>>0);
    }
  else if(pname == OWL_COMMDATA)
    {
      if(i != _owl.strings.end()) _owl.strings.erase(i--);
    }
  else if(pname == OWL_STATUS_STRING)
    {
      if(i != _owl.strings.end()) _owl.strings.erase(i--);
    }
  else if(pname == OWL_CUSTOM_STRING)
    {
      if(i != _owl.strings.end()) _owl.strings.erase(i--);
    }
  else if(pname == OWL_CALIB_STATUS)
    {
      if(i != _owl.strings.end()) _owl.strings.erase(i--);
    }
  else
    {
      assert(i == _owl.strings.end());
      _owl.errors.push_back(OWL_INVALID_ENUM);
      return -1;
    }

  return n;
}

//// RPD /////

_RPDClient _rpd_client(0x1000000, 0x100000);

int owlRPDOpen(const char *servername, const char *filename, int mode)
{
  if(!servername) servername = "localhost";
  int ret = _rpd_client.Open(servername, filename, mode);
  if(ret < 0) _rpd_client.Close();
  return ret;
}

void owlRPDClose()
{
  _rpd_client.Disconnect();

  // flush
  while(_rpd_client._mode == OWL_RPD_SAVE &&
	_rpd_client.sock > -1 &&
	_rpd_client.Recv() >= 0);

  if(_rpd_client.sock > -1) _rpd_client.Close();
}

int owlRPDSend()
{
  if(_rpd_client.sock < 0) return 0;
  int ret = _rpd_client.Send();//0, 0x100000);
  if(ret < 0) _rpd_client.Close();
  return ret;
}

int owlRPDRecv()
{
  if(_rpd_client.sock < 0) return 0;
  int ret = _rpd_client.Recv();
  if(ret < 0) _rpd_client.Close();
  return ret;
}

//// RPDClient ////



int _RPDClient::Open(const char *servername, const char *filename, int mode)
{
  Close();

  if(mode == OWL_RPD_SAVE)
    {
#ifdef WIN32
      fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
#else
      fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_LARGEFILE, 0644);
#endif
    }
  else if(mode == OWL_RPD_LOAD)
    {
#ifdef WIN32
      // don't forget to open file in binary mode %#@^$*(#%$#$!!!
      fd = open(filename, O_RDONLY|O_BINARY);
#else
      fd = open(filename, O_RDONLY|O_LARGEFILE);
#endif
    }
  else
    {
      cerr << "error: invalid RPD mode " << mode << endl;
      return -1;
    }

  if(fd < 0)
    {
      cerr << "error: could not open file " << filename << ": " << strerror(errno) << endl;
      return -2;
    }

  _mode = mode;

  clear();

#if 0
  // load file
  if(mode == OWL_RPD_LOAD)
    {
      // seek file
      int fsize = lseek(fd, 0, SEEK_END);
      if(fsize < 0 || lseek(fd, 0, SEEK_SET) < 0)
	{
	  cerr << "lseek failed: " << strerror(errno) << endl;
	  return -5;
	}

      // allocate space
      reserve(fsize);
      if(available() < (size_t)fsize)
	{
	  cerr << "failed to allocate " << fsize << " bytes: " << available() << endl;
	  return -6;
	}

      // read file
      cout << "reading file: " << filename << endl;
      while(1)
	{
	  int ret = read(fd, end(), available());
	  if(ret == 0) break;
	  if(ret < 0) return ret;
	  (*this) += ret, _read += ret;
	}
      cout << " read " << _read << " bytes" << endl;
    }
#endif

  nonexec(fd);

  // separate server name into name:port
  char name[1024] = "localhost";
  int port = 0;

  if(servername)
    {
      char *ptr = (char*)strchr(servername, ':');
      int size = ptr ? ptr - servername : strlen(servername);

      if(size) memcpy(name, servername, size), name[size] = 0;
      if(ptr) sscanf(ptr, ":%d", &port);
    }

  sock = connect_tcp(name, 9000+port);

  if(sock < 0)
    {
      cerr << "error: could not connect to " << servername << endl;
      return -3;
    }
  nonblock(sock);

  cout << "RPD: connect: " << sock << endl;

  int ret = owl_send(sock, (char*)&mode, sizeof(mode));
  if(ret != sizeof(mode))
    {
      cerr << "error: failed to send mode" << endl;
      return -4;
    }

  if(mode == OWL_RPD_LOAD)
    {
      cout << "sending RPD header" << endl;
      ret = Send(10000, 0x100000);
      if(ret > 0) cout << " sent=" << ret << endl;
      else return ret;
    }

  cout << "RPD: opened with " << mode << endl;

  return 1;
}

void _RPDClient::Disconnect()
{
  if(sock > -1 && _mode == OWL_RPD_SAVE)
    {
      //cout << "rpd::disconnect" << endl;
      // set mode=0, works only in SAVE mode
      int mode = 0;
      int ret = owl_send(sock, (char*)&mode, sizeof(mode));
      if(ret != sizeof(mode))
	{
	  cerr << "error: failed to send mode" << endl;
	  return;
	}
    }
}

void _RPDClient::Close()
{
  if(sock < 0) return;
  cout << "rpd close: " << sock << endl;
  if(sock > -1) closesocket(sock);
  if(fd > -1) close(fd);
  sock = -1;
  fd = -1;
  _mode = 0;
  clear();

  if(_write || _read || _send || _recv)
    cout << "rpd: write=" << _write << " read=" << _read << " send=" << _send << " recv=" << _recv
         << " max_size=" << _max_size << endl;
  cout << "RPD: closed" << endl;

  _write = _read = _send = _recv = _max_size = 0;
}

int _RPDClient::Recv()
{
  if(_mode != OWL_RPD_SAVE) return -1;

  int ret = owl_recv(sock, end(), available());

  if(ret < 0 && size() == 0) return ret;

  if(ret > 0) (*this) += ret, _recv += ret;

  if((int)size() > _max_size) _max_size = size();

  // write exactly one 'chunk' at a time
  if(ret < 0 || _rpd_chunk_size == 0)
    ret = write(fd, begin(), size());
  else if(size() > _rpd_chunk_size)
    ret = write(fd, begin(), _rpd_chunk_size);
  else
    ret = 0;

  if(ret < 0) return ret;

  if(ret > 0) (*this) -= ret, _write += ret;

  return ret;
}

int _RPDClient::Send(size_t retry, size_t txsize)
{
  if(_mode != OWL_RPD_LOAD) return -1;

  int ret = read(fd, end(), available());

  if(ret <= 0 && size() == 0) return ret ? ret : -1;

  if(ret > 0) (*this) += ret, _read += ret;

  if(txsize == 0 || txsize > size()) txsize = size();

  ret = owl_select(0, sock, 0);

  if(ret <= 0) return ret;

  ret = owl_send(sock, begin(), txsize, retry);

  if(ret < 0) return ret;

  if(ret > 0) (*this) -= ret, _send += ret;

  return ret;
}
