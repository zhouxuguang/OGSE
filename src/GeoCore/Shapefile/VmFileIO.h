#ifndef VMFILEIO_INCLUDE
#define VMFILEIO_INCLUDE

#include <memory.h>

#include "shapefil.h"
//#include "GeoDefine.h"

//EXTERN_C_START


//��ӳ���ļ�
SAFile mfopen( const char *pszFilename, const char *pszAccess );

SAOffset mfread( void *p, SAOffset size, SAOffset nmemb, SAFile file );

int mfclose( SAFile file );


//EXTERN_C_END


#endif