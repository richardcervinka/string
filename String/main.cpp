#include "String.h"

void main()
{
    char32_t s[] = U"z\u00df\u6c34\U0001f34c";

    std::string asciiFromUtf8 = String::Ascii::FromUtf8(u8"Kůň");
    std::string asciiFromUtf16 = String::Ascii::FromUtf16(u"Kůň");
    std::string asciiFromUtf32 = String::Ascii::FromUtf32(U"Kůň");

    std::string utf8FromAscii = String::Utf8::FromAscii("Kůň");
    std::string utf8FromUtf16 = String::Utf8::FromUtf16(u"Kůň");
    std::string utf8FromUtf32 = String::Utf8::FromUtf32(U"Kůň");

    std::u16string utf16FromAscii = String::Utf16::FromAscii("Kůň");
    std::u16string utf16FromUtf8 = String::Utf16::FromUtf8(u8"Kůň");
    std::u16string utf16FromUtf32 = String::Utf16::FromUtf32(U"Kůň");

    std::u32string utf32FromAscii = String::Utf32::FromAscii("Kůň");
    std::u32string utf32FromUtf8 = String::Utf32::FromUtf8(u8"Kůň");
    std::u32string utf32FromUtf16 = String::Utf32::FromUtf16(u"Kůň");

    return;
}

void PushUtf8(const char32_t ch, std::string& target)
{
}