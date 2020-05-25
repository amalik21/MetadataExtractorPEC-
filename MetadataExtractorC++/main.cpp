#include "MetadataExtractorC++.h"
#include <iostream>

#if 0
int old_main()
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
#endif

int main()
{
	versionInformationMap versionInfo;
	
	MetadataEx extractor;
	extractor.getVersionInformation(std::string("C:\\Windows\\system32\\hostname.exe"), versionInfo);
	
	auto originalFileName = versionInfo[ITEM_ID_VERSION_RESOURCE_ORIGINAL_FILE_NAME];
	auto companyName = versionInfo[ITEM_ID_VERSION_RESOURCE_COMPANY_NAME];
	auto productName = versionInfo[ITEM_ID_VERSION_RESOURCE_PRODUCT_NAME];
	auto productVersion = versionInfo[ITEM_ID_VERSION_RESOURCE_PRODUCT_VERSION];
	auto fileVersion = versionInfo[ITEM_ID_VERSION_RESOURCE_FILE_VERSION];

	std::wcout << "Original Filename is [" << originalFileName << "].\n";
	std::wcout << "Company Name is      [" << companyName << "].\n";
	std::wcout << "Product Name is      [" << productName << "].\n";
	std::wcout << "Product Version is   [" << productVersion << "].\n";
	std::wcout << "File Version is      [" << fileVersion << "].\n";

	return 0;
}