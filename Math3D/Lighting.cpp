//
//#include "Vector3.h"
//#include "Lighting.h"
//#include "Light.h"
//#include "Material.h"
//
//Lighting::Lighting()
//{
//	mLightNum = 1;
//	m_pLights = new Light[mLightNum];
//	mAmbient = ColorRGBA(1.0,1.0,1.0);//环境光恒定不变
//}
//
//Lighting::~Lighting()
//{
//    DELETEARRAY(m_pLights);
//}
//void Lighting::SetLightNumber(int lnum)
//{
//    DELETEARRAY(m_pLights);
//	mLightNum = lnum;
//	m_pLights = new Light[lnum];
//}
//
//Lighting::Lighting(int lnum)
//{
//	mLightNum = lnum;
//	m_pLights = new Light[lnum];
//	mAmbient = ColorRGBA(0.3,0.3,0.3);	
//}
//
//ColorRGBA Lighting::ComputeLight(const Vector3& viewPoint,const Vector3& point,
//								 const Vector3& normal,Material *pMaterial)
//{
//	ColorRGBA LastC = pMaterial->M_Emit;//材质自身发散色为初始值
//	for(int i=0; i<mLightNum; i++)//来自光源
//	{
//		if(m_pLights[i].mOnOff)
//		{
//			ColorRGBA InitC(0,0,0);
//			Vector3 VL(point,m_pLights[i].mPosition);//指向光源的矢量
//			double d = VL.Length();//光传播的距离，等于矢量VL的模
//
//			VL.Normalize();
//			Vector3 VN = normal;
//			VN.Normalize();//单位化法矢量
//
//			//第1步，加入漫反射光
//			if(m_pLights[i].m_bDiffuse)
//			{
//				double CosTheta = std::max(VL.DotProduct(VN),0.0f);
//				InitC += m_pLights[i].mDiffuse.Multiply(pMaterial->M_Diffuse) * CosTheta;
//			}
//
//			//第2步，加入镜面反射光
//			if(m_pLights[i].m_bSpecular)
//			{
//				Vector3 VV(point,viewPoint);//VV为视矢量
//				VV.Normalize();//单位化视矢量
//				Vector3 VH = (VL + VV)/(VL+VV).Length();//平分矢量
//				double nHN = pow(std::max(VH.DotProduct(VN),0.0f),pMaterial->M_n);
//				InitC += m_pLights[i].mSpecular.Multiply(pMaterial->M_Specular) * nHN;	
//			}
//
//			//第3步，光强衰减
//			double c0 = m_pLights[i].L_C0;//c0为常数衰减因子
//			double c1 = m_pLights[i].L_C1;//c1线性衰减因子
//			double c2 = m_pLights[i].L_C2;//c2二次衰减因子
//			double f = (1.0/(c0+c1*d+c2*d*d));//光强衰减函数
//			f = std::min(1.0,f);
//			LastC += InitC * f;
//		}
//		else
//		{
//			//LastC += point.c;//物体自身颜色
//		}
//	}
//	//第4步，加入环境光
//	if(m_pLights[0].m_bAmbient)
//	{
//		LastC += mAmbient*pMaterial->M_Ambient;
//	}
//	//第5步，颜色归一化到[0,1]区间
//	//LastC.Normalize();
//	//第6步，返回所计算顶点的光强颜色
//	return LastC;
//}
