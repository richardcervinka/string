#include "String.h"
#include <cctype>
#include <algorithm>

// Utf8Iterator

String::Utf8Iterator::Utf8Iterator() {
	ptr = nullptr;
	code = 0;
	units = 0;
}

String::Utf8Iterator::Utf8Iterator( const char* const utf8Str ) {
	ptr = utf8Str;
	Decode();
}

String::Utf8Iterator::Utf8Iterator( const std::string& utf8Str ) {
	ptr = utf8Str.c_str();
	Decode();
}

void String::Utf8Iterator::Decode() noexcept {
	if ( ptr == nullptr ) {
		code = 0;
		units = 0;
		return;
	}

	const unsigned char* byte = reinterpret_cast< const unsigned char* >( ptr );

	// 1 byte character 0xxxxxxx
	if ( *byte < static_cast< unsigned char >( 0xc0 ) ) {
		code = static_cast< char32_t >( *byte );
		units = 1;
		return;
	}
	// 2 byte character 110xxxxx 10xxxxxx
	if ( *byte < static_cast< unsigned char >( 0xe0 ) ) {
		code =
			( ( static_cast< char32_t >( *( byte + 0 ) ) & 0x01fu ) << 6 ) |
			( ( static_cast< char32_t >( *( byte + 1 ) ) & 0x03fu ) );
		units = 2;
		return;
	}
	// 3 byte character 1110xxxx 10xxxxxx 10xxxxxx
	if ( *byte < static_cast< unsigned char >( 0xf0 ) ) {
		code =
			( ( static_cast< char32_t >( *( byte + 0 ) ) & 0x000f ) << 12 ) |
			( ( static_cast< char32_t >( *( byte + 1 ) ) & 0x003f )	<< 6 )  |
			( ( static_cast< char32_t >( *( byte + 2 ) ) & 0x003f ) );
		units = 3;
		return;
	}
	// 4 byte character 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	if ( *byte < static_cast< unsigned char >( 0xf8 ) ) {
		code =
			( ( static_cast< char32_t >( *( byte + 0 ) ) & 0x0007 ) << 18 ) |
			( ( static_cast< char32_t >( *( byte + 1 ) ) & 0x003f ) << 12 ) |
			( ( static_cast< char32_t >( *( byte + 2 ) ) & 0x003f ) << 6 )  |
			( ( static_cast< char32_t >( *( byte + 3 ) ) & 0x003f ) );
		units = 4;
		return;
	}
	// 5 byte character 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	if ( *byte < static_cast< unsigned char >( 0xfC ) ) {
		code =
			( ( static_cast< char32_t >( *( byte + 0 ) ) & 0x0003 ) << 24 ) |
			( ( static_cast< char32_t >( *( byte + 1 ) ) & 0x003f ) << 18 ) |
			( ( static_cast< char32_t >( *( byte + 2 ) ) & 0x003f ) << 12 ) |
			( ( static_cast< char32_t >( *( byte + 3 ) ) & 0x003f ) << 6 )  |
			( ( static_cast< char32_t >( *( byte + 4 ) ) & 0x003f ) );
		units = 5;
		return;
	}
	// 6 byte character 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	code =
		( ( static_cast< char32_t >( *( byte + 0 ) ) & 0x0001 ) << 30 ) |
		( ( static_cast< char32_t >( *( byte + 1 ) ) & 0x003f ) << 24 ) |
		( ( static_cast< char32_t >( *( byte + 2 ) ) & 0x003f ) << 18 ) |
		( ( static_cast< char32_t >( *( byte + 3 ) ) & 0x003f ) << 12 ) |
		( ( static_cast< char32_t >( *( byte + 4 ) ) & 0x003f ) << 6 )  |
		( ( static_cast< char32_t >( *( byte + 5 ) ) & 0x003f ) );
	units = 6;
}

const char32_t String::Utf8Iterator::operator*() const noexcept {
	return code;
}

const char32_t* String::Utf8Iterator::operator->() const {
	return &code;
}

String::Utf8Iterator& String::Utf8Iterator::operator++() noexcept {
	if ( ptr == nullptr ) {
		return *this;
	}
	ptr = ptr + units;
	Decode();
	return *this;
}

String::Utf8Iterator String::Utf8Iterator::operator++( int ) noexcept {
	if ( ptr == nullptr ) {
		return *this;
	}
	Utf8Iterator old = *this;
	ptr = ptr + units;
	Decode();
	return old;
}

bool String::Utf8Iterator::operator!=( const String::Utf8Iterator& it ) const noexcept {
	return this->ptr != it.ptr;
}

bool String::Utf8Iterator::operator==( const String::Utf8Iterator& it ) const noexcept {
	return this->ptr == it.ptr;
}

bool String::Utf8Iterator::operator==( const std::nullptr_t ) const noexcept {
	return ptr == nullptr;
}

bool String::Utf8Iterator::operator!=( const std::nullptr_t ) const noexcept {
	return ptr != nullptr;
}

int String::Utf8Iterator::Units() const noexcept {
	return units;
}

// Utf16Iterator

String::Utf16Iterator::Utf16Iterator() {
	ptr = nullptr;
	code = 0;
}

String::Utf16Iterator::Utf16Iterator( const char16_t* const utf16Str ) {
	ptr = utf16Str;
	Decode();
}

String::Utf16Iterator::Utf16Iterator( const std::u16string& utf16Str ) {
	ptr = utf16Str.c_str();
	Decode();
}

void String::Utf16Iterator::Decode() noexcept {
	if ( ptr == nullptr ) {
		code = 0;
		return;
	}
	// surrogate pairs
	if ( *ptr >= 0xd800 && *ptr <= 0xdfff ) {
		code = ( ( *ptr & 0x3ff ) << 10 ) | ( *( ptr + 1 ) & 0x3ff );
		code += 0x010000;
		return;
	}
	code = *ptr;
}

const char32_t String::Utf16Iterator::operator*() const noexcept {
	return code;
}

const char32_t* String::Utf16Iterator::operator->() const {
	return &code;
}

String::Utf16Iterator& String::Utf16Iterator::operator++() noexcept {
	if ( ptr == nullptr ) {
		return *this;
	}
	ptr += Units();
	Decode();
	return *this;
}

String::Utf16Iterator String::Utf16Iterator::operator++( int ) noexcept {
	if ( ptr == nullptr ) {
		return *this;
	}
	Utf16Iterator old = *this;
	ptr += Units();
	Decode();
	return old;
}

bool String::Utf16Iterator::operator!=( const String::Utf16Iterator& it ) const noexcept {
	return this->ptr != it.ptr;
}

bool String::Utf16Iterator::operator==( const String::Utf16Iterator& it ) const noexcept {
	return this->ptr == it.ptr;
}

bool String::Utf16Iterator::operator==( const std::nullptr_t ) const noexcept {
	return ptr == nullptr;
}

bool String::Utf16Iterator::operator!=( const std::nullptr_t ) const noexcept {
	return ptr != nullptr;
}

int String::Utf16Iterator::Units() const noexcept {
	if ( code > 0xffff ) {
		return 2;
	}
	return 1;
}

// String functions

void String::PushUtf8( std::string& target, char32_t ch ) {
	// 1 byte
	if ( ch <= 0x7f ) {
		target.push_back( static_cast< char >( ch ) );
		return;
	}
	// 2 byte
	if ( ch <= 0x7ff ) {
		target.push_back( static_cast< char >( 0xc0 | ( ( ch >> 6 ) & 0x1f ) ) );
		target.push_back( static_cast< char >( 0x80 | ( ( ch >> 0 ) & 0x3f ) ) );
		return;
	}
	// 3 byte
	if ( ch <= 0xffff ) {
		target.push_back( static_cast< char >( 0xe0 | ( ( ch >> 12 ) & 0x0f ) ) );
		target.push_back( static_cast< char >( 0x80 | ( ( ch >> 6  ) & 0x3f ) ) );
		target.push_back( static_cast< char >( 0x80 | ( ( ch >> 0  ) & 0x3f ) ) );
		return;
	}
	// 4 byte
	if ( ch <= 0x1fffff ) {
		target.push_back( static_cast< char >( 0xf0u | ( ( ch >> 18 ) & 0x07u ) ) );
		target.push_back( static_cast< char >( 0x80u | ( ( ch >> 12 ) & 0x3fu ) ) );
		target.push_back( static_cast< char >( 0x80u | ( ( ch >> 6  ) & 0x3fu ) ) );
		target.push_back( static_cast< char >( 0x80u | ( ( ch >> 0  ) & 0x3fu ) ) );
		return;
	}
}

void String::PushUtf16( std::u16string& target, char32_t ch ) {
	// Unicode define no characters in range 0xd800 - 0xdfff
	if ( ch >= 0xd800 && ch <= 0xdfff ) {
		return;
	}
	// surrogate pairs
	if ( ch > 0xffff ) {
		target.push_back( 0xd800 | ( ( ( ch - 0x010000 ) >> 10 ) & 0x3ff ) );
		target.push_back( 0xdc00 | ( ( ( ch - 0x010000 ) >> 0  ) & 0x3ff ) );
		return;
	}
	target.push_back( static_cast< char16_t >( ch ) );
}

std::string String::Utf8( const char16_t* const src ) {
	std::string str;
	Utf16Iterator iter( src );
	while ( *iter != 0 ) {
		PushUtf8( str, *iter );
		++iter;
	}
	return str;
}

std::string String::Utf8( const std::u16string& src ) {
	return Utf8( src.c_str() );
}

std::string String::Utf8( const char32_t* const src ) {
	std::string str;
	const char32_t* iter = src;
	while ( *iter != 0 ) {
		PushUtf8( str, *iter );
		++iter;
	}
	return str;
}

std::string String::Utf8( const std::u32string& src ) {
	return Utf8( src.c_str() );
}

std::u16string String::Utf16( const char* const src ) {
	std::u16string str;
	Utf8Iterator iter( src );
	while ( *iter != 0 ) {
		PushUtf16( str, *iter );
		++iter;
	}
	return str;
}

std::u16string String::Utf16( const std::string& src ) {
	return Utf16( src.c_str() );
}

std::u16string String::Utf16( const char32_t* const src ) {
	std::u16string str;
	const char32_t* iter = src;
	while ( *iter != 0 ) {
		PushUtf16( str, *iter );
		++iter;
	}
	return str;
}

std::u16string String::Utf16( const std::u32string& src ) {
	return Utf16( src.c_str() );
}

std::u32string String::Utf32( const char* const src ) {
	std::u32string str;
	Utf8Iterator iter( src );
	while ( *iter != 0 ) {
		str.push_back( *iter );
		++iter;
	}
	return str;
}

std::u32string String::Utf32( const std::string& src ) {
	return Utf32( src.c_str() );
}

std::u32string String::Utf32( const char16_t* const src ) {
	std::u32string str;
	Utf16Iterator iter( src );
	while ( *iter != 0 ) {
		str.push_back( *iter );
		++iter;
	}
	return str;
}

std::u32string String::Utf32( const std::u16string& src ) {
	return Utf32( src.c_str() );
}

int String::Length( const char* const str ) noexcept {
	if ( str == nullptr ) {
		return 0;
	}
	int count = 0;
	Utf8Iterator iter( str );
	while ( *iter != 0 ) {
		++count;
		++iter;
	}
	return count;
}

int String::Length( const std::string& str ) noexcept {
	return Length( str.c_str() );
}

int String::Length( const char16_t* const str ) noexcept {
	if ( str == nullptr ) {
		return 0;
	}
	int count = 0;
	Utf16Iterator iter( str );
	while ( *iter != 0 ) {
		++count;
		++iter;
	}
	return count;
}

int String::Length( const std::u16string& str ) noexcept {
	return Length( str.data() );
}

int String::Length( const char32_t* const str ) noexcept {
	return std::char_traits< char32_t >::length( str );
}

int String::Length( const std::u32string& str ) noexcept {
	return static_cast< int >( str.length() );
}

int String::Strlen( const char* const str ) noexcept {
	return std::char_traits< char >::length( str );
}

int String::Strlen( const char16_t* const str ) noexcept {
	return std::char_traits< char16_t >::length( str );
}

int String::Strlen( const char32_t* const str ) noexcept {
	return std::char_traits< char32_t >::length( str );
}

int String::Strlen( std::string& str ) noexcept {
	return static_cast< int >( str.length() );
}

int String::Strlen( std::u16string& str ) noexcept {
	return static_cast< int >( str.length() );
}

int String::Strlen( std::u32string& str ) noexcept {
	return static_cast< int >( str.length() );
}

template< typename T >
int ReplaceT( T* const str, const T ref, const T replace ) noexcept {
	if ( str == nullptr ) {
		return 0;
	}
	if ( ref == replace ) {
		return 0;
	}
	int count = 0;
	T* ch = str;
	while ( *ch != 0 ) {
		if ( *ch == ref ) {
			*ch = replace;
			count += 1;
		}
		ch++;
	}
	return count;
}

int String::Replace( char* const str, const char ref, const char replace ) noexcept {
	return ReplaceT( str, ref, replace );
}

int String::Replace( char16_t* const str, const char16_t ref, const char16_t replace ) noexcept {
	return ReplaceT( str, ref, replace );
}

template< typename TStr, typename TChar >
int ReplaceStdStringT( TStr& str, const TChar ref, const TChar replace ) noexcept {
	if ( ref == replace ) {
		return 0;
	}
	int count = 0;
	for ( auto& ch : str ) {
		if ( ch == ref ) {
			ch = replace;
			count += 1;
		}
	}
	return count;
}

int String::Replace( std::string& str, const char ref, const char replace ) noexcept {
	return ReplaceStdStringT( str, ref, replace );
}

int String::Replace( std::u16string& str, const char16_t ref, const char16_t replace ) noexcept {
	return ReplaceStdStringT( str, ref, replace );
}

//************************

template< typename Iterator, typename T >
int FindT( const Iterator begin, const T ref ) noexcept {
	Iterator it = begin;
	int index = 0;
	while ( *it != 0 ) {
		if ( *it == ref ) {
			return index;
		}
		++index;
		++it;
	}
	return -1;
}

int String::Find( const char* const begin, const char ref ) noexcept {
	return FindT( begin, ref );
}

int String::Find( const char16_t* const begin, const char16_t ref ) noexcept {
	return FindT( begin, ref );
}

int String::Find( const char32_t* const begin, const char32_t ref ) noexcept {
	return FindT( begin, ref );
}

int String::Find( const std::string& str, const char ref ) noexcept {
	return static_cast< int >( str.find( ref ) );
}

int String::Find( const std::u16string& str, const char16_t ref ) noexcept {
	return static_cast< int >( str.find( ref ) );
}

int String::Find( const std::u32string& str, const char32_t ref ) noexcept {
	return static_cast< int >( str.find( ref ) );
}

int String::Find( const std::string::const_iterator begin, const char ref ) noexcept {
	return FindT( begin, ref );
}

int String::Find( const std::u16string::const_iterator begin, const char16_t ref ) noexcept {
	return FindT( begin, ref );
}

int String::Find( const std::u32string::const_iterator begin, const char32_t ref ) noexcept {
	return FindT( begin, ref );
}

template< typename Iter, typename Type >
int FindUtfT( const Iter& iterator, const Type ref ) noexcept {
	auto it = iterator;
	int units = 0;
	while ( *it != 0 ) {
		if ( *it == ref ) {
			return units;
		}
		units += it.Units();
		++it;
	}
	return units;
}

int String::Find( const Utf8Iterator& iterator, const char32_t ref ) noexcept {
	return FindUtfT( iterator, ref );
}

int String::Find( const Utf16Iterator& iterator, const char32_t ref ) noexcept {
	return FindUtfT( iterator, ref );
}

void String::Lower( char* const str ) {
	char* ch = str;
	while ( *ch != '\0' ) {
		if ( *ch >= 65 && *ch <= 90 ) {
			*ch += 32;
		}
		++ch;
	}
}

void String::Upper( char* const str ) {
	char* ch = str;
	while ( *ch != '\0' ) {
		if ( *ch >= 97 && *ch <= 122 ) {
			*ch -= 32;
		}
		++ch;
	}
}

template< typename T >
int FindLastT( T* const str, const T ref, const int pos ) noexcept {
	int found = -1;
	int i = 0;
	T* ch = str;
	while ( *ch != 0 && i <= pos ) {
		if ( *ch == ref ) {
			found = i;
		}
		ch++;
		i++;
	}
	return found;
}

int String::FindLast( char* const str, const char ref, const int pos ) noexcept {
	return FindLastT( str, ref, pos );
}

int String::FindLast( char16_t* const str, const char16_t ref, const int pos ) noexcept {
	return FindLastT( str, ref, pos );
}

int String::FindLast( std::string& str, const char ref, const int pos ) noexcept {
	const auto found = str.rfind( ref, static_cast< size_t >( pos ) );
	if ( found == std::string::npos ) {
		return -1;
	}
	return found;
}

int String::FindLast( std::u16string& str, const char16_t ref, const int pos ) noexcept {
	const auto found = str.rfind( ref, static_cast< size_t >( pos ) );
	if ( found == std::u16string::npos ) {
		return -1;
	}
	return found;
}


bool FileNameEquals( const std::string& left, const std::string& right ) {
	if ( left.length() != right.length() ) {
		return false;
	}
	auto l = left.begin();
	auto r = right.begin();
	const auto end = left.cend();
	while ( l != end ) {
		if ( std::tolower( static_cast< int >( *l ) ) != std::tolower( static_cast< int >( *r ) ) ) {
			return false;
		}
		++l;
		++r;
	}
	return true;
}

template <typename T>
int CompareNoCaseT( const T* const left, const T* const right ) noexcept {
	// invalid tapameters
	if ( left == nullptr || right == nullptr ) {
		return false;
	}
	// equals pointers address
	if ( left == right ) {
		return 0;
	}
	const T* l = left;
	const T* r = right;
	int eq = std::tolower( *l ) - std::tolower( *r );
	while ( eq == 0 ) {
		// end of string
		if ( *l == 0 ) {
			return 0;
		}
		eq = std::tolower( *( ++l ) ) - std::tolower( *( ++r ) );
	}
	return eq;
}

int String::CompareNoCase( const char* const left, const char* const right ) noexcept {
	return CompareNoCaseT( left, right );
}

int String::CompareNoCase( const char16_t* const left, const char16_t* const right ) noexcept {
	return CompareNoCaseT( left, right );
}

template< typename TChar, typename TString >
std::vector< TString > SplitT( const TChar* const str, const TChar splitter ) noexcept {
	if ( str == nullptr ) {
		return {};
	}
	const TChar* begin = str;
	const TChar* end = str;
	std::vector< TString > result;

	while ( *end != 0 ) {
		if ( *end == splitter ) {
			if ( end != begin ) {
				result.push_back( TString( begin, end ) );
			}
			begin = end + 1;
		}
		++end;
	}
	if ( end != begin ) {
		result.push_back( TString( begin, end ) );
	}
	return result;
}

std::vector< std::string > String::Split( const char* const str, const char splitter ) noexcept {
	return SplitT< char, std::string >( str, splitter );
}

std::vector< std::u16string > String::Split( const char16_t* const str, const char16_t splitter ) noexcept {
	return SplitT< char16_t, std::u16string >( str, splitter );
}

std::vector< std::string > String::Split( const std::string& str, const char splitter ) noexcept {
	return SplitT< char, std::string >( str.c_str(), splitter );
}

std::vector< std::u16string > String::Split( const std::u16string& str, const char16_t splitter ) noexcept {
	return SplitT< char16_t, std::u16string >( str.c_str(), splitter );
}