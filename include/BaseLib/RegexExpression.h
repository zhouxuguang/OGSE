
//
//  RegexExpression
//  BaseLib
//
//  Created by Zhou,Xuguang on 17/5/3.
//  Copyright ?2017�� Zhou,Xuguang. All rights reserved.
//

#ifndef BASELIB_REGEXEXPRESSION_INCLUDE_FGDDSGGD8DG
#define BASELIB_REGEXEXPRESSION_INCLUDE_FGDDSGGD8DG

#include "PreCompile.h"

NS_BASELIB_BEGIN

/**��ƽ̨������ʽ������
***/
class BASELIB_API RegexExpression
{
public:
	RegexExpression(void);
	~RegexExpression(void);

	bool RegexParser(const std::string&srcStr, const std::string& pattern,std::vector<std::string>& vecMatchPos);
};

NS_BASELIB_END

#endif

