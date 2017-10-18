#pragma once

#include <cstring>
#include <string>
#include <vector>

namespace String {

	//namespace

	//namespace Utf8 {
	//}


	class Utf8Iterator : public std::iterator< std::forward_iterator_tag, char32_t > {
	public:
		Utf8Iterator();
		explicit Utf8Iterator( const char* const utf8Str );
		explicit Utf8Iterator( const std::string& utf8Str );
		const char32_t operator*() const noexcept;
		const char32_t* operator->() const;
		Utf8Iterator& operator++() noexcept;
		Utf8Iterator operator++( int ) noexcept;
		bool operator==( const Utf8Iterator& it ) const noexcept;
		bool operator!=( const Utf8Iterator& it ) const noexcept;
		bool operator==( const std::nullptr_t ) const noexcept;
		bool operator!=( const std::nullptr_t ) const noexcept;

		// Number of code units of the character
		int Units() const noexcept;

	private:
		void Decode() noexcept;

		const char* ptr;
		char32_t code;
		int units;
	};

	class Utf16Iterator : public std::iterator< std::forward_iterator_tag, char32_t > {
	public:
		Utf16Iterator();
		explicit Utf16Iterator( const char16_t* const utf16Str );
		explicit Utf16Iterator( const std::u16string& utf16Str );
		const char32_t operator*() const noexcept;
		const char32_t* operator->() const;
		Utf16Iterator& operator++() noexcept;
		Utf16Iterator operator++( int ) noexcept;
		bool operator==( const Utf16Iterator& it ) const noexcept;
		bool operator!=( const Utf16Iterator& it ) const noexcept;
		bool operator==( const std::nullptr_t ) const noexcept;
		bool operator!=( const std::nullptr_t ) const noexcept;

		// Number of code units of the character
		int Units() const noexcept;

	private:
		void Decode() noexcept;

		const char16_t* ptr;
		char32_t code;
	};

	// create utf8 from utf16
	std::string Utf8( const char16_t* const src );
	std::string Utf8( const std::u16string& src );

	// create utf8 from utf32
	std::string Utf8( const char32_t* const src );
	std::string Utf8( const std::u32string& src );

	// create utf16 from utf8
	std::u16string Utf16( const char* const src );
	std::u16string Utf16( const std::string& src );

	// create utf16 from utf32
	std::u16string Utf16( const char32_t* const src );
	std::u16string Utf16( const std::u32string& src );

	// create utf32 from utf8
	std::u32string Utf32( const char* const src );
	std::u32string Utf32( const std::string& src );

	// create utf32 from utf16
	std::u32string Utf32( const char16_t* const src );
	std::u32string Utf32( const std::u16string& src );

	// Push character with code ch as utf character into the target
	void PushUtf8( std::string& target, char32_t ch );
	void PushUtf16( std::u16string& target, char32_t ch );

	// Convert built-in types to utf8 string (ASCII string)
	template< typename T >
	const std::string Utf8( const T value ) {
		return std::to_string( value );
	}

	// Return length of the utf8 string.
	int Length( const char* const str ) noexcept;
	int Length( const std::string& str ) noexcept;

	// Return length of the utf16 string.
	int Length( const char16_t* const str ) noexcept;
	int Length( const std::u16string& str ) noexcept;

	// Return length of the utf32 string.
	int Length( const char32_t* const str ) noexcept;
	int Length( const std::u32string& str ) noexcept;

	// Return number of fixed size characters in string (like strlen).
	int Strlen( const char* const str ) noexcept;
	int Strlen( const char16_t* const str ) noexcept;
	int Strlen( const char32_t* const str ) noexcept;
	int Strlen( std::string& str ) noexcept;
	int Strlen( std::u16string& str ) noexcept;
	int Strlen( std::u32string& str ) noexcept;

	// Find first occurrence of the character, if there is no one, return -1.

	int Find( const char* const begin, const char ref ) noexcept;
	int Find( const char16_t* const begin, const char16_t ref ) noexcept;
	int Find( const char32_t* const begin, const char32_t ref ) noexcept;

	int Find( const std::string& str, const char ref ) noexcept;
	int Find( const std::u16string& str, const char16_t ref ) noexcept;
	int Find( const std::u32string& str, const char32_t ref ) noexcept;

	int Find( const std::string::const_iterator begin, const char ref ) noexcept;
	int Find( const std::u16string::const_iterator begin, const char16_t ref ) noexcept;
	int Find( const std::u32string::const_iterator begin, const char32_t ref ) noexcept;

	int Find( const Utf8Iterator& iterator, const char32_t ref ) noexcept;
	int Find( const Utf16Iterator& iterator, const char32_t ref ) noexcept;

	// Convert ASCII characters A-Z to lowercase
	void Lower( char* const str );

	// Convert ASCII characters a-z to uppercase
	void Upper( char* const str );

	// find substring
	// ...

	// Find last occurrence of the character and return its position.
	// If there is no one, return -1.
	// pos is the last compared charcter.
	int FindLast( char* const str, const char ref, const int pos ) noexcept;
	int FindLast( char16_t* const str, const char16_t ref, const int pos ) noexcept;
	int FindLast( std::string& str, const char ref, const int pos ) noexcept;
	int FindLast( std::u16string& str, const char16_t ref, const int pos ) noexcept;

	// Replace all ref characters by replace character. Returns total number of replaced characters.
	int Replace( char* const str, const char ref, const char replace ) noexcept;
	int Replace( char16_t* const str, const char16_t ref, const char16_t replace ) noexcept;
	int Replace( std::string& str, const char ref, const char replace ) noexcept;
	int Replace( std::u16string& str, const char16_t ref, const char16_t replace ) noexcept;

	// No case sensitive comparing
	int CompareNoCase( const char* const left, const char* const right ) noexcept;
	int CompareNoCase( const char16_t* const left, const char16_t* const right ) noexcept;

	// Split string into the substrings
	std::vector< std::string > Split( const char* const str, const char splitter ) noexcept;
	std::vector< std::u16string > Split( const char16_t* const str, const char16_t splitter ) noexcept;
	std::vector< std::string > Split( const std::string& str, const char splitter ) noexcept;
	std::vector< std::u16string > Split( const std::u16string& str, const char16_t splitter ) noexcept;
}