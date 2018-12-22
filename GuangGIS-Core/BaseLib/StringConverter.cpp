//
//  StringConverter.cpp
//  BaseLib
//
//  Created by zhouxuguang on 16/6/18.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#include "StringConverter.h"
#include "ConvertUTF.h"

#if defined __APPLE__ //|| (defined(__ANDROID__) && !defined(__LP64__))

#define USE_ICONV

#include <iconv.h>

static int utf8togb2312(const char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen)
{
    
    iconv_t cd = 0;
    if( (cd = iconv_open("gb2312","utf-8")) ==0 )
        return -1;
    
    memset(destbuf,0,destlen);
    const char **source = &sourcebuf;
    
    char **dest = &destbuf;
    if(-1 == iconv(cd,(char**)source,&sourcelen,dest,&destlen))
        return -1;   
    
    iconv_close(cd);   
    
    return 0;   
    
}

static int Gb2312toUtf8(const char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen)
{
    
    iconv_t cd = 0;
    if( (cd = iconv_open("utf-8","gb2312")) ==0 )
        return -1;
    
    memset(destbuf,0,destlen);
    const char **source = &sourcebuf;
    
    char **dest = &destbuf;
    if(-1 == iconv(cd,(char**)source,&sourcelen,dest,&destlen))
        return -1;
    iconv_close(cd);   
    
    return 0;
}

#elif defined WIN32

#include "unicodeTables.h"
#include "gbkTables.h"

void unicodeToGB2312(const std::wstring& wstr , std::string& result)
{
	int n = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), -1, 0, 0, 0, 0 );
	result.resize(n);
	::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, (char*)result.c_str(), n, 0, 0 );
}

void utf8ToUnicode(const std::string& src, std::wstring& result)
{
	int n = MultiByteToWideChar( CP_UTF8, 0, src.c_str(), -1, NULL, 0 );
	result.resize(n);
	::MultiByteToWideChar( CP_UTF8, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
}

void gb2312ToUnicode(const std::string& src, std::wstring& result)
{
	int n = MultiByteToWideChar( CP_ACP, 0, src.c_str(), -1, NULL, 0 );
	result.resize(n);
	::MultiByteToWideChar( CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
}

void unicodeToUTF8(const std::wstring &src, std::string& result)
{
	int n = WideCharToMultiByte( CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0 );
	result.resize(n);
	::WideCharToMultiByte( CP_UTF8, 0, src.c_str(), -1, (char*)result.c_str(), result.length(), 0, 0 );
}


//查表

//gbk->unicode
static int GbkToUnicode(char *gbk_buf, unsigned short *unicode_buf, int max_unicode_buf_size)
{
	unsigned short word;
	unsigned char *gbk_ptr =(unsigned char *) gbk_buf;
	unsigned short *uni_ptr = unicode_buf;
	unsigned int uni_ind = 0, gbk_ind = 0, uni_num = 0;
	unsigned char ch;
	int word_pos;

	if( !gbk_buf || !unicode_buf )
		return -1;

	while(1)
	{
		ch = *(gbk_ptr + gbk_ind);

		if(ch == 0x00)
			break;

		if( ch > 0x80 )
		{
			//将gbk编码的中文字符的两个字节组合成一个    unsigned short word;
			word = *(gbk_ptr + gbk_ind);
			word <<= 8;
			word += *(gbk_ptr + gbk_ind+1);
			gbk_ind += 2;

			word_pos = word - gbk_first_code;
			if(word >= gbk_first_code && word <= gbk_last_code  && (word_pos < unicode_buf_size))
			{
				*(uni_ptr + uni_ind) = unicodeTables[word_pos];
				uni_ind++;
				uni_num++;
			}
		}
		else
		{
			gbk_ind++;
			*(uni_ptr + uni_ind) = ch;
			uni_ind++;
			uni_num++;
		}

		if(uni_num > max_unicode_buf_size - 1)
			break;
	}

	return uni_num;
}

//unicode->gbk
static int UnicodeToGbk(unsigned short *unicode_buf, char *gbk_buf, int max_gbk_buf_size)
{
	unsigned short word;
	unsigned short gbk_word;
	unsigned char ch;
	unsigned char *gbk_ptr =(unsigned char *) gbk_buf;
	unsigned short *uni_ptr = unicode_buf;
	unsigned int uni_ind = 0, gbk_ind = 0, gbk_num = 0;
	int word_pos;

	if( !gbk_buf || !unicode_buf )
		return -1;

	while(1)
	{
		word = *(uni_ptr + uni_ind);
		uni_ind++;

		if(word == 0x0000)  //字符串结束符
			break;

		if(word < 0x80)  /*ASCII不用查表*/
		{
			*(gbk_ptr + gbk_ind) = (unsigned char)word;
			gbk_ind++;
		}
		else
		{
			word_pos = word - unicode_first_code;
			if(word >= unicode_first_code && word <= unicode_last_code && word_pos < gbk_buf_size)
			{
				gbk_word = gbkTables[word_pos];//gbk_word是gbk编码，但是为unsigned short类型，需要拆分成两个字节

				*(gbk_ptr + gbk_ind) = (unsigned char)(gbk_word >> 8);//提取高8位
				gbk_ind++;
				*(gbk_ptr + gbk_ind) = (unsigned char)(gbk_word >> 0);//提取低8位
				gbk_ind++;
				gbk_num +=2;//gbk字符加2个
			}
		}

		if(gbk_num > max_gbk_buf_size - 1)
			break;
	}

	return gbk_num;
}

#endif

NS_BASELIB_BEGIN

void StringConverter::WideStringToAnsiString(const std::wstring& src,std::string& dest)
{
#ifndef WIN32
    dest.clear();
    
    size_t nSize = src.size();
    for (size_t i = 0; i < nSize; i++)
    {
        wchar_t w = src[i];
        if (w <= 0x7f)
            dest.push_back((char)w);
        else if (w <= 0x7ff){
            dest.push_back(0xc0 | ((w >> 6)& 0x1f));
            dest.push_back(0x80| (w & 0x3f));
        }
        else if (w <= 0xffff){
            dest.push_back(0xe0 | ((w >> 12)& 0x0f));
            dest.push_back(0x80| ((w >> 6) & 0x3f));
            dest.push_back(0x80| (w & 0x3f));
        }
        else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
            dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
            dest.push_back(0x80| ((w >> 12) & 0x3f));
            dest.push_back(0x80| ((w >> 6) & 0x3f));
            dest.push_back(0x80| (w & 0x3f));
        }
        else
            dest.push_back('?');
    }
#else
	size_t len = src.size();
	if(len<=0)
		dest = ""; 

	int nChars = WideCharToMultiByte( CP_ACP , 0 , 
		src.c_str() , len , NULL , 0 , NULL , NULL ) ; 
	if ( len == -1 )
		-- nChars ; 
	if ( nChars == 0 )
		dest = "";

	// convert the wide string to a narrow string
	// nb: slightly naughty to write directly into the string like this
	dest.resize(nChars);
	WideCharToMultiByte(CP_ACP,0,src.c_str(),len,const_cast<char*>(dest.c_str()),nChars,NULL,NULL);  
#endif
}

std::wstring BASELIB_API_HIDE FromUtf8(const std::string& utf8string)
{
	size_t widesize = utf8string.length();
	if (sizeof(wchar_t) == 2)
	{
		wchar_t* widestringnative = new wchar_t[widesize+1];
		const UTF8* sourcestart = reinterpret_cast<const UTF8*>(utf8string.c_str());
		const UTF8* sourceend = sourcestart + widesize;
		UTF16* targetstart = reinterpret_cast<UTF16*>(widestringnative);
		UTF16* targetend = targetstart + widesize+1;
		ConversionResult res = ConvertUTF8toUTF16
			(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
		if (res != conversionOK)
		{
			delete [] widestringnative;
			return L"";
		}
		*targetstart = 0;
		std::wstring resultstring(widestringnative);
		delete [] widestringnative;
		return resultstring;
	}
	else if (sizeof(wchar_t) == 4)
	{
		wchar_t* widestringnative = new wchar_t[widesize];
		const UTF8* sourcestart = reinterpret_cast<const UTF8*>(utf8string.c_str());
		const UTF8* sourceend = sourcestart + widesize;
		UTF32* targetstart = reinterpret_cast<UTF32*>(widestringnative);
		UTF32* targetend = targetstart + widesize;
		ConversionResult res = ConvertUTF8toUTF32
			(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
		if (res != conversionOK)
		{
			delete [] widestringnative;
			return L"";
		}
		*targetstart = 0;
		std::wstring resultstring(widestringnative);
		delete [] widestringnative;
		return resultstring;
	}
	else
	{
		 return L"";
	}
	return L"";
}


void StringConverter::AnsiStringToWideString(const std::string& str,std::wstring& wStr)
{
#ifndef WIN32 
	wStr = FromUtf8(str);
#else  //win32
	size_t len = str.size();
	if(len<=0)
		wStr = L"";

	// figure out how many wide characters we are going to get 
	int nChars = ::MultiByteToWideChar( CP_ACP , 0 , str.c_str() , len , NULL , 0 ) ; 
	if ( len == -1 )
		-- nChars ; 
	if ( nChars == 0 )
		wStr = L"";

	// convert the narrow string to a wide string 
	// nb: slightly naughty to write directly into the string like this
	wStr.resize(nChars); 
	::MultiByteToWideChar(CP_ACP,0,str.c_str(),len,const_cast<wchar_t*>(wStr.c_str()),nChars); 
#endif 
}

//utf8,utf16,utf32之间的转换
//Trait  待研究
template <typename T>
struct ConvertTrait {
    typedef T ArgType;
};
template <>
struct ConvertTrait<char> {
    typedef UTF8 ArgType;
};
template <>
struct ConvertTrait<eUInt16> {
    typedef UTF16 ArgType;
};
template <>
struct ConvertTrait<eUInt32> {
    typedef UTF32 ArgType;
};

//c++ 98在模板函数中不支持默认模板参数
template <typename From, typename To, typename FromTrait, typename ToTrait >
static bool utfConvert(
                const std::basic_string<From>& from, std::basic_string<To>& to,
                ConversionResult(*cvtfunc)(const FromTrait**, const FromTrait*,
                                           ToTrait**, ToTrait*,
                                           ConversionFlags)
                )
{
    assert(sizeof(From) == sizeof(FromTrait));
    assert(sizeof(To) == sizeof(ToTrait));
    
    if (from.empty())
    {
        to.clear();
        return true;
    }
    
    // See: http://unicode.org/faq/utf_bom.html#gen6
    const int most_bytes_per_character = 4;
    
    const size_t maxNumberOfChars = from.length(); // all UTFs at most one element represents one character.
    const size_t numberOfOut = maxNumberOfChars * most_bytes_per_character / sizeof(To);
    
    std::basic_string<To> working(numberOfOut, 0);

    const FromTrait* inbeg = reinterpret_cast<const FromTrait*>(&from[0]);
    const FromTrait* inend = inbeg + from.length();
    
    
    ToTrait* outbeg = reinterpret_cast<ToTrait*>(&working[0]);
    ToTrait* outend = outbeg + working.length();
    ConversionResult result = cvtfunc(&inbeg, inend, &outbeg, outend, strictConversion);
    if (result != conversionOK)
        return false;
    
    working.resize(reinterpret_cast<To*>(outbeg) - (To*)&working[0]);
    //to = std::move(working);
    to = working;
    
    return true;
};

bool StringConverter::UTF8ToUTF16(const std::string& utf8, utf16String& outUtf16)
{
    return utfConvert<char,eUInt16,UTF8,UTF16>(utf8, outUtf16, ConvertUTF8toUTF16);
}

bool StringConverter::UTF8ToUTF32(const std::string& utf8, utf32String& outUtf32)
{
    return utfConvert<char,eUInt32,UTF8,UTF32>(utf8, outUtf32, ConvertUTF8toUTF32);
}

bool StringConverter::UTF16ToUTF8(const utf16String& utf16, std::string& outUtf8)
{
    return utfConvert<eUInt16,char,UTF16,UTF8>(utf16, outUtf8, ConvertUTF16toUTF8);
}

bool StringConverter::UTF16ToUTF32(const utf16String& utf16, utf32String& outUtf32)
{
    return utfConvert<eUInt16,eUInt32,UTF16,UTF32>(utf16, outUtf32, ConvertUTF16toUTF32);
}

bool StringConverter::UTF32ToUTF8(const utf32String& utf32, std::string& outUtf8)
{
    return utfConvert<eUInt32,char,UTF32,UTF8>(utf32, outUtf8, ConvertUTF32toUTF8);
}

bool StringConverter::UTF32ToUTF16(const utf32String& utf32, utf16String& outUtf16)
{
    return utfConvert<eUInt32,eUInt16,UTF32,UTF16>(utf32, outUtf16, ConvertUTF32toUTF16);
}

bool StringConverter::UTF8ToGB2312(const std::string& utf8,std::string& gb2312)
{
    if (utf8.empty())
    {
        return false;
    }
    
#ifdef WIN32
	std::wstring strUnicode;
	baselib::StringConverter::AnsiStringToWideString(utf8,strUnicode);

	gb2312.resize(strUnicode.size()*2 +2);
	UnicodeToGbk((unsigned short*)strUnicode.c_str(),(char*)gb2312.c_str(),strUnicode.size()*2 +2);

	return true;
#elif defined USE_ICONV

    gb2312.resize(utf8.size()*2+2);
    int nRet = utf8togb2312(utf8.c_str(),utf8.size(),(char*)gb2312.c_str(),gb2312.size());
    
    if (nRet == -1)
    {
        return 0;
    }
    return 1;

#endif

    return true;
}

bool StringConverter::GB2312ToUTF8(const std::string& gb2312,std::string& utf8)
{
    if (gb2312.empty())
    {
        return false;
    }
    
#ifdef WIN32
    
	std::wstring wStr;
	
	size_t nLen = gb2312.size() * 2 + 2;
	wStr.resize(nLen);
	GbkToUnicode((char*)gb2312.c_str(),(unsigned short*)wStr.c_str(),nLen);
	baselib::StringConverter::WideStringToAnsiString(wStr,utf8);

	return true;
#elif defined USE_ICONV

    utf8.resize(gb2312.size() + 2);
    int nRet = Gb2312toUtf8(gb2312.c_str(), gb2312.size(), (char*)utf8.c_str(), utf8.size());
    
    const char* pszResult = utf8.c_str();
    utf8 = pszResult;
    
    if (nRet == -1)
    {
        return 0;
    }
    return 1;
#endif

    return true;
}


NS_BASELIB_END
