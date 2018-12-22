//
//  SwapPtr.c
//  BaseLib
//
//  Created by zhouxuguang on 16/10/9.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#include "SwapPtr.h"
void test()
{
	std::string *s2 = NULL;
	{
		std::string s1 = "周旭光ssssssssssssssssssssssssssssssssssssss";
		s2 = new std::string();
		*s2 = s1;

		char* str = strdup(s1.c_str());
	}

	delete s2;
	//assert(s2->c_str() == s1.c_str());

	int *p1 = new int();
	int *p2 = NULL;
	std::swap(p1,p2);
}

//template <class T = double>
//void Test(T obj)
//{
//    //
//}


