#pragma once

#include <string>

namespace String
{
    // Ascii encoding.
    namespace Ascii
    {
        // Encoding conversion.
        std::string FromUtf8(const char* const utf8);
        std::string FromUtf8(const std::string& utf8);
        std::string FromUtf16(const char16_t* const utf16);
        std::string FromUtf16(const std::u16string& utf16);
        std::string FromUtf32(const char32_t* const utf32);
        std::string FromUtf32(const std::u32string& utf32);

        // Number of characters.
        std::size_t Length(const char* const ascii) noexcept;
        std::size_t Length(const std::string& ascii) noexcept;
    }

    // Unicode UTF8 encoding.
    namespace Utf8
    {
        // Encoding conversion.
        std::string FromAscii(const char* const ascii);
        std::string FromAscii(const std::string& ascii);
        std::string FromUtf16(const char16_t* const utf16);
        std::string FromUtf16(const std::u16string& utf16);
        std::string FromUtf32(const char32_t* const utf32);
        std::string FromUtf32(const std::u32string& utf32);

        // Number of characters.
        std::size_t Length(const char* const utf8) noexcept;
        std::size_t Length(const std::string& utf8) noexcept;
    }

    // Unicode UTF16 encoding.
    namespace Utf16
    {
        // Encoding conversion.
        std::u16string FromAscii(const char* const ascii);
        std::u16string FromAscii(const std::string& ascii);
        std::u16string FromUtf8(const char* const utf8);
        std::u16string FromUtf8(const std::string& utf8);
        std::u16string FromUtf32(const char32_t* const utf32);
        std::u16string FromUtf32(const std::u32string& utf32);

        // Number of characters.
        std::size_t Length(const char16_t* const utf16) noexcept;
        std::size_t Length(const std::u16string& utf16) noexcept;
    }

    // Unicode UTF32 encoding.
    namespace Utf32
    {
        // Encoding conversion.
        std::u32string FromAscii(const char* const ascii);
        std::u32string FromAscii(const std::string& ascii);
        std::u32string FromUtf8(const char* const utf8);
        std::u32string FromUtf8(const std::string& utf8);
        std::u32string FromUtf16(const char16_t* const utf16);
        std::u32string FromUtf16(const std::u16string& utf16);

        // Number of characters.
        std::size_t Length(const char32_t* const utf32) noexcept;
        std::size_t Length(const std::u32string& utf32) noexcept;
    }

    // Utf8 forward iterator.
    class Utf8Iterator : public std::iterator<std::forward_iterator_tag, char32_t>
    {
    public:
        Utf8Iterator() = default;
        Utf8Iterator(const char* const utf8);
        Utf8Iterator(const std::string& utf8);
        Utf8Iterator(const Utf8Iterator&) = default;
        ~Utf8Iterator() = default;
        Utf8Iterator& operator=(const Utf8Iterator&) = default;
        const char32_t operator*() const noexcept;
        const char32_t* operator->() const noexcept;
        Utf8Iterator& operator++() noexcept;
        Utf8Iterator operator++(int) noexcept;
        bool operator==(const Utf8Iterator& it) const noexcept;
        bool operator!=(const Utf8Iterator& it) const noexcept;
        bool operator==(const std::nullptr_t) const noexcept;
        bool operator!=(const std::nullptr_t) const noexcept;

    private:
        void Decode() noexcept;

    private:
        const char* ptr = nullptr;
        char32_t code = 0;
        int units = 0;
    };

    // Utf16 forward iterator.
    class Utf16Iterator : public std::iterator<std::forward_iterator_tag, char32_t>
    {
    public:
        Utf16Iterator() = default;
        Utf16Iterator(const char16_t* const utf16);
        Utf16Iterator(const std::u16string& utf16);
        Utf16Iterator(const Utf16Iterator&) = default;
        ~Utf16Iterator() = default;
        Utf16Iterator& operator=(const Utf16Iterator&) = default;
        const char32_t operator*() const noexcept;
        const char32_t* operator->() const noexcept;
        Utf16Iterator& operator++() noexcept;
        Utf16Iterator operator++(int) noexcept;
        bool operator==(const Utf16Iterator& it) const noexcept;
        bool operator!=(const Utf16Iterator& it) const noexcept;
        bool operator==(const std::nullptr_t) const noexcept;
        bool operator!=(const std::nullptr_t) const noexcept;

    private:
        void Decode() noexcept;

    private:
        const char16_t* ptr = nullptr;
        char32_t code = 0;
        int units = 0;
    };
}