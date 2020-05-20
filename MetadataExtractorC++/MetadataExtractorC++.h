#pragma once
#include <Windows.h>
#include "PEParser.h"

#define ORIGINAL_FILENAME_STRING	L"OriginalFilename"
#define COMPANY_NAME_STRING			L"CompanyName"

#define CHECK_RET_CODE(ret,ERR)\
do {\
if (!ret)\
{\
std::cout << "Failed to parse: " << ERR << std::endl; \
goto out;\
}\
}while(0)

class MetadataEx
{
public:
	MetadataEx(const std::string& fileName);
	bool getOriginalFileName(std::wstring& originalFileName);
	bool getCompanyName(std::wstring& companyName);

private:
	const std::string m_fileName;
	version_values_t m_versionInfo;
	PEParser m_parser;

private:
	bool loadFile(const std::string& path, BYTE*& buffer, uint32_t& fileSize);
	bool parse(const std::string& path, BYTE* buffer, size_t bufSize);
	void resetParser();

	bool openFile(
		const char* path, HANDLE& handle);

	bool mapFile(
		HANDLE handle,
		BYTE*& buf);

	void* displayErrorString(
		DWORD);

	bool getValueFromKey(
		const std::wstring& key,
		std::wstring& value);

	bool searchVersionInfoByName(const std::wstring& key,
		const version_values_t& versionInfo,
		std::wstring& value);
};
