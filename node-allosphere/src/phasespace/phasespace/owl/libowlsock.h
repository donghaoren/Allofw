
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBOWLSOCK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBOWLSOCK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBOWLSOCK_EXPORTS
#define LIBOWLSOCK_API __declspec(dllexport)
#else
#define LIBOWLSOCK_API __declspec(dllimport)
#endif

// This class is exported from the libowlsock.dll
class LIBOWLSOCK_API CLibowlsock {
public:
	CLibowlsock(void);
	// TODO: add your methods here.
};

extern LIBOWLSOCK_API int nLibowlsock;

LIBOWLSOCK_API int fnLibowlsock(void);

