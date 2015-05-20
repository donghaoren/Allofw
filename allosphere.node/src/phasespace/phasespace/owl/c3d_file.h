// c3d_file.h
// c3d file utilities
#ifndef C3D_FILE_H
#define C3D_FILE_H
#include "owl.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include <fcntl.h>

#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

using namespace std;

// definition processor types
#define P_INTEL 84
#define P_DEC 85
#define P_MIPS 86

// number of parameter sections
#define NUMPARAM 9

// definition standard types
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

//
struct C3DHeader {
  byte	ppar;		// pointer to the first block of the parameter section
  byte	keyval0;	// key value=0x50
  word	np3d;		// number of 3D points
  word	nam;		// number of analog measurements per 3D frame
  word	nff;		// number of the first frame of 3D data (1 based)
  word	nlf;		// number of the last frame of 3D data
  word	mig;		// Maximum interpolation gap in 3D frames
  float	scfact;		// the 3d scale factor
  word	dstart;		// DATA_START - the number of the first block of the 3D and analog data section
  word	asf;		// The number of analog samples per 3D frame
  float	fps;		// The 3D frame rate in Hz (floating-point)
  word	res0[135];	// Reserved for future use
  word	keyval1;	// A key value (12345 decimal present if Label and Range data is present)
  word	lrsect;		// The first block of the label and Range section (if present)
  word	keyval2;	// A key value (12345 decimal present if Label and Range data is present)
  word	eventnum;	// Number of defined time events
  word	res1;		// reserved for future use (=0x0000)
  float	et[18];		// event times (floating points)
  byte	eflags[18];	// event flags 0x00 = ON, 0x01 = OFF
  word	res2;		// Reserved for future use
  dword	evl[18];	// Event labels. Each label is 4 characters long
  word	res3[22];	// reserved for future use
};

struct Param {
  byte bnum;
  byte key;
  byte bcount;
  byte proc;
};

struct Param_Data {
  byte proc;
  int aStartField;
  int aEndField;
};

inline float fromDEC(float f)
{
  char *a = (char*)&f;
  char b[4];
  b[0] = a[2];
  b[1] = a[3];
  b[2] = a[0];
  b[3] = a[1];
  if(b[0] || b[1] || b[2] || b[3]) b[3]--;
  return *(float*)b;
}

inline float fromDEC(int f)
{
  char *a = (char*)&f;
  char b[4];
  b[0] = a[2];
  b[1] = a[3];
  b[2] = a[0];
  b[3] = a[1];
  if(b[0] || b[1] || b[2] || b[3]) b[3]--;
  return *(int*)b;
}


// write c3d parameter

inline int transfer(void *dest, const void *src, size_t num, int *offset)
{
  memcpy(dest, src, num);
  (*offset) += num;
  return 1;
}

/* // this TRANSFER fcn not used
inline int transfer(int fildes, const void *buf, size_t nbyte, int *offset)
{
  write(fildes, buf, nbyte);
  (*offset) += nbyte;
  return 1;
} 
*/

inline int parameter_c3d(int fd, int markers, int frames, float fps) 
{
  if (fd <= 0) return 0;
  char buff[512*NUMPARAM];
  for (int i = 0; i < 512*NUMPARAM; i++) {
    buff[i] = 0;
  }

  int fileOffset = 0;  

  // PARAM Header 
  buff[fileOffset++] = 0x01; // the block number of the first block in the parameter section
  buff[fileOffset++] = 0x50; // key indentifier of 80 decimal
  buff[fileOffset++] = NUMPARAM; // number of parameter blocks
  buff[fileOffset++] = P_INTEL; // Intel Processor
  
  
  /*
  byte res = 0x01; // the block number of the first block in the parameter section
  transfer(buff, &res, sizeof(byte), &fileOffset);
  res = 0x50; // key indentifier of 80 decimal
  transfer(buff+fileOffset, &res, sizeof(byte), &fileOffset);
  res = NUMPARAM; // number of parameter blocks
  transfer(buff+fileOffset, &res, sizeof(byte), &fileOffset);
  res = P_INTEL; // Intel Processor
  transfer(buff+fileOffset, &res, sizeof(byte), &fileOffset);
  */

  // GROUP Header section
  word nxtPtr = 3;

  buff[fileOffset++] = 5; // 5 chars in 'point'
  buff[fileOffset++] = 0xfd; // designate ID = -3;
  transfer(buff+fileOffset, "POINT", 5*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor
  
  buff[fileOffset++] = 6; 
  buff[fileOffset++] = 0xfc; // designate ID = -4;
  transfer(buff+fileOffset, "ANALOG", 6*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor
  
  buff[fileOffset++] = 14; 
  buff[fileOffset++] = 0xfb; // designate ID = -5;
  transfer(buff+fileOffset, "FORCE_PLATFORM", 14*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor

  buff[fileOffset++] = 5; 
  buff[fileOffset++] = 0xfa; // designate ID = -6;
  transfer(buff+fileOffset, "TRIAL", 5*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor    

  // PARAMETER Point section
  nxtPtr = 7;
  buff[fileOffset++] = 4; // chars in "USED"
  buff[fileOffset++] = 3; // point group ID
  transfer(buff+fileOffset, "USED", 4*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 2; // integer type spec
  buff[fileOffset++] = 0; // dimensions
  word numMarkers = markers;
  transfer(buff+fileOffset, &numMarkers, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor

  buff[fileOffset++] = 6;
  buff[fileOffset++] = 3; // point group ID
  transfer(buff+fileOffset, "FRAMES", 6*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 2; // integer type spec
  buff[fileOffset++] = 0; // dimensions
  word numFrames = frames;
  transfer(buff+fileOffset, &numFrames, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor

  nxtPtr = 9;
  buff[fileOffset++] = 5;
  buff[fileOffset++] = 3; // point group ID
  transfer(buff+fileOffset, "SCALE", 5*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 4; // real type spec
  buff[fileOffset++] = 0; // dimensions
  float scale = -1.0;
  transfer(buff+fileOffset, &scale, sizeof(float), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor  

  buff[fileOffset++] = 4;
  buff[fileOffset++] = 3; // point group ID
  transfer(buff+fileOffset, "RATE", 4*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 4; // real type spec
  buff[fileOffset++] = 0; // dimensions
  float rate = fps;
  transfer(buff+fileOffset, &rate, sizeof(float), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor  

  nxtPtr = 8;
  buff[fileOffset++] = 5;
  buff[fileOffset++] = 3; // point group ID
  transfer(buff+fileOffset, "UNITS", 5*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = -1; // string type spec
  buff[fileOffset++] = 1; // dimensions
  buff[fileOffset++] = 2; // parameter dimensions
  transfer(buff+fileOffset, "mm", 2*sizeof(byte), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor

  nxtPtr = 7;
  buff[fileOffset++] = 10;
  buff[fileOffset++] = 3; // point group ID
  transfer(buff+fileOffset, "DATA_START", 10*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 2; // integer type spec
  buff[fileOffset++] = 0; // dimensions
  word dStart = NUMPARAM + 2;
  transfer(buff+fileOffset, &dStart, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor 
  
  nxtPtr = 4*markers + 7;
  buff[fileOffset++] = 6;
  buff[fileOffset++] = 3;
  transfer(buff+fileOffset, "LABELS", 6*sizeof(byte), &fileOffset);  
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset); 
  buff[fileOffset++] = -1; // string type spec
  buff[fileOffset++] = 2; // dimensions
  buff[fileOffset++] = 4; // 1st dim - string length
  buff[fileOffset++] = (byte) markers; //2nd dim - num labels ie markers
  char str[17];
  for (int i = 0; i < markers; i++) {
    sprintf(str, "M%03d", i);
    transfer(buff+fileOffset, str, 4*sizeof(byte), &fileOffset);  
  }
  buff[fileOffset++] = 0; // chars in descriptor 
  
  nxtPtr = 16*markers + 7;
  buff[fileOffset++] = 12;
  buff[fileOffset++] = 3;
  transfer(buff+fileOffset, "DESCRIPTIONS", 12*sizeof(byte), &fileOffset);  
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset); 
  buff[fileOffset++] = -1; // string type spec
  buff[fileOffset++] = 2; // dimensions
  buff[fileOffset++] = 16; // 1st dim - string length
  buff[fileOffset++] = (byte) markers; //2nd dim - num labels ie markers
  for (int i = 0; i < markers; i++) {
    sprintf(str, "Marker %03d%6c", i, ' ');
    transfer(buff+fileOffset, str, 16*sizeof(byte), &fileOffset);  
  }
  buff[fileOffset++] = 0; // chars in descriptor 
  

  // PARAMETER Analog section
  nxtPtr = 7;
  buff[fileOffset++] = 4;
  buff[fileOffset++] = 4; // analog group ID
  transfer(buff+fileOffset, "USED", 4*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 2; // integer type spec
  buff[fileOffset++] = 0; // dimensions
  numMarkers = 0;
  transfer(buff+fileOffset, &numMarkers, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor

  nxtPtr = 9;
  buff[fileOffset++] = 9;
  buff[fileOffset++] = 4; // analog group ID
  transfer(buff+fileOffset, "GEN_SCALE", 9*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 4; // real type spec
  buff[fileOffset++] = 0; // dimensions
  transfer(buff+fileOffset, &scale, sizeof(float), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor

  // PARAMETER Force_platform section
  nxtPtr = 7; //when trial section in use
  //nxtPtr = 0; // for when trial section not in use
  buff[fileOffset++] = 4;
  buff[fileOffset++] = 5; // force_platform group ID
  transfer(buff+fileOffset, "USED", 4*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 2; // integer type spec
  buff[fileOffset++] = 0; // dimensions
  numMarkers = 0;
  transfer(buff+fileOffset, &numMarkers, sizeof(word), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor
  
  // PARAMETER Trial section
#if 1 // use new 32bit start and end fields
  nxtPtr = 10;
  buff[fileOffset++] = 18;
  buff[fileOffset++] = 6; // force_platform group ID
  transfer(buff+fileOffset, "ACTUAL_START_FIELD", 18*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 2; // integer type spec
  buff[fileOffset++] = 1; // dimensions
  buff[fileOffset++] = 2; // parameter dimensions  
  int frameField = 1;
  transfer(buff+fileOffset, &frameField, sizeof(int), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor
    
  nxtPtr = 0;
  buff[fileOffset++] = 16;
  buff[fileOffset++] = 6; // force_platform group ID
  transfer(buff+fileOffset, "ACTUAL_END_FIELD", 16*sizeof(byte), &fileOffset);
  transfer(buff+fileOffset, &nxtPtr, sizeof(word), &fileOffset);
  buff[fileOffset++] = 2; // integer type spec
  buff[fileOffset++] = 1; // dimensions
  buff[fileOffset++] = 2; // parameter dimensions  
  frameField = frames;
  transfer(buff+fileOffset, &frameField, sizeof(int), &fileOffset);
  buff[fileOffset++] = 0; // chars in descriptor
#endif
  
  write(fd, buff, 512*NUMPARAM*sizeof(byte));
  return 1;
}

inline int grab(int fd, void *buf, size_t count, int *bytes_left) {
  int ret = read(fd, buf, count);
  (*bytes_left) -= count;
  return ret;
}

// retrieve ACTUAL_START_FIELD and ACTUAL_END_FIELD parameters if available
inline int frame_parameters_c3d(int fd, Param_Data *p, int *bytes_left)
{
  byte nameLen = 0;
  byte ID = 0;
  char paramName[129];
  word offset;
  
  int loop = 1;
  size_t check = 0;
  while (loop) {
    check = grab(fd, &nameLen, sizeof(byte), bytes_left);
    if (nameLen == 0) break;
    check += grab(fd, &ID, sizeof(byte), bytes_left);
    if (check < 2*sizeof(byte));
    check = grab(fd, paramName, nameLen*sizeof(byte), bytes_left);
    if (check < nameLen);
    check = grab(fd, &offset, sizeof(word), bytes_left);
    if (check < sizeof(word));
    if (offset <= 0) loop = 0;
    offset -= 2;
    paramName[nameLen] = '\0';
    //printf("hmm %s\n", paramName);
    if (strcmp(paramName, "ACTUAL_START_FIELD") == 0) {
      byte temp;
      for (int i = 0; i< 3; i++) {
        grab(fd, &temp, sizeof(byte), bytes_left);
      }
      int field = -1;
      grab(fd, &field, sizeof(int), bytes_left);
      p->aStartField = field;
      grab(fd, &temp, sizeof(byte), bytes_left);
    } else if (strcmp(paramName, "ACTUAL_END_FIELD") == 0) {
      byte temp;
      for (int i = 0; i< 3; i++) {
        grab(fd, &temp, sizeof(byte), bytes_left);
      }
      int field = -1;
      grab(fd, &field, sizeof(int), bytes_left);
      p->aEndField = field;
      grab(fd, &temp, sizeof(byte), bytes_left);
    } else {
      (*bytes_left) -= offset;
      if (lseek(fd, (off_t) offset, SEEK_CUR) < (off_t) 0);
    }           
  }
  //printf("gah %d %d\n", p->aStartField, p->aEndField);
  lseek(fd, (off_t) *bytes_left, SEEK_CUR);
  return 1;
}


// load c3d parameter
// bytes left stores a pointer to number of expected bytes left in parameter 
// that have yet to be parsed from the position of the current pointer within the c3d file 
inline int load_parameter_c3d(int fd, Param_Data *p, int *bytes_left)
{
  int ret = frame_parameters_c3d(fd, p, bytes_left);
  return ret;
}





// write c3d header
inline int header_c3d(int fd, int markers, int frames, float fps)
{
  if(fd <= 0) return 0;

  C3DHeader *h = new C3DHeader;

  // filling the C3D file header
  h->ppar = 0x02; //
  h->keyval0 = 0x50; // 1
  h->np3d = markers; // 2
  h->nam = 0; // 3
  h->nff = 1; // 4
  
  if (frames > 65535)
    h->nlf = 65535;
  else 
    h->nlf = frames; // 5
  
  h->mig = 10; // 6
  // h->scfact=(float)-0.01;    // the 3d scale factor
  h->scfact = -1.0; // KSA test changed to -1.0 does not seem to matter
  h->dstart = NUMPARAM + 2;//3; // DATA_START - the number of the first block of the 3D and analog data section
  h->asf = 0; // The number of analog samples per 3D frame
  h->fps = fps;    // The 3D frame rate in Hz (floating-point)
  for(size_t i = 0; i < 135; i++)
    {
      h->res0[i] = 0;    // Reserved for future use
    }
  h->keyval1 = 0x3039; // A key value (12345 decimal present if Label and Range data is present)
  h->lrsect = 0; // The first block of the label and Range section (if present)
  h->keyval2 = 0x3039; // A key value (12345 decimal present if Label and Range data is present)
  h->eventnum = 0x0000; // Number of defined time events
  h->res1 = 0x0000; // reserved for future use (=0x0000)
  for(size_t i = 0; i < 18; i++)
    {
      h->et[i] = 0.0; // event times (floating points)
      h->eflags[i] = 0x0001; // event flags 0x00 = ON, 0x01 = OFF
    }
  h->res2 = 0x0000; // Reserved for future use
  for(size_t i = 0; i < 18; i++)
    {
      h->evl[i] = 0x00000000; // Event labels. Each label is 4 characters long
    }
  for(size_t i = 0; i < 22; i++)
    {
      h->res3[i] = 0; // reserved for future use
    }

  // writting header to the file

  write(fd, h, sizeof(C3DHeader));

  // writting parameter section
#if 0 // old empty parameter
  byte npar = 0;
  byte proctype = P_INTEL;

  byte res;
  res = 0x01; // the block number of the first block in the parameter section
  write(fd, &res, sizeof(byte));
  res = 0x50; // key indentifier of 80 decimal
  write(fd, &res, sizeof(byte));
  res = npar; // number of parameter blocks
  write(fd, &res, sizeof(byte));
  res = proctype; // Intel Processor
  write(fd, &res, sizeof(byte));

  // preparing the parameters
  if(npar != 0)
    {
      // write the parameters
    }

  // fill the free file area
  res = 0x00;
  for(size_t i = 0; i < (512-4); i++)
    {
      write(fd, &res, sizeof(byte));
    }
#else
  parameter_c3d(fd, markers, frames, fps);
#endif

  delete h;
  return 1;
}

// write one frame
inline int write_c3d(int fd, const vector<OWLMarker> &markers, float scale = 1, int MOCAP = 0)
{
  if(fd <= 0) return 0;
  if(markers.size() == 0) return 0;

  // writing the C3D data to the file

  // marker xyz positions[1..num_markers][first_field..last_field]
  
  for(size_t marker = 0; marker < markers.size(); marker++)
    {
      const OWLMarker &m = markers[marker];
	  
      // Motion Builder Bug Compensation!!!
      float x = -m.x;
      float y = m.y;
      float z = m.z;

      if(scale != 0)
        {
          x /= scale;
          y /= scale;
          z /= scale;
        }

      // write X,Z,Y positions
      write(fd, &x, sizeof(float));
      write(fd, &z, sizeof(float));
      write(fd, &y, sizeof(float));
      
      // write residual value (condition number); write -1 for invalid marker
      float residual = (m.cond < 0) ? -1.0 : m.cond;
      if (MOCAP && residual == 0)
        residual = -1.0;
      write(fd, &residual, sizeof(float));
    }

  return 1;
}

inline int open_c3d(const char *filename)
{
  int fd;
#ifdef WIN32
  fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
#else
  fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
#endif

  if(fd < 0)
    {
      cerr << "open(" << filename << "): " << strerror(errno) << endl;
      return fd;
    }
  
  header_c3d(fd, 0, 0, 0);
  
  return fd;
}

inline int save_c3d(const char *filename, const vector< vector<OWLMarker> > &recorded, float fps)
{
  if(recorded.size() == 0) return 0;

  const vector<OWLMarker> &m0 = recorded[0];

  int fd = open_c3d(filename);

  if(fd <= 0) return 0;

  size_t count = 0;
  for(size_t i = 0; i < recorded.size(); i++, count++)
    {
      const vector<OWLMarker> &markers = recorded[i];
      
      if(!write_c3d(fd, markers)) break;      
    }

  // rewrite the header
  lseek(fd, 0, SEEK_SET);

  header_c3d(fd, m0.size(), recorded.size(), fps);

  close(fd);

  cout << "wrote to " << filename << endl;

  return count;
}

// [start, end)
inline int save_c3d(const char *filename, const vector< vector<OWLMarker> > &recorded, 
                    int start, int end, float fps, int MOCAP = 0)
{
  if(recorded.size() == 0) return 0;

  const vector<OWLMarker> &m0 = recorded[0];

  int fd = open_c3d(filename);

  if(fd <= 0) return 0;

  size_t count = 0;
  for(int i = start; i < end; i++, count++)
    {
      const vector<OWLMarker> &markers = recorded[i];
      
      if(!write_c3d(fd, markers, MOCAP)) break;      
    }

  // rewrite the header
  lseek(fd, 0, SEEK_SET);

  header_c3d(fd, m0.size(), end - start, fps);
  // header_c3d(fd, m0.size(), end - start+1, fps);

  close(fd);

  cout << "wrote to " << filename << endl;

  return count;
}

inline ostream &operator<<(ostream &out, const C3DHeader &h)
{
  out << hex 
      << " ppar=" << (int)h.ppar
      << " keyval0=" << (int)h.keyval0
      << dec
      << " np3d=" << (int)h.np3d
      << " nff=" << (int)h.nff
      << " nlf=" << (int)h.nlf
      << " mig=" << (int)h.mig
      << " scale=" << h.scfact
      << " dstart=" << (int)h.dstart
      << " asf=" << (int)h.asf
      << " fps=" << h.fps
      << dec;

  return out;
}

inline ostream &operator<<(ostream &out, const Param &p)
{
  out << hex
      << " bnum=" << (int)p.bnum
      << " key=" << (int)p.key
      << dec
      << " bcount=" << (int)p.bcount
      << " proc=" << (int)p.proc
      << dec;
  return out;
}

inline void convertFromDec(C3DHeader &h)
{
  h.scfact = fromDEC(h.scfact);
  h.fps = fromDEC(h.fps);
}

//get the frequency from a c3d file
inline int frequency_c3d(const char *filename, float *f)
{
   int fd; 
   unsigned char buf[512];
#ifdef WIN32
   fd = open(filename, O_RDONLY|O_BINARY, 0644);
#else
   fd = open(filename, O_RDONLY, 0644);
#endif
   if(fd < 0)
     {
       cerr << "open(" << filename << "): " << strerror(errno) << endl;
       return -1;
     }
   
   // read header
   C3DHeader h;
   int ret = read(fd, &h, sizeof(C3DHeader));
   if(ret < (int)sizeof(C3DHeader)) { close(fd); return -2; }
 
  // read 1st parameter section
   ret = read(fd, buf, 512);

   if(ret < 512) { close(fd); return -2; }

   Param param;
   memcpy(&param, buf, sizeof(Param));

   if(param.proc == P_INTEL);
   else if(param.proc == P_DEC) convertFromDec(h);
   else if(param.proc == P_MIPS)
     {
       cerr << "MIPS convertion not implemented!" << endl;
       return -2;
     }

  //printf("%f qwer\n", fromDEC(h.fps));

   *f = h.fps;


   return 1;
}

inline int load_c3d(const char *filename, vector< vector<OWLMarker> > &recorded)
{
  int fd;
  size_t count = 0;

  recorded.clear();

#ifdef WIN32
  fd = open(filename, O_RDONLY|O_BINARY, 0644);
#else
  fd = open(filename, O_RDONLY, 0644);
#endif
  
  if(fd < 0)
    {
      cerr << "open(" << filename << "): " << strerror(errno) << endl;
      return -1;
    }
  
  C3DHeader h;
  unsigned char buf[512];
  
  // read header

  int ret = read(fd, &h, sizeof(C3DHeader));

  if(ret < (int)sizeof(C3DHeader)) { close(fd); return -2; }

#if 0 // old parameter method vs new parameter method

  // read 1st parameter section
  ret = read(fd, buf, 512);

  if(ret < 512) { close(fd); return -2; }

  Param param;
  memcpy(&param, buf, sizeof(Param));

  if(param.proc == P_INTEL);
  else if(param.proc == P_DEC) convertFromDec(h);
  else if(param.proc == P_MIPS)
    {
      cerr << "MIPS convertion not implemented!" << endl;
      return -2;
    }
  
  //cout << param << endl;
  //cout << h << endl;

  // read last parameter section
  for(size_t i = 3; i < h.dstart; i++)
    {
      ret = read(fd, buf, 512);
      
      if(ret < 512) { close(fd); return -2; }
    }
  //printf("whereami %d\n", (int) lseek(fd, 0, SEEK_CUR));

  // check header
  size_t marker_count = h.np3d;

  if(marker_count > 512) { close(fd); return -3; }
  
  if(h.nff > h.nlf) { close(fd); return -4; }

  size_t frame_count = h.nlf - h.nff + 1; // original version did not have '+1'
  cout << "frames=" << frame_count << endl;

  recorded.resize(frame_count);

#else
  ret = read(fd, buf, sizeof(Param)); //read param section header
  if (ret < (int) sizeof(Param)) { close(fd); return -2; }
  Param param;
  memcpy(&param, buf, sizeof(Param));
  if(param.proc == P_INTEL);
  else if(param.proc == P_DEC) convertFromDec(h);
  else if(param.proc == P_MIPS)
    {
      cerr << "MIPS convertion not implemented!" << endl;
      return -2;
    }
  
  Param_Data* param_data = new Param_Data;
  int param_size = (h.dstart - 2) * 512;
  param_size -= sizeof(Param);
  param_data->proc = param.proc;
  param_data->aStartField = -1;
  param_data->aEndField = -1;
  load_parameter_c3d(fd, param_data, &param_size);

  //printf("whereami %d\n", (int) lseek(fd, 0, SEEK_CUR));

  // check header
  size_t marker_count = h.np3d;
  if(marker_count > 512) { close(fd); return -3; }
  size_t frame_count = 0;
  if (param_data->aStartField >= 0 && param_data->aEndField > 0) {
    if (param_data->aStartField > param_data->aEndField) { close(fd); return -4; }
    frame_count = param_data->aEndField - param_data->aStartField + 1;
  } else {
    if(h.nff > h.nlf) { close(fd); return -4; }
    frame_count = h.nlf - h.nff + 1;
  }
  cout << "frames=" << frame_count << endl;
  recorded.resize(frame_count);
#endif
  
  
  // below is unchanged due to parameter section modifications
  // read frame data
  for(size_t f = 0; f < frame_count; f++)
    {
      vector<OWLMarker> &frame = recorded[f];
      
      frame.resize(marker_count);
      
      for(size_t m = 0; m < marker_count; m++)
        {
          OWLMarker &marker = frame[m];

          marker.id = m;
          marker.frame = f + h.nff - 1;

          ret = read(fd, buf, 4 * sizeof(float));
          
          if(ret < 4 * (int)sizeof(float)) { close(fd); return -5; }

          {
            //size_t *ptr = (size_t*)buf;
            //for(size_t i = 0; i < 4; i++) cout << " " << hex << ptr[i];  cout << dec << endl;
          }

          float *ptr = (float*)buf;

          float scale = 1;
          //if(h.scfact > 0) scale = h.scfact;

          // read X,Z,Y positions
          if(param.proc == P_INTEL)
            {
              marker.x = ptr[0] * scale;
              marker.z = ptr[1] * scale;
              marker.y = ptr[2] * scale;
              marker.cond = ptr[3];
            }
          else if(param.proc == P_DEC)
            {
              marker.x = fromDEC(ptr[0]) * scale;
              marker.z = fromDEC(ptr[1]) * scale;
              marker.y = fromDEC(ptr[2]) * scale;
              marker.cond = fromDEC(ptr[3]);
            }
          else
            {
              cerr << "invalid processor type: " << (int)param.proc << endl;
            }

          // 0 in c3d means 'good' // not anymore (2006-04-12) -  0 means cleaned or interpolated data
          //if(marker.cond == 0) marker.cond = 1; // commented out 2006-04-12

          //cout << "marker: " << marker.x << " " << marker.y << " " << marker.z << " " << marker.cond << endl;

          count++;
        }
    }

  return count;
}

#endif // C3D_FILE_H
