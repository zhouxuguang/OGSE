//
//  StringConverter.h 字符串转换的类
//  BaseLib
//
//  Created by zhouxuguang on 16/6/18.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef BASELIB_STRINGCONVERTER_INCLUDE_H
#define BASELIB_STRINGCONVERTER_INCLUDE_H

#include "PreCompile.h"

NS_BASELIB_BEGIN

class BASELIB_API StringConverter
{
public:
    /**
     *  宽字符串转换为ANSI字符串
     *  @param wStr 宽字符串
     *  @param str  ANSI字符串
     */
    static void WideStringToAnsiString(const std::wstring& wStr,std::string& str);
    
    /**
     *  ANSI字符串转换为宽字符串
     *  @param str  ANSI字符串
     *  @param wStr 宽字符串
     */
    static void AnsiStringToWideString(const std::string& str,std::wstring& wStr);
    
    /**
     *  utf8->utf16
     *  @param utf8     utf8
     *  @param outUtf16 utf16
     *  @return 成功返回1
     */
    static bool UTF8ToUTF16(const std::string& utf8, utf16String& outUtf16);
    
    /**
     *  ut8->utf32
     *  @param utf8     utf8
     *  @param outUtf32 utf32
     *  @return 成功返回1
     */
    static bool UTF8ToUTF32(const std::string& utf8, utf32String& outUtf32);
    
    /**
     *  utf16->utf8
     *  @param utf16   utf16
     *  @param outUtf8 utf8
     *  @return 成功返回1
     */
    static bool UTF16ToUTF8(const utf16String& utf16, std::string& outUtf8);
    
    /**
     *  utf16->utf32
     *  @param utf16    utf16
     *  @param outUtf32 utf32
     *  @return 成功返回1
     */
    static bool UTF16ToUTF32(const utf16String& utf16, utf32String& outUtf32);
    
    /**
     *  utf32->utf8
     *  @param utf32   utf32
     *  @param outUtf8 utf8
     *  @return 成功返回1
     */
    static bool UTF32ToUTF8(const utf32String& utf32, std::string& outUtf8);
    
    /**
     *  utf32->utf16
     *  @param utf32    utf32
     *  @param outUtf16 utf16
     *  @return 成功返回1
     */
    static bool UTF32ToUTF16(const utf32String& utf32, utf16String& outUtf16);
    
    /**
     *  utf8->gb2312
     *  @param [in] utf8
     *  @param [out] gb2312
     *  @return 成功返回1
     */
    static bool UTF8ToGB2312(const std::string& utf8,std::string& gb2312);
    
    /**
     *  gb2312 -> utf8
     *  @param [in] utf8
     *  @param [out] gb2312
     *  @return 成功返回1
     */
    static bool GB2312ToUTF8(const std::string& gb2312,std::string& utf8);
    
private:
    StringConverter();
    StringConverter(const StringConverter& );
    StringConverter& operator=(const StringConverter& );
    ~StringConverter();
};

NS_BASELIB_END

#endif /* BASELIB_STRINGCONVERTER_INCLUDE_H */
