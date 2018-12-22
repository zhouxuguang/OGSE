/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2011
@ �ļ����	: GeoDefine.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2011��12��21��
@ ����ժҪ	: Ԥ�����һЩ���� by zxg 
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef  __GEODEFINE_H_91ED9AC8_749B_4AE1_AE47_B8F95E03B0A6__
#define  __GEODEFINE_H_91ED9AC8_749B_4AE1_AE47_B8F95E03B0A6__


//����������
#if defined _WIN32 || defined __CYGWIN__
	#ifdef GEOMETRYDLL_EXPORTS		
		#ifdef __GNUC__
			#define GEOMETRY_API __attribute__((dllexport))
		#else
			#define GEOMETRY_API __declspec(dllexport)
		#endif
	#else
	#ifdef __GNUC__
		#define GEOMETRY_API __attribute__((dllimport))
		#else
		#define GEOMETRY_API __declspec(dllimport)
		#endif
	#endif
	#define GEOMETRY_HIDE
#else
	#if __GNUC__>=4
		#define GEOMETRY_API __attribute__ ((visibility("default")))
		#define GEOMETRY_HIDE __attribute__ ((visibility("hidden")))
	#else
		#define GEOMETRY_API
		#define GEOMETRY_HIDE
	#endif
#endif


//��׼���÷�ʽ
#if defined _WIN32 || defined __CYGWIN__

	#define GEOMETRY_API_CALL __stdcall
#else
	#if __GNUC__>=4
		#define GEOMETRY_API_CALL __attribute__((__stdcall__))
	#endif
#endif

//�����ӿڷ����
#ifdef __GNUC__
	#define DEPRECATE_API __attribute__((deprecated))
#elif defined _WIN32 && defined (_MSC_VER) || defined __CYGWIN__
	#define DEPRECATE_API __declspec(deprecated)
#else
	#define DEPRECATE_API
	#pragma message("DEPRECATE_API"�ڴ˱�������û�ж���)
#endif

/************************************************************************/
/* �����ַ�Ƚϵĺ�                                                   */
/************************************************************************/
#ifndef EQUAL
#if defined(WIN32) || defined(WIN32CE)
	#define STRCASECMP(a,b)         (stricmp(a,b))
	#define STRNCASECMP(a,b,n)      (strnicmp(a,b,n))
#else
#include <string.h>
	#define STRCASECMP(a,b)         (strcasecmp(a,b))
	#define STRNCASECMP(a,b,n)      (strncasecmp(a,b,n))
#endif
	#define EQUALN(a,b,n)           (STRNCASECMP(a,b,n)==0)
	#define EQUAL(a,b)              (STRCASECMP(a,b)==0)
#endif

//C���ӷ�ʽ�궨��
#ifdef __cplusplus
	#define EXTERN_C_START           extern "C" {
	#define EXTERN_C_END             }
#else
	#define EXTERN_C_START
	#define EXTERN_C_END
#endif

#include <utility>
using namespace std;

//����Ƚϴ�С�ĺ�
#define xg_min(a,b) (a<b?a:b)
#define xg_max(a,b) (a>b?a:b)

	  

/************************************************************************/
/* ��������Ͷ���                                                     */
/************************************************************************/

	typedef float				GFloat;
	typedef double				GDouble;
	typedef unsigned long		GWord;

#if defined __linux__
#include <unistd.h>
	typedef __int64_t			GLonglong;
	typedef __uint64_t			GULonglong;
	typedef __int32_t		    GInt32;
	typedef __uint32_t		    GUInt32;
	typedef __int16_t		    GInt16;
	typedef __uint16_t		    GUInt16;
	typedef __uint8_t  			GByte;

#elif (defined _WIN32 ||  defined _WIN64) && defined (_MSC_VER) || defined __CYGWIN__
	typedef __int64				GLonglong;
	typedef unsigned __int64	GULonglong;
	typedef signed   __int32    GInt32;
	typedef unsigned __int32    GUInt32;
	typedef signed   __int16    GInt16;
	typedef unsigned __int16    GUInt16;
	typedef unsigned __int8     GByte;
#endif

#define GeoNullFID				-1
#define GeoUnsetMarker			-21121

#define SPF_FID					0
#define SPF_OGR_GEOMETRY		1
#define SPF_OGR_STYLE			2
#define SPF_OGR_GEOM_WKT		3
#define SPF_OGR_GEOM_AREA		4
#define SPECIAL_FIELD_COUNT		5


typedef enum  //WKB��������ö��
{
	gWkbUnKnow = 0,
	gWkbPoint = 1,
	gWkbLineString = 2,
	gWkbPolygon = 3,

	gWkbMultiPoint = 4,      /* ��� */
	gWkbMultiLineString = 5, /* ���� */
	gWkbMultiPolygon = 6,    /* ���� */
	gWkbGeometryCollection = 7, /* �����弯��*/

	gWkbNone = 100,          /* */
	gWkbLinearRing = 101,    /* */
	gWkbPoint25D = 0x80000001, /* */
	gWkbLineString25D = 0x80000002, /* */
	gWkbPolygon25D = 0x80000003, /* */
	gWkbMultiPoint25D = 0x80000004, /* */
	gWkbMultiLineString25D = 0x80000005, /* */
	gWkbMultiPolygon25D = 0x80000006, /* */
	gWkbGeometryCollection25D = 0x80000007 /* */
}GeoWkbGeometryType;

#define gWkb25DBit 0x80000000
#define gWkbFlatten(x)  ((GeoWkbGeometryType) ((x) & (~gWkb25DBit)))

//�����ݲ�
const double eps = 0.0000000000001;

//�ֽ�˳��
typedef enum 
{
	gWkbXDR = 0,         //��β��
	gWkbNDR = 1          //Сβ��
} GeoByteOrder;

//�ֶ�����
typedef enum 
{
	gWkbInteger = 0,				//32λ����
	gWkbIntegerList = 1,			//32λ�����б�
	gWkbReal = 2,					//˫���ȸ�����
	gWkbRealList = 3,				//˫���ȸ������б�
	gWkbString = 4,					//�ַ�
	gWkbStringList = 5,				//�ַ��б�
	gWkbWideString = 6,				//���ַ�		��������
	gWkbWideStringList = 7,			//���ַ��б�	  ��������
	gWkbBinary = 8,					//������
	gWkbDate = 9,					//����
	gWkbTime = 10,					//ʱ��
	gWkbDateTime = 11,				//���ں�ʱ��
	gWkbMaxType = 12				//��Ч����
} GeoFieldType;

//�ֶζ��뷽ʽ
typedef enum 
{
	gWkbUndefined = 0,				//δ����
	gWkbLeft = 1,				   //�����
	gWkbRight = 2				   //�Ҷ���
} GeoJustification;

//�ֶ�(���������,Ҳ����ͨ���������)
typedef union {
	int         Integer;		//����
	double      Real;			//������
	char       *String;			//�ַ�

	struct {
		int     nCount;
		int     *paList;
	} IntegerList;				 //�����б�

	struct {
		int     nCount;
		double  *paList;
	} RealList;					 //�������б�

	struct {
		int     nCount;
		char    **paList;
	} StringList;				 //�ַ��б�

	struct {
		int     nCount;
		GByte   *paData;
	} Binary;					 //������

	struct {
		int     nMarker1;
		int     nMarker2;
	} Set;						//����

	struct {
		short	Year;
		GByte   Month;
		GByte   Day;
		GByte   Hour;
		GByte   Minute;
		GByte   Second;
		GByte   TZFlag; /* 0=unknown, 1=localtime(ambiguous), 
						100=GMT, 104=GMT+1, 80=GMT-5, etc */
	} Date;						//��������
} GeoField;

#if defined linux

void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);

#endif


#endif // end of __GEODEFINE_H_F9446D54_EBF3_44DB_97EF_28B1A376A31F__
