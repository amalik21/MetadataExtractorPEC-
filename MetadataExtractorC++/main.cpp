#include "MetadataExtractorC++.h"
#include <iostream>

int main()
{
	std::wstring originalFileName, companyName;
	std::string fileName("C:\\Windows\\system32\\calc.exe");

	MetadataEx extractor(std::string("C:\\Windows\\system32\\calc.exe"));
	if (extractor.getOriginalFileName(originalFileName))
	{
		if (extractor.getCompanyName(companyName))
		{
			std::cout << "\n*************************" << std::endl;
			std::wcout << L"Original Filename = " << originalFileName << std::endl;
			std::wcout << L"Company  Name = " << companyName << std::endl;
			std::cout << "*************************" << std::endl;
		}
	}

	return 0;
}