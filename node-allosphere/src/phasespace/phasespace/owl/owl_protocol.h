// owl_protocol.h
// OWL v1.3

#ifndef OWL_PROTOCOL_H
#define OWL_PROTOCOL_H

#define OWL_PROTOCOL_VERSION 0x010300

// pRequest parameters
#define OWL_REQUEST_CAMERAS 0x0010
#define OWL_REQUEST_CONFIG  0x0011
#define OWL_REQUEST_ACK     0x0012

enum packet_t {
  pNone = 0,
  pInit,
  pDone,
  
  // client
  pSetFloatv = 0x10,
  pSetIntegerv,
  pSetString,

  pTracker,
  pTrackerfv,
  pTrackeriv,

  pMarker,
  pMarkerfv,
  pMarkeriv,
  
  pRequest,

  // server
  pError = 0x20,
  pMarkers,
  pRigids,
  pCameras,
  pConfig,
  pFloatv,
  pIntegerv,
  pString,
  pAck,

  pPlanes = 0x30,
  pDetectors = 0x31,
  pImages = 0x32,

  // last
  pLast
};

struct _OWLHeader {
  packet_t type;
  int size;
  int frame;
  char data[0];
};

struct _OWLMarkerData {
  int id;
  float pos[3];
  float cond;
  uint_t flag;
};

struct _OWLRigidData {
  int id;
  float pose[7];
  float cond;
  uint_t flag;
};

struct _OWLCameraData {
  int id;
  float pose[7];
  float cond;
  uint_t flag;
};

struct _OWLConfigData {
  int id;
  uint_t type;
  int marker_count;
};

struct _OWLImageData {
  int id;
  unsigned char camera;
  unsigned char detector;
  unsigned short width;
  uint_t flag;
  
  float pos;
  float amp;
};

struct _OWLDetectorsData {
  int id;
  uint_t detectors[8];
};

struct _OWLPlaneData {
  int id;
  int camera;
  float plane[4];
  float cond;
  uint_t flag;
};

#endif // OWL_PROTOCOL_H
