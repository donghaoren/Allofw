// owl_scan.h
// scans for owl servers

#ifndef SCAN_H
#define SCAN_H

#ifdef WIN32
#ifdef __DLL
#define OWLAPI __declspec(dllexport)
#else // !__DLL
#define OWLAPI __declspec(dllimport)
#endif // __DLL
#else // ! WIN32
#define OWLAPI
#endif // WIN32

struct OWLConnection {
  char address[256];
  char info[256];
};

#ifdef __cplusplus
extern "C"
#endif
OWLAPI int owl_scan(const char *address, int port, const char *msg, size_t timeout, OWLConnection *cons, size_t count);

#endif // SCAN_H
