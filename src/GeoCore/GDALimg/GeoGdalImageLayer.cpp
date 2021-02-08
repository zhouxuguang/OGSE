
#include <assert.h>
#include <float.h>

#include <algorithm>

#include "GeoRasterDefine.h"

#include "GeoImageLayer.h"
#include "GeoGdalImageLayer.h"
#include "gdal_priv.h"

#include "GeoEnvelope.h"
#include "GeoSpatialReference.h"
#include "GeoRasterDefine.h"
#include "../GeoImagePreProcess.h"

class GeoGdalImageLayer;


GeoImageLayer* GeoImageOpen(const char* pszName,int bUpdata,void* pVoid)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	GeoImageLayer* pImageLayer =  new GeoGdalImageLayer;
	if (!pImageLayer->Open(pszName,bUpdata,pVoid))
	{
		delete pImageLayer;
		return NULL;
	}

	return pImageLayer;
}

GeoGdalImageLayer::GeoGdalImageLayer(void)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	m_poDataset = NULL;
}

GeoGdalImageLayer::~GeoGdalImageLayer(void)
{
	Close();
	m_poDataset = NULL;
}

int GeoGdalImageLayer::Open(const char* pszName,int bUpdata,void* pVoid /* = NULL */)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	if (m_poDataset == NULL)
	{
		m_poDataset = (GDALDataset*)GDALOpen(pszName,(GDALAccess)bUpdata);
		if (NULL != m_poDataset)
		{
			m_nBandCount = ((GDALDataset*)m_poDataset)->GetRasterCount();
			m_nHeight = ((GDALDataset*)m_poDataset)->GetRasterYSize();
			m_nWidth = ((GDALDataset*)m_poDataset)->GetRasterXSize();
			//m_pszLayerName = strdup(pszName);
			//m_pszDataSource = strdup(pszName);

			//看看影像是否有金字塔
			GDALDataset* poDataset = (GDALDataset*)m_poDataset;
			GDALRasterBand* poBand = poDataset->GetRasterBand(1);
			int nOvrCount = poBand->GetOverviewCount();
			if ( 0 == nOvrCount )		//需要建立金字塔
			{
				int *pBandList = new int[m_nBandCount];
				for (int i = 0; i < m_nBandCount; i ++)
				{
					pBandList[i] = i + 1;
				}
				BuildOverviews(m_nBandCount,pBandList,NULL,NULL);
				delete []pBandList;
			}
		}

		else
		{
			m_nHeight = 0;
			m_nWidth = 0;
			m_nBandCount = 0;
		}
	}	

	return 1;
}

void GeoGdalImageLayer::Close()
{
	if (m_poDataset == NULL)
	{
		GDALDataset *poDataset = (GDALDataset *)m_poDataset;
		GDALClose((GDALDatasetH)poDataset);

		m_nHeight = 0;
		m_nWidth = 0;
		m_nBandCount = 0;
	}	
}

bool GeoGdalImageLayer::GetExtent(GeoEnvelope* pExtent) const
{
	if (NULL == pExtent)
	{
		return false;
	}
	double dbMinx = 0,dbMaxx = 0,dbMiny = 0,dbMaxy = 0;
	
	double dblGeoTransform[6];
	GDALDataset* poDataset = (GDALDataset*)m_poDataset;
	if (NULL == poDataset)
	{
		return false;
	}
	
	if(CE_None == poDataset->GetGeoTransform(dblGeoTransform))
	{
		PixelToWorld(0,0,dbMinx,dbMaxy);
		PixelToWorld(m_nWidth,m_nHeight,dbMaxx,dbMiny);

		pExtent->Init(dbMinx,dbMaxx,dbMiny,dbMaxy);
	}

	else if(CE_None != poDataset->GetGeoTransform(dblGeoTransform))
	{
		int nGCPCount = poDataset->GetGCPCount();	//是否有控制点

		if (nGCPCount > 0)
		{
			const GDAL_GCP* pGCPList = poDataset->GetGCPs();

			int bFail = GDALGCPsToGeoTransform(nGCPCount,pGCPList,dblGeoTransform,1);
			double dbX[4];
			double dbY[4];
			dbX[0] = dblGeoTransform[0] + dblGeoTransform[1] + dblGeoTransform[2];
			dbY[0] = dblGeoTransform[3] + dblGeoTransform[4] + dblGeoTransform[5];

			//右上角坐标
			dbX[1] = dblGeoTransform[0] + (m_nWidth)*dblGeoTransform[1] + dblGeoTransform[2];
			dbY[1] = dblGeoTransform[3] + (m_nWidth)*dblGeoTransform[4] + dblGeoTransform[5];

			//右下角点坐标
			dbX[2] = dblGeoTransform[0] + (m_nWidth)*dblGeoTransform[1] + (m_nHeight)*dblGeoTransform[2];
			dbY[2] = dblGeoTransform[3] + (m_nWidth)*dblGeoTransform[4] + (m_nHeight)*dblGeoTransform[5];

			//左下角坐标
			dbX[3] = dblGeoTransform[0] + dblGeoTransform[1] + (m_nHeight)*dblGeoTransform[2];
			dbY[3] = dblGeoTransform[3] + dblGeoTransform[4] + (m_nHeight)*dblGeoTransform[5];

			dbMinx = min(min(min(dbX[0],dbX[1]),dbX[2]),dbX[3]);
			dbMaxx = max(max(max(dbX[0],dbX[1]),dbX[2]),dbX[3]);
			dbMiny = min(min(min(dbY[0],dbY[1]),dbY[2]),dbY[3]);
			dbMaxy = max(max(max(dbY[0],dbY[1]),dbY[2]),dbY[3]);
			pExtent->Init(dbMinx,dbMaxx,dbMiny,dbMaxy);
		}
	}

	return true;
	
}

int GeoGdalImageLayer::GetWidth() const
{
	return m_nWidth;
}

int GeoGdalImageLayer::GetHeight() const
{
	return m_nHeight;
}

int GeoGdalImageLayer::GetBandCount() const
{
	return m_nBandCount;
}

int GeoGdalImageLayer::HasColorTable() const
{
	GDALDataset* poDataset = (GDALDataset*)m_poDataset;
	if (NULL == poDataset)
	{
		return 0;
	}
	GDALRasterBand *poBand = poDataset->GetRasterBand(1);
	if (NULL == poBand)
	{
		return 0;
	}
	GDALColorTable * pColorTab = poBand->GetColorTable();
	if (NULL == pColorTab)
	{
		return 0;
	}

	int nEntryCount = pColorTab->GetColorEntryCount();

	return nEntryCount > 0;
}

int GeoGdalImageLayer::GetColorTable(int nBandIndex,std::vector<ColorTable> &vecColorTables) const
{
	//std::vector<ColorTable> vecColorTables;
	vecColorTables.clear();
	if (nBandIndex < 0 || nBandIndex >= m_nBandCount)
	{
		return 0;
	}

	GDALRasterBand *poBand = ( (GDALDataset*)m_poDataset )->GetRasterBand(nBandIndex+1);
	if (NULL == poBand)
	{
		return 0;
	}
	GDALColorTable * pColorTab = poBand->GetColorTable();
	if (NULL == pColorTab)
	{
		return 0;
	}
	int nEntryCount = pColorTab->GetColorEntryCount();
	for (int i = 0; i < nEntryCount; i ++)
	{
		const GDALColorEntry* pEntry = pColorTab->GetColorEntry(i);
		if (NULL == pEntry)
		{
			continue;
		}

		ColorTable stTable;
		stTable.c1 = pEntry->c1;
		stTable.c2 = pEntry->c2;
		stTable.c3 = pEntry->c3;
		stTable.c4 = pEntry->c4;

		vecColorTables.push_back(stTable);
	}

	return 1;
}

GeoSpatialReference *GeoGdalImageLayer::GetSpatialRef() const
{
	if (NULL == m_poDataset)
	{
		return NULL;
	}

	const char* pszWkt = ((GDALDataset*)m_poDataset)->GetProjectionRef();
	if (NULL == pszWkt)
	{
		return NULL;
	}
	if (EQUAL(pszWkt,""))
	{
		pszWkt = ((GDALDataset*)m_poDataset)->GetGCPProjection();
		if (EQUAL(pszWkt,""))
		{
			return NULL;
		}
		
	}

	static GeoSpatialReference oSrs;
	oSrs.ImportFromWkt(pszWkt);
	return &oSrs;
}

void GeoGdalImageLayer::SetSpatialReference(GeoSpatialReference *pSRS)
{
	GDALDataset *poDataset = (GDALDataset*)m_poDataset;
	if (NULL == poDataset)
	{
		return;
	}
	if (NULL == pSRS)
	{
		return;
	}
	std::string strWkt;
	pSRS->ExportToWkt(strWkt);
	poDataset->SetProjection(strWkt.c_str());

	poDataset->FlushCache();
}

int GeoGdalImageLayer::ReadData(int nBandIndex,int nXstart,int nYstart,int nWidth,int nHeight,
								int nBufXSize,int nBufYSize,RASDataType eDataType,void* poData)
{
	if (nBandIndex <= m_nBandCount && nBandIndex >0)
	{
		if (m_poDataset != NULL)
		{
			GDALRasterBand* poBand = ((GDALDataset*)m_poDataset)->GetRasterBand(nBandIndex);
			if (NULL == poBand)
			{
				return 0;
			}
			CPLErr err = poBand->RasterIO(GF_Read,nXstart,nYstart,nWidth,nHeight,poData,nBufXSize,nBufYSize,(GDALDataType)eDataType,0,0);
			return err == CE_None;
		}
		
	}

	return 0;
}

int GeoGdalImageLayer::ReadData(int nBandCount,
								int		*pBandIndex,
								int		nXstart,
								int		nYstart,
								int		nWidth,
								int		nHeight,
								int		nBufXSize,
								int		nBufYSize,
								RASDataType eDataType,
								void    *poData,
								int 	nPixelSpace,
								int 	nLineSpace,
								int 	nBandSpace)
{
	assert(nBandCount > 0);
	assert(pBandIndex != NULL);
	assert(poData != NULL);

	if (m_poDataset != NULL)
	{
		int nDataSize = RASGetDataTypeSize(eDataType);
		( (GDALDataset*)m_poDataset )->RasterIO(GF_Read,nXstart,nYstart,nWidth,nHeight,
			poData,nBufXSize,nBufYSize,(GDALDataType)eDataType,nBandCount,pBandIndex,nBandCount*nDataSize,
			nBufXSize*nBandCount*nDataSize,1*nDataSize);			//先默认按照BIP格式来读了

		return 1;
	}

	return 0;
}

int GeoGdalImageLayer::ReadDataNormalLized(int nBandIndex, 
										   int nLevel, 
										   int nXstart, 
										   int nYstart, 
										   int nWidth, 
										   int nHeight, 
										   int nBuffSizeX, 
										   int nBuffSizeY, 
										   GByte* pBuffer)
{
	assert(nLevel >= 0 && nXstart >= 0 && nYstart >= 0 && nWidth >= 0 && nHeight >= 0);
	assert(nBuffSizeX >= 0 && nBuffSizeY >= 0);
	assert(NULL != pBuffer);
	short* psSrcData = NULL;
	unsigned short* pusSrcData = NULL;
	int* pSrcDataInt = NULL;
	unsigned int* puiSrcData = NULL;
	float* pSrcDataFloat = NULL;
	double* pdbSrcData = NULL;

	GDALDataset *poDataset = (GDALDataset *)m_poDataset;
	if (NULL == poDataset)
	{
		return 0;
	}
	GDALRasterBand *poBand = poDataset->GetRasterBand(1);
	if (NULL == poBand)
	{
		return 0;
	}

	GDALDataType dt = poBand->GetRasterDataType();
	GLonglong lBufferSize = nBuffSizeX * nBuffSizeY;
	switch(dt)
	{
	case GDT_Byte:
		{
			break;
		}
	case GDT_Int16:
		{
			psSrcData = (short*)CPLMalloc(sizeof(short)*lBufferSize);
			break;
		}
	case GDT_UInt16:
		{
			pusSrcData = (unsigned short*)CPLMalloc(sizeof(unsigned short)*lBufferSize);
			break;
		}
	case GDT_Int32:
		{
			pSrcDataInt = (int*)CPLMalloc(sizeof(int)*lBufferSize);
			break;
		}
	case GDT_UInt32:
		{
			puiSrcData = (unsigned int*)CPLMalloc(sizeof(unsigned int)*lBufferSize);
			break;
		}
	case GDT_Float32:
		{
			pSrcDataFloat = (float*)CPLMalloc(sizeof(float)*lBufferSize);
			break;
		}
	case GDT_Float64:
		{
			pdbSrcData = (double*)CPLMalloc(sizeof(double)*lBufferSize);
			break;
		}
	default:
		assert(false);
	}
	CPLErr Err = CE_Failure;

	switch(dt)
	{
	case GDT_Byte:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pBuffer);
			break;
		}
	case GDT_Int16:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,psSrcData);
			short sMin = 0;
			short sMax = 0;
			double dbRealMin = 0.0;
			double dbRealMax = 0.0;
			GetBandMaxAndMin(nBandIndex,dbRealMax,dbRealMin);
			sMin = static_cast<short>(dbRealMin);
			sMax = static_cast<short>(dbRealMax);

			NormalLizeData<short>(psSrcData,pBuffer,lBufferSize,sMin,sMax);
			CPLFree(psSrcData);
			break;
		}
	case GDT_UInt16:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pusSrcData);
			unsigned short usMin = 0;
			unsigned short usMax = 0;
			double dbRealMin = 0.0;
			double dbRealMax = 0.0;
			GetBandMaxAndMin(nBandIndex,dbRealMax,dbRealMin);
			usMin = static_cast<unsigned short>(dbRealMin);
			usMax = static_cast<unsigned short>(dbRealMax);

			NormalLizeData<unsigned short>(pusSrcData,pBuffer,lBufferSize,usMin,usMax);
			CPLFree(pusSrcData);
			break;
		}
	case GDT_Int32:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pSrcDataInt);
			int nMinValue = 0;
			int nMaxValue = 0;
			double dbRealMin = 0.0;
			double dbRealMax = 0.0;
			GetBandMaxAndMin(nBandIndex,dbRealMax,dbRealMin);
			nMinValue = static_cast<int>(dbRealMin);
			nMaxValue = static_cast<int>(dbRealMax);

			NormalLizeData<int>(pSrcDataInt,pBuffer,lBufferSize,nMinValue,nMaxValue);
			CPLFree(pSrcDataInt);
			break;
		}
	case GDT_UInt32:
		{

			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,puiSrcData);
			unsigned int ulMinValue = 0UL;
			unsigned int ulMaxValue = 0UL;

			double dbRealMax = 0.0;
			double dbRealMin = 0.0;
			GetBandMaxAndMin(nBandIndex,dbRealMax,dbRealMin);
			ulMinValue = static_cast<unsigned int>(dbRealMin);
			ulMaxValue = static_cast<unsigned int>(dbRealMax);

			NormalLizeData<unsigned int>(puiSrcData,pBuffer,lBufferSize,ulMinValue,ulMaxValue);
			CPLFree(puiSrcData);
			break;
		}
	case GDT_Float32:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pSrcDataFloat);

			double dbRealMax = 0.0;
			double dbRealMin = 0.0;
			GetBandMaxAndMin(nBandIndex,dbRealMax,dbRealMin);
			float fMinValue = dbRealMin;
			float fMaxValue = dbRealMax;
			NormalLizeData<float>(pSrcDataFloat,pBuffer,lBufferSize,fMinValue,fMaxValue);
			CPLFree(pSrcDataFloat);
			break;
		}
	case GDT_Float64:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pdbSrcData);
			double dbRealMax = 0.0;
			double dbRealMin = 0.0;
			GetBandMaxAndMin(nBandIndex,dbRealMax,dbRealMin);

			NormalLizeData<double>(pdbSrcData,pBuffer,lBufferSize,dbRealMin,dbRealMax);
			CPLFree(pdbSrcData);
			break;
		}
	default:
		assert(false);
		break;
	}
	return Err;
}

int GeoGdalImageLayer::ReadDataStdDev(int nBandIndex, 
										   int nLevel, 
										   int nXstart, 
										   int nYstart, 
										   int nWidth, 
										   int nHeight, 
										   int nBuffSizeX, 
										   int nBuffSizeY, 
										   GByte* pBuffer)
{
	assert(nLevel >= 0 && nXstart >= 0 && nYstart >= 0 && nWidth >= 0 && nHeight >= 0);
	assert(nBuffSizeX >= 0 && nBuffSizeY >= 0);
	assert(NULL != pBuffer);
	short* psSrcData = NULL;
	unsigned short* pusSrcData = NULL;
	int* pSrcDataInt = NULL;
	unsigned int* puiSrcData = NULL;
	float* pSrcDataFloat = NULL;
	double* pdbSrcData = NULL;

	GDALDataset *poDataset = (GDALDataset *)m_poDataset;
	if (NULL == poDataset)
	{
		return 0;
	}
	GDALRasterBand *poBand = poDataset->GetRasterBand(1);
	if (NULL == poBand)
	{
		return 0;
	}

	GDALDataType dt = poBand->GetRasterDataType();
	GLonglong lBufferSize = nBuffSizeX * nBuffSizeY;
	switch(dt)
	{
	case GDT_Byte:
		{
			break;
		}
	case GDT_Int16:
		{
			psSrcData = (short*)malloc(sizeof(short)*lBufferSize);
			break;
		}
	case GDT_UInt16:
		{
			pusSrcData = (unsigned short*)malloc(sizeof(unsigned short)*lBufferSize);
			break;
		}
	case GDT_Int32:
		{
			pSrcDataInt = (int*)malloc(sizeof(int)*lBufferSize);
			break;
		}
	case GDT_UInt32:
		{
			puiSrcData = (unsigned int*)malloc(sizeof(unsigned int)*lBufferSize);
			break;
		}
	case GDT_Float32:
		{
			pSrcDataFloat = (float*)malloc(sizeof(float)*lBufferSize);
			break;
		}
	case GDT_Float64:
		{
			pdbSrcData = (double*)malloc(sizeof(double)*lBufferSize);
			break;
		}
	default:
		assert(false);
	}
	CPLErr Err = CE_Failure;

	switch(dt)
	{
	case GDT_Byte:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pBuffer);
			break;
		}
	case GDT_Int16:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,psSrcData);
			double dbAve = 0.0;
			double dbStdDev = 0.0;
			GetStatistics(nBandIndex,NULL,NULL,&dbAve,&dbStdDev,NULL,NULL);

 			float fMax,fMin,fMean,fVar;
 			GetBandStatisInfoEx(nBandIndex,NULL,fMax,fMin,fMean,fVar,0);
			StandDevStreh<short>(psSrcData,pBuffer,nBuffSizeX*nBuffSizeY,fVar,3,fMean);
			free(psSrcData);
			break;
		}
	case GDT_UInt16:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pusSrcData);
			double dbAve = 0.0;
			double dbStdDev = 0.0;
			//GetStatistics(nBandIndex,NULL,NULL,&dbAve,&dbStdDev,NULL,NULL);
			float fMax,fMin,fMean,fVar;
			GetBandStatisInfoEx(nBandIndex,NULL,fMax,fMin,fMean,fVar,0);
			StandDevStreh<unsigned short>(pusSrcData,pBuffer,nBuffSizeX*nBuffSizeY,fVar,2,fMean);
			free(pusSrcData);
			break;
		}
	case GDT_Int32:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pSrcDataInt);
			double dbAve = 0.0;
			double dbStdDev = 0.0;
			GetStatistics(nBandIndex,NULL,NULL,&dbAve,&dbStdDev,NULL,NULL);
			StandDevStreh<int>(pSrcDataInt,pBuffer,nBuffSizeX*nBuffSizeY,dbStdDev,2,dbAve);
			free(pSrcDataInt);
			break;
		}
	case GDT_UInt32:
		{

			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,puiSrcData);
			double dbAve = 0.0;
			double dbStdDev = 0.0;
			GetStatistics(nBandIndex,NULL,NULL,&dbAve,&dbStdDev,NULL,NULL);
			StandDevStreh<unsigned int>(puiSrcData,pBuffer,nBuffSizeX*nBuffSizeY,dbStdDev,2,dbAve);
			free(puiSrcData);
			break;
		}
	case GDT_Float32:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pSrcDataFloat);

			double dbAve = 0.0;
			double dbStdDev = 0.0;
			GetStatistics(nBandIndex,NULL,NULL,&dbAve,&dbStdDev,NULL,NULL);
			StandDevStreh<float>(pSrcDataFloat,pBuffer,nBuffSizeX*nBuffSizeY,dbStdDev,2,dbAve);
			free(pSrcDataFloat);
			break;
		}
	case GDT_Float64:
		{
			Err = (CPLErr)ReadDataBand(nBandIndex,nLevel,nXstart,nYstart,nWidth,nHeight,nBuffSizeX,nBuffSizeY,pdbSrcData);
			double dbAve = 0.0;
			double dbStdDev = 0.0;
			GetStatistics(nBandIndex,NULL,NULL,&dbAve,&dbStdDev,NULL,NULL);
			StandDevStreh<double>(pdbSrcData,pBuffer,nBuffSizeX*nBuffSizeY,dbStdDev,2,dbAve);
			free(pdbSrcData);
			break;
		}
	default:
		assert(false);
		break;
	}
	return Err;
}

int GeoGdalImageLayer::ReadDataBand(int nBandIndex, 
									int nLevel, 
									int nXstart, 
									int nYstart, 
									int nWidth, 
									int nHeight, 
									int nBuffSizeX, 
									int nBuffSizeY, 
									void* pBuffer)
{
	CPLErr Err = CE_Failure;
	if (NULL == pBuffer)
	{
		return Err; 
	}

	GDALDataset *poDataset = (GDALDataset*)m_poDataset;
	if (NULL == poDataset)
	{
		return Err; 
	}

	GDALRasterBand* poBand = NULL;
	poBand = poDataset->GetRasterBand(nBandIndex+1);
	if (NULL == poBand)
	{
		return Err;
	}
	
	GDALRasterBand* pPyrBand = NULL;
	if (0 == nLevel)
	{
		pPyrBand =  poBand;
	}
	else
	{
		pPyrBand =  poBand->GetOverview(nLevel-1);
	}

	if (NULL == pPyrBand)
	{
		return Err;
	}
	GDALDataType eDataType = poBand->GetRasterDataType();
	Err = pPyrBand->RasterIO(GF_Read,nXstart,nYstart,nWidth,nHeight,pBuffer,nBuffSizeX,nBuffSizeY,eDataType,0,0);
	return Err;
}

int GeoGdalImageLayer::PixelToWorld(long nCol,long nRow,double & dbX,double &dbY) const
{
	double padfTransform[6];
	if (m_poDataset != NULL)
	{
		( (GDALDataset*)m_poDataset)->GetGeoTransform(padfTransform);
		dbX = padfTransform[0] + nCol*padfTransform[1] + nRow*padfTransform[2];
		dbY = padfTransform[3] + nCol*padfTransform[4] + nRow*padfTransform[5];

		return 1;
	}

	return 0;
}

int GeoGdalImageLayer::WorldToPixel(double dbX,double dbY,long &nCol,long &nRow) const
{
	if (m_poDataset != NULL)
	{
		double padfTransform[6];
		((GDALDataset*)m_poDataset)->GetGeoTransform(padfTransform);

		double adfInvGeoTransform[6];
		//if( ! )
		GDALInvGeoTransform( padfTransform, adfInvGeoTransform );

		nCol = (int) floor(
			adfInvGeoTransform[0]
		+ adfInvGeoTransform[1] * dbX
			+ adfInvGeoTransform[2] * dbY );
		nRow = (int) floor(
			adfInvGeoTransform[3]
		+ adfInvGeoTransform[4] * dbX
			+ adfInvGeoTransform[5] * dbY );

		return 1;
	}

	return 0;
}

int GeoGdalImageLayer::GetHistogram( int nBand, int *panHistogram )
{
	if (m_poDataset == NULL)
	{
		return 0;
	}
	else if ((m_poDataset != NULL) && (nBand <1 || nBand > ((GDALDataset*)m_poDataset)->GetRasterCount()))
	{
		return 0;
	}
	int        nXBlocks, nYBlocks, nXBlockSize, nYBlockSize;
	int        iXBlock, iYBlock;
	GByte      *pabyData;
	memset( panHistogram, 0, sizeof(int) * 256 );
	CPLAssert( poBand->GetRasterDataType() == GDT_Byte );
	GDALRasterBand *poBand = ((GDALDataset*)m_poDataset)->GetRasterBand(nBand);

	//获得每一图像块的大小
	poBand->GetBlockSize( &nXBlockSize, &nYBlockSize );
	nXBlocks = (poBand->GetXSize() + nXBlockSize - 1) / nXBlockSize;
	nYBlocks = (poBand->GetYSize() + nYBlockSize - 1) / nYBlockSize;
	pabyData = (GByte *) CPLMalloc(nXBlockSize * nYBlockSize);
	for( iYBlock = 0; iYBlock < nYBlocks; iYBlock++ )
	{
		for( iXBlock = 0; iXBlock < nXBlocks; iXBlock++ )
		{
			int        nXValid, nYValid;
			poBand->ReadBlock( iXBlock, iYBlock, pabyData );
			
			//计算图像块边界的索引值
			if( (iXBlock+1) * nXBlockSize > poBand->GetXSize() )
				nXValid = poBand->GetXSize() - iXBlock * nXBlockSize;
			else
				nXValid = nXBlockSize;
			if( (iYBlock+1) * nYBlockSize > poBand->GetYSize() )
				nYValid = poBand->GetYSize() - iYBlock * nYBlockSize;
			else
				nYValid = nYBlockSize;
			// Collect the histogram counts.
			for( int iY = 0; iY < nYValid; iY++ )
			{
				for( int iX = 0; iX < nXValid; iX++ )
				{
					panHistogram[pabyData[iX + iY * nXBlockSize]] += 1;
				}
			}
		}
	}

	return 1;
}

int GeoGdalImageLayer::BuildOverviews(
									  int nListBands, 
									  int * panBandList, 
									  RASProgressFunc pfnProgress, 
									  void * pProgressData )
{
	int nMaxExtent = 0;
	if (m_nHeight <= m_nWidth)
		nMaxExtent = m_nWidth;
	else
		nMaxExtent = m_nHeight;

	if (nMaxExtent < 128)
	{
		return false;
	}

	int nLevels = log(nMaxExtent/128.0)/log(2.0);

	int* pBandList = new int[nLevels];
	for (int i=0; i<nLevels; ++i)
	{
		pBandList[i] = int(pow(2.0,double(i+1)));
	}

	CPLSetConfigOption("USE_RRD","YES");		//产生RRD格式金字塔
	CPLErr errorCode = CE_Failure;
	errorCode = ((GDALDataset*)m_poDataset)->BuildOverviews("NEAREST",nLevels,pBandList,nListBands,panBandList,
		pfnProgress,pProgressData);

	if (pBandList != NULL)
	{
		delete []panBandList;
		panBandList = NULL;
	}
	return errorCode == CE_None;
}

bool GeoGdalImageLayer::GetPyramidSize(int nLevel,double& dbPyramidWdith,double& dbPyramidHeight) const
{
	GDALDataset *poDataset = (GDALDataset*)m_poDataset;
	if (NULL==poDataset)
	{
		return false;
	}

	GDALRasterBand* pBand = poDataset->GetRasterBand(1);
	if (NULL == pBand)
	{
		return false;
	}
	int nOvrCount = pBand->GetOverviewCount();
	if (nLevel < 0 || nLevel > nOvrCount)
	{
		return false;
	}
	GDALRasterBand* poverBand = NULL;
	if (0 == nLevel)
	{
		poverBand = pBand;
	}
	else
	{
		poverBand = pBand->GetOverview(nLevel - 1);
	}

	if (NULL == poverBand)
	{
		return false;
	}
	dbPyramidWdith = poverBand->GetXSize();
	dbPyramidHeight = poverBand->GetYSize();
	return true;
}

int GeoGdalImageLayer::GetPyramidLevelCount() const
{
	GDALDataset *poDataset = (GDALDataset*)m_poDataset;
	if (NULL == poDataset)
	{
		return 0;
	}
	GDALRasterBand* pBand = poDataset->GetRasterBand(1);
	if (NULL == pBand)
	{
		return 0;
	}
	return pBand->GetOverviewCount();
}

bool GeoGdalImageLayer::GetBandMaxAndMin( int nBandIndex,double& dbMax,double& dbMin ) const
{
	GDALDataset *poDataset = (GDALDataset*)m_poDataset;
	if (NULL == poDataset)
	{
		return false;
	}

	if (nBandIndex < 0 || nBandIndex >= m_nBandCount)
	{
		return false;
	}
	GDALRasterBand* poBand = NULL;
	poBand = poDataset->GetRasterBand(nBandIndex+1);
	if (NULL == poBand)
	{
		return false;
	}

	int bGotMin = 0;
	int bGotMax = 0;
	dbMin = poBand->GetMinimum(&bGotMin);
	dbMax = poBand->GetMaximum(&bGotMax);
	if( ! (bGotMin&& bGotMax) )  
	{
		double adfMinMax[2];
		GDALComputeRasterMinMax((GDALRasterBandH)poBand,1, adfMinMax);  
		dbMin = adfMinMax[0];
		dbMax = adfMinMax[1];
	}
	return true;
}

bool GeoGdalImageLayer::GetStatistics(int nBandIndex,
									  double *pdfMin,
									  double *pdfMax,
									  double *pdfMean,
									  double *pdfStdDev,
									  RASProgressFunc pfnProgress,
									  void *pProgressData) const
{
	GDALDataset *poDataset = (GDALDataset*)m_poDataset;
	if (NULL == poDataset)
	{
		return false;
	}

	if (nBandIndex < 0 || nBandIndex >= m_nBandCount)
	{
		return false;
	}

	CPLErr eErr = CE_Failure;
	GDALRasterBand  *poBand = NULL;
	poBand = poDataset->GetRasterBand(nBandIndex+1);
	if (NULL == poBand)
	{
		return false;
	}
	eErr = poBand->GetStatistics(FALSE,TRUE,pdfMin,pdfMax,pdfMean,pdfStdDev);
	if (eErr != CE_None)
	{
		poBand->ComputeStatistics(FALSE,pdfMin,pdfMax,pdfMean,pdfStdDev,
			pfnProgress,pProgressData);
	}
}

#define TILE_SIZE 256

bool GeoGdalImageLayer::GetBandStatisInfoEx(
	int nChannelIndex, 
	float* pfHist, 
	float& fMax, 
	float& fMin, 
	float& fMean, 
	float& fVar, 
	float fExclude) const
{
	GDALDataset *poDataset = (GDALDataset*)m_poDataset;
	if (NULL == poDataset)
	{
		return false;
	}

	assert(NULL != poDataset);
	int nXsize = poDataset->GetRasterXSize();
	int nYsize = poDataset->GetRasterYSize();
	int pBandList[1] = {nChannelIndex+1};

	int nTileHeight = ( nYsize/float(nXsize) ) * TILE_SIZE;

	float *pBytes = new float[TILE_SIZE*nTileHeight];

	poDataset->RasterIO(GF_Read,0,0,nXsize,nYsize,pBytes,TILE_SIZE,nTileHeight,GDT_Float32,1,pBandList,0,0,0);

	std::vector<float> vecPixels;
	vecPixels.resize(TILE_SIZE*nTileHeight);

	int nCount = 0;
	double dbSum = 0;

	//开始统计
	fMax = DBL_MIN;
	fMin = DBL_MAX;
	for (int i = 0; i < TILE_SIZE*nTileHeight; i ++)
	{
		if (fabs( pBytes[i] - fExclude) <= 0.000001)
		{
			continue;
		}

		else
		{
			if (pBytes[i] <= fMin)
			{
				fMin = pBytes[i];
			}

			if (pBytes[i] >= fMax)
			{
				fMax = pBytes[i];
			}

			dbSum += pBytes[i];
			vecPixels[nCount] = pBytes[i];
			nCount ++;
		}
	}

	//计算均值
	fMean = dbSum/nCount;

	//计算标准差
	dbSum = 0;
	for (int i = 0; i < nCount; i ++)
	{
		dbSum += (vecPixels[i]-fMean)*(vecPixels[i]-fMean);
	}
	dbSum /= nCount;
	fVar = sqrt(dbSum);

	if (pBytes != NULL)
	{
		delete []pBytes;
		pBytes = NULL;
	}
	return 1;
}
