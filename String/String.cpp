#include "String.h"
#include <cctype>
#include <algorithm>

// Ascii

std::size_t String::Ascii::Length(const char* const ascii) noexcept
{
    return std::char_traits<char>::length(ascii);
}
    
std::size_t String::Ascii::Length(const std::string& ascii) noexcept
{
    return ascii.length();
}

inline static char AsciiFromUnicode(const char32_t ch) noexcept
{
    if (ch <= 127)
    {
        return static_cast<char>(ch);
    }
    return '?';
}

static std::string AsciiFromUtf8(const char* const utf8, const std::size_t length)
{
    std::string str;
    str.reserve(length);

    for (String::Utf8Iterator it = utf8; *it != U'\0'; it++)
    {
        str.push_back(AsciiFromUnicode(*it));
    }
    return str;
}

std::string String::Ascii::FromUtf8(const char* const utf8)
{
    if (utf8 == nullptr)
    {
        return std::string(u8"nullptr");
    }
    return AsciiFromUtf8(utf8, std::char_traits<char>::length(utf8));
}

std::string String::Ascii::FromUtf8(const std::string& utf8)
{
    return AsciiFromUtf8(utf8.data(), utf8.length());
}

static std::string AsciiFromUtf16(const char16_t* const utf16, const std::size_t length)
{
    std::string str;
    str.reserve(length);

    for (String::Utf16Iterator it = utf16; *it != U'\0'; it++)
    {
        str.push_back(AsciiFromUnicode(*it));
    }
    return str;
}

std::string String::Ascii::FromUtf16(const char16_t* const utf16)
{
    if (utf16 == nullptr)
    {
        return std::string(u8"nullptr");
    }
    return AsciiFromUtf16(utf16, std::char_traits<char16_t>::length(utf16));
}

std::string String::Ascii::FromUtf16(const std::u16string& utf16)
{
    return AsciiFromUtf16(utf16.data(), utf16.length());
}

static std::string AsciiFromUtf32(const char32_t* const utf32, const std::size_t length)
{
    std::string str;
    str.reserve(length);

    for (std::size_t i = 0; i < length; i++)
    {
        str.push_back(AsciiFromUnicode(utf32[i]));
    }
    return str;
}

std::string String::Ascii::FromUtf32(const char32_t* const utf32)
{
    if (utf32 == nullptr)
    {
        return std::string(u8"nullptr");
    }
    return AsciiFromUtf32(utf32, std::char_traits<char32_t>::length(utf32));
}

std::string String::Ascii::FromUtf32(const std::u32string& utf32)
{
    return AsciiFromUtf32(utf32.data(), utf32.length());
}

// Utf8

std::size_t String::Utf8::Length(const char* const utf8) noexcept
{
    std::size_t length = 0;
    for (Utf8Iterator it = utf8; *it != U'\0'; it++)
    {
        length++;
    }
    return length;
}

std::size_t String::Utf8::Length(const std::string& utf8) noexcept
{
    return Length(utf8.data());
}

static void PushUtf8(const char32_t ch, std::string& target)
{
    // 1 byte
    if (ch <= 0x7f)
    {
        target.push_back(static_cast<char>(ch));
        return;
    }
    // 2 byte
    if (ch <= 0x7ff)
    {
        target.push_back(static_cast<char>(0xc0 | ((ch >> 6) & 0x1f)));
        target.push_back(static_cast<char>(0x80 | ((ch >> 0) & 0x3f)));
        return;
    }
    // 3 byte
    if (ch <= 0xffff)
    {
        target.push_back(static_cast<char>(0xe0 | ((ch >> 12) & 0x0f)));
        target.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3f)));
        target.push_back(static_cast<char>(0x80 | ((ch >> 0) & 0x3f)));
        return;
    }
    // 4 byte
    if (ch <= 0x1fffff)
    {
        target.push_back(static_cast<char>(0xf0u | ((ch >> 18) & 0x07u)));
        target.push_back(static_cast<char>(0x80u | ((ch >> 12) & 0x3fu)));
        target.push_back(static_cast<char>(0x80u | ((ch >> 6) & 0x3fu)));
        target.push_back(static_cast<char>(0x80u | ((ch >> 0) & 0x3fu)));
        return;
    }
}

static std::string Utf8FromAscii(const char* const ascii, const std::size_t length)
{
    std::string str;
    str.reserve(length);

    for (std::size_t i = 0; i < length; i++)
    {
        if (static_cast<unsigned char>(ascii[i]) <= 127)
        {
            str.push_back(ascii[i]);
        }
        else
        {
            str.push_back(u8'?');
        }
    }
    return str;
}

std::string String::Utf8::FromAscii(const char* const ascii)
{
    if (ascii == nullptr)
    {
        return std::string(u8"nullptr");
    }
    return Utf8FromAscii(ascii, std::char_traits<char>::length(ascii));
}

std::string String::Utf8::FromAscii(const std::string& ascii)
{
    return Utf8FromAscii(ascii.data(), ascii.length());
}

static std::string Utf8FromUtf16(const char16_t* const utf16, const std::size_t length)
{
    std::string str;
    str.reserve(length);

    for (String::Utf16Iterator it = utf16; *it != U'\0'; it++)
    {
        PushUtf8(*it, str);
    }
    return str;
}

std::string String::Utf8::FromUtf16(const char16_t* const utf16)
{
    if (utf16 == nullptr)
    {
        return std::string(u8"nullptr");
    }
    return Utf8FromUtf16(utf16, std::char_traits<char16_t>::length(utf16));
}

std::string String::Utf8::FromUtf16(const std::u16string& utf16)
{
    return Utf8FromUtf16(utf16.data(), utf16.length());
}

static std::string Utf8FromUtf32(const char32_t* const utf32, const std::size_t length)
{
    std::string str;
    str.reserve(length);

    for (std::size_t i = 0; i < length; i++)
    {
        PushUtf8(utf32[i], str);
    }
    return str;
}

std::string String::Utf8::FromUtf32(const char32_t* const utf32)
{
    if (utf32 == nullptr)
    {
        return std::string(u8"nullptr");
    }
    return Utf8FromUtf32(utf32, std::char_traits<char32_t>::length(utf32));
}

std::string String::Utf8::FromUtf32(const std::u32string& utf32)
{
    return Utf8FromUtf32(utf32.data(), utf32.length());
}

// Utf16

std::size_t String::Utf16::Length(const char16_t* const utf16) noexcept
{
    std::size_t length = 0;
    for (Utf16Iterator it = utf16; *it != U'\0'; it++)
    {
        length++;
    }
    return length;
}

std::size_t String::Utf16::Length(const std::u16string& utf16) noexcept
{
    return Length(utf16.data());
}

static void PushUtf16(const char32_t ch, std::u16string& target)
{
    // Unicode define no characters in range 0xd800 - 0xdfff.
    if (ch >= 0xd800 && ch <= 0xdfff)
    {
        return;
    }
    // Surrogate pairs.
    if (ch > 0xffff)
    {
        target.push_back(0xd800 | (((ch - 0x010000) >> 10) & 0x3ff));
        target.push_back(0xdc00 | (((ch - 0x010000) >> 0) & 0x3ff));
        return;
    }
    // One character.
    target.push_back(static_cast<char16_t>(ch));
}

static std::u16string Utf16FromAscii(const char* const ascii, const std::size_t length)
{
    std::u16string str;
    str.reserve(length);

    for (std::size_t i = 0; i < length; i++)
    {
        if (static_cast<unsigned char>(ascii[i]) <= 127)
        {
            str.push_back(ascii[i]);
        }
        else
        {
            str.push_back(u'?');
        }
    }
    return str;
}

std::u16string String::Utf16::FromAscii(const char* const ascii)
{
    if (ascii == nullptr)
    {
        return std::u16string(u"nullptr");
    }
    return Utf16FromAscii(ascii, std::char_traits<char>::length(ascii));
}

std::u16string String::Utf16::FromAscii(const std::string& ascii)
{
    return Utf16FromAscii(ascii.data(), ascii.length());
}

static std::u16string Utf16FromUtf8(const char* const utf8, const std::size_t length)
{
    std::u16string str;
    str.reserve(length);

    for (String::Utf8Iterator it = utf8; *it != U'\0'; it++)
    {
        PushUtf16(*it, str);
    }
    return str;
}

std::u16string String::Utf16::FromUtf8(const char* const utf8)
{
    if (utf8 == nullptr)
    {
        return std::u16string(u"nullptr");
    }
    return Utf16FromUtf8(utf8, std::char_traits<char>::length(utf8));
}

std::u16string String::Utf16::FromUtf8(const std::string& utf8)
{
    return Utf16FromUtf8(utf8.data(), utf8.length());
}

static std::u16string Utf16FromUtf32(const char32_t* const utf32, const std::size_t length)
{
    std::u16string str;
    str.reserve(length);

    for (std::size_t i = 0; i < length; i++)
    {
        PushUtf16(utf32[i], str);
    }
    return str;
}

std::u16string String::Utf16::FromUtf32(const char32_t* const utf32)
{
    if (utf32 == nullptr)
    {
        return std::u16string(u"nullptr");
    }
    return Utf16FromUtf32(utf32, std::char_traits<char32_t>::length(utf32));
}

std::u16string String::Utf16::FromUtf32(const std::u32string& utf32)
{
    return Utf16FromUtf32(utf32.data(), utf32.length());
}

// Utf32

std::size_t String::Utf32::Length(const char32_t* const utf32) noexcept
{
    return std::char_traits<char32_t>::length(utf32);
}

std::size_t String::Utf32::Length(const std::u32string& utf32) noexcept
{
    return Length(utf32.data());
}

static std::u32string Utf32FromAscii(const char* const ascii, const std::size_t length)
{
    std::u32string str;
    str.reserve(length);

    for (std::size_t i = 0; i < length; i++)
    {
        if (static_cast<unsigned char>(ascii[i]) <= 127)
        {
            str.push_back(ascii[i]);
        }
        else
        {
            str.push_back(U'?');
        }
    }
    return str;
}

std::u32string String::Utf32::FromAscii(const char* const ascii)
{
    if (ascii == nullptr)
    {
        return std::u32string(U"nullptr");
    }
    return Utf32FromAscii(ascii, std::char_traits<char>::length(ascii));
}

std::u32string String::Utf32::FromAscii(const std::string& ascii)
{
    return Utf32FromAscii(ascii.data(), ascii.length());
}

static std::u32string Utf32FromUtf8(const char* const utf8, const std::size_t length)
{
    std::u32string str;
    str.reserve(length);

    for (String::Utf8Iterator it = utf8; *it != U'\0'; it++)
    {
        str.push_back(*it);
    }
    return str;
}

std::u32string String::Utf32::FromUtf8(const char* const utf8)
{
    if (utf8 == nullptr)
    {
        return std::u32string(U"nullptr");
    }
    return Utf32FromUtf8(utf8, std::char_traits<char>::length(utf8));
}

std::u32string String::Utf32::FromUtf8(const std::string& utf8)
{
    return Utf32FromUtf8(utf8.data(), utf8.length());
}

static std::u32string Utf32FromUtf16(const char16_t* const utf16, const std::size_t length)
{
    std::u32string str;
    str.reserve(length);

    for (String::Utf16Iterator it = utf16; *it != U'\0'; it++)
    {
        str.push_back(*it);
    }
    return str;
}

std::u32string String::Utf32::FromUtf16(const char16_t* const utf16)
{
    if (utf16 == nullptr)
    {
        return std::u32string(U"nullptr");
    }
    return Utf32FromUtf16(utf16, std::char_traits<char16_t>::length(utf16));
}

std::u32string String::Utf32::FromUtf16(const std::u16string& utf16)
{
    return Utf32FromUtf16(utf16.data(), utf16.length());
}

// Utf8 iterator

String::Utf8Iterator::Utf8Iterator(const char* const utf8Str)
{
    ptr = utf8Str;
    Decode();
}

String::Utf8Iterator::Utf8Iterator(const std::string& utf8Str)
{
    ptr = utf8Str.c_str();
    Decode();
}

void String::Utf8Iterator::Decode() noexcept
{
    if (ptr == nullptr)
    {
        code = 0;
        units = 0;
        return;
    }

    const unsigned char* byte = reinterpret_cast<const unsigned char*>(ptr);

    // 1 byte character 0xxxxxxx
    if (*byte < static_cast<unsigned char>(0xc0))
    {
        code = static_cast<char32_t>(*byte);
        units = 1;
        return;
    }

    // 2 byte character 110xxxxx 10xxxxxx
    if (*byte < static_cast<unsigned char>(0xe0))
    {
        code = ((static_cast<char32_t>(*(byte + 0)) & 0x01fu) << 6) |
               ((static_cast<char32_t>(*(byte + 1)) & 0x03fu));
        units = 2;
        return;
    }

    // 3 byte character 1110xxxx 10xxxxxx 10xxxxxx
    if (*byte < static_cast<unsigned char>(0xf0))
    {
        code = ((static_cast<char32_t>(*(byte + 0)) & 0x000f) << 12) |
               ((static_cast<char32_t>(*(byte + 1)) & 0x003f) << 6) |
               ((static_cast<char32_t>(*(byte + 2)) & 0x003f));
        units = 3;
        return;
    }

    // 4 byte character 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    if (*byte < static_cast<unsigned char>(0xf8))
    {
        code = ((static_cast<char32_t>(*(byte + 0)) & 0x0007) << 18) |
               ((static_cast<char32_t>(*(byte + 1)) & 0x003f) << 12) |
               ((static_cast<char32_t>(*(byte + 2)) & 0x003f) << 6) |
               ((static_cast<char32_t>(*(byte + 3)) & 0x003f));
        units = 4;
        return;
    }

    // 5 byte character 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    if (*byte < static_cast<unsigned char>(0xfC))
    {
        code = ((static_cast<char32_t>(*(byte + 0)) & 0x0003) << 24) |
               ((static_cast<char32_t>(*(byte + 1)) & 0x003f) << 18) |
               ((static_cast<char32_t>(*(byte + 2)) & 0x003f) << 12) |
               ((static_cast<char32_t>(*(byte + 3)) & 0x003f) << 6) |
               ((static_cast<char32_t>(*(byte + 4)) & 0x003f));
        units = 5;
        return;
    }

    // 6 byte character 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    code = ((static_cast<char32_t>(*(byte + 0)) & 0x0001) << 30) |
           ((static_cast<char32_t>(*(byte + 1)) & 0x003f) << 24) |
           ((static_cast<char32_t>(*(byte + 2)) & 0x003f) << 18) |
           ((static_cast<char32_t>(*(byte + 3)) & 0x003f) << 12) |
           ((static_cast<char32_t>(*(byte + 4)) & 0x003f) << 6) |
           ((static_cast<char32_t>(*(byte + 5)) & 0x003f));
    units = 6;
}

const char32_t String::Utf8Iterator::operator*() const noexcept
{
    return code;
}

const char32_t* String::Utf8Iterator::operator->() const noexcept
{
    return &code;
}

String::Utf8Iterator& String::Utf8Iterator::operator++() noexcept
{
    if (ptr == nullptr)
    {
        return *this;
    }
    ptr = ptr + units;
    Decode();
    return *this;
}

String::Utf8Iterator String::Utf8Iterator::operator++(int) noexcept
{
    if (ptr == nullptr)
    {
        return *this;
    }
    auto old = *this;
    ptr = ptr + units;
    Decode();
    return old;
}

bool String::Utf8Iterator::operator!=(const String::Utf8Iterator& it) const noexcept
{
    return this->ptr != it.ptr;
}

bool String::Utf8Iterator::operator==(const String::Utf8Iterator& it) const noexcept
{
    return this->ptr == it.ptr;
}

bool String::Utf8Iterator::operator==(const std::nullptr_t) const noexcept
{
    return ptr == nullptr;
}

bool String::Utf8Iterator::operator!=(const std::nullptr_t) const noexcept
{
    return ptr != nullptr;
}

// Utf16 iterator

String::Utf16Iterator::Utf16Iterator(const char16_t* const utf16)
{
    ptr = utf16;
    Decode();
}

String::Utf16Iterator::Utf16Iterator(const std::u16string& utf16)
{
    ptr = utf16.c_str();
    Decode();
}

void String::Utf16Iterator::Decode() noexcept
{
    // Null.
    if (ptr == nullptr)
    {
        code = 0;
        units = 0;
        return;
    }
    
    // Surrogate pairs.
    if (*ptr >= 0xd800 && *ptr <= 0xdfff)
    {
        code = ((*ptr & 0x3ff) << 10) | (*(ptr + 1) & 0x3ff);
        code += 0x010000;
        units = 2;
        return;
    }

    // One char16 character.
    code = *ptr;
    units = 1;
}

const char32_t String::Utf16Iterator::operator*() const noexcept
{
    return code;
}

const char32_t* String::Utf16Iterator::operator->() const noexcept
{
    return &code;
}

String::Utf16Iterator& String::Utf16Iterator::operator++() noexcept
{
    if (ptr == nullptr)
    {
        return *this;
    }
    ptr += units;
    Decode();
    return *this;
}

String::Utf16Iterator String::Utf16Iterator::operator++(int) noexcept
{
    if (ptr == nullptr)
    {
        return *this;
    }
    auto old = *this;
    ptr += units;
    Decode();
    return old;
}

bool String::Utf16Iterator::operator!=(const String::Utf16Iterator& it) const noexcept
{
    return this->ptr != it.ptr;
}

bool String::Utf16Iterator::operator==(const String::Utf16Iterator& it) const noexcept
{
    return this->ptr == it.ptr;
}

bool String::Utf16Iterator::operator==(const std::nullptr_t) const noexcept
{
    return ptr == nullptr;
}

bool String::Utf16Iterator::operator!=(const std::nullptr_t) const noexcept
{
    return ptr != nullptr;
}