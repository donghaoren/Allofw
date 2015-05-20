// owl.h
// OWL v1.3

#ifndef OWL_H
#define OWL_H

#define OWL_API_VERSION 0x010300

#ifdef WIN32
#ifdef __DLL
#define OWLAPI __declspec(dllexport)
#else // !__DLL
#define OWLAPI __declspec(dllimport)
#endif // __DLL
#else // ! WIN32
#define OWLAPI
#endif // WIN32

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int uint_t;

struct OWLMarker {
  int id;
  int frame;
  float x, y, z;
  float cond;
  uint_t flag;
};

struct OWLRigid {
  int id;
  int frame;
  float pose[7];
  float cond;
  uint_t flag;
};

struct OWLCamera {
  int id;
  float pose[7];
  float cond;
  uint_t flag;
};


typedef unsigned int OWLenum;

/*
 * Constants
 */

#define OWL_MAX_FREQUENCY       480.0

/* Errors */
#define OWL_NO_ERROR            0x0
#define OWL_INVALID_VALUE       0x0020
#define OWL_INVALID_ENUM        0x0021
#define OWL_INVALID_OPERATION   0x0022

/* Common flags */
#define OWL_CREATE              0x0100
#define OWL_DESTROY             0x0101
#define OWL_ENABLE              0x0102
#define OWL_DISABLE             0x0103

/* Init flags */
#define OWL_SLAVE               0x0001  // socket only
#define OWL_FILE                0x0002  // socket only
#define OWL_POSTPROCESS         0x0010
#define OWL_MODE1               0x0100
#define OWL_MODE2               0x0200
#define OWL_MODE3               0x0300
#define OWL_MODE4               0x0400
#define OWL_LASER               0x0A00
#define OWL_CALIB               0x0C00
#define OWL_CALIBPLANAR         0x0F00

/* Sets */
#define OWL_FREQUENCY           0x0200
#define OWL_STREAMING           0x0201  // socket only
#define OWL_INTERPOLATION       0x0202
#define OWL_BROADCAST           0x0203  // socket only
#define OWL_BUTTONS             0x0210
#define OWL_MARKERS             0x0211
#define OWL_RIGIDS              0x0212
#define OWL_COMMDATA            0x0220
#define OWL_TIMESTAMP           0x0221
#define OWL_PLANES              0x02A0
#define OWL_DETECTORS           0x02A1
#define OWL_IMAGES              0x02A2

#define OWL_TRANSFORM           0xC200  // camera transformation

/* Trackers */
#define OWL_POINT_TRACKER       0x0300
#define OWL_RIGID_TRACKER       0x0301

// planar tracker (may be temporary)
#define OWL_PLANAR_TRACKER      0x030A

#define OWL_SET_FILTER          0x0310

// undocumented freatures
// use at your own risk
#define OWL_FEATURE0            0x03F0 // optical
#define OWL_FEATURE1            0x03F1 // offsets
#define OWL_FEATURE2            0x03F2 // projection
#define OWL_FEATURE3            0x03F3 // predicted
#define OWL_FEATURE4            0x03F4 // valid min
#define OWL_FEATURE5            0x03F5 // query min
#define OWL_FEATURE6            0x03F6 // storedepth
#define OWL_FEATURE7            0x03F7 // 
#define OWL_FEATURE8            0x03F8 // rejection
#define OWL_FEATURE9            0x03F9 // filtering
#define OWL_FEATURE10           0x03FA // window size
#define OWL_FEATURE11           0x03FB // LS cutoff
#define OWL_FEATURE12           0x03FC // off-fill
#define OWL_FEATURE_LAST        0x03FD // last feature

// calibration only
#define OWL_CALIB_TRACKER       0x0C01
#define OWL_CALIB_RESET         0x0C10
#define OWL_CALIB_LOAD          0x0C11
#define OWL_CALIB_SAVE          0x0C12
#define OWL_CALIBRATE           0x0C13
#define OWL_CAPTURE_RESET       0x0C20
#define OWL_CAPTURE_START       0x0C21
#define OWL_CAPTURE_STOP        0x0C22
#define OWL_CALIB_ACTIVE        0x0C30

// planar calib tracker (may be temporary)
#define OWL_CALIBPL_TRACKER     0x0CA1

/* Markers */
#define OWL_SET_LED             0x0400
#define OWL_SET_POSITION        0x0401
#define OWL_CLEAR_MARKER        0x0402

/* Gets */
#define OWL_VERSION             0x0500
#define OWL_FRAME_NUMBER        0x0510
#define OWL_STATUS_STRING       0x0520
#define OWL_CUSTOM_STRING       0x05F0

// calibration only
#define OWL_CALIB_STATUS        0x0C51
#define OWL_CALIB_ERROR         0x0C52
  
/* Macros */
#define MARKER(tracker, index)  (((tracker)<<12)|(index))

#define INDEX(id)   ((id)&0x0fff)
#define TRACKER(id) ((id)>>12)

// owlGetRigid is deprecated
#define owlGetRigid owlGetRigids

/* initialization */

OWLAPI int owlInit(const char *server, int flags);
OWLAPI void owlDone(void);

/* client -> server */

OWLAPI void owlSetFloat(OWLenum pname, float param);
OWLAPI void owlSetInteger(OWLenum pname, int param);
OWLAPI void owlSetFloatv(OWLenum pname, const float *param);
OWLAPI void owlSetIntegerv(OWLenum pname, const int *param);
OWLAPI void owlSetString(OWLenum pname, const char *str);

// 'tracker' is the tracker id
OWLAPI void owlTracker(int tracker, OWLenum pname);
OWLAPI void owlTrackerf(int tracker, OWLenum pname, float param);
OWLAPI void owlTrackeri(int tracker, OWLenum pname, int param);
OWLAPI void owlTrackerfv(int tracker, OWLenum pname, const float *param);
OWLAPI void owlTrackeriv(int tracker, OWLenum pname, const int *param);

// 'marker' is MARKER(tracker, index)
OWLAPI void owlMarker(int marker, OWLenum pname);
OWLAPI void owlMarkerf(int marker, OWLenum pname, float param);
OWLAPI void owlMarkeri(int marker, OWLenum pname, int param);
OWLAPI void owlMarkerfv(int marker, OWLenum pname, const float *param);
OWLAPI void owlMarkeriv(int marker, OWLenum pname, const int *param);

/* client */

OWLAPI void owlScale(float scale);
// pose: pos, rot -- [x y z], [s x y z]
OWLAPI void owlLoadPose(const float *pose);

/* server -> client */

OWLAPI int owlGetStatus(void);
OWLAPI int owlGetError(void);


OWLAPI int owlGetMarkers(OWLMarker *markers, uint_t count);
OWLAPI int owlGetRigids(OWLRigid *rigid, uint_t count);
OWLAPI int owlGetCameras(OWLCamera *cameras, uint_t count);

OWLAPI int owlGetFloatv(OWLenum pname, float *param);
OWLAPI int owlGetIntegerv(OWLenum pname, int *param);
OWLAPI int owlGetString(OWLenum pname, char *str);

#ifdef __cplusplus
}
#endif

#endif // OWL_H
