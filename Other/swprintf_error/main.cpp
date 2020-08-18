#include <iostream>
#include <locale>
#include <codecvt>

std::string WStringToString (const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
	return std::string (convert.to_bytes (str));
}

int main (int, char**)
{
	std::cout << "BEGIN" << std::endl;
	
	std::wstring directString (L"Aa\xc1");
	std::cout << WStringToString (directString) << std::endl;
	
	wchar_t formattedStringBuf[32];
	swprintf (formattedStringBuf, 32, L"%ls", directString.c_str ());
	
	std::wstring formattedString (formattedStringBuf);
	std::cout << WStringToString (formattedString) << std::endl;

	std::cout << "END" << std::endl;
	return EXIT_SUCCESS;
}
