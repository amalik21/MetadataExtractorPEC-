// MetadataExtractorC++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "PEParser.h"

class MetadataEx
{
public:
	bool loadFile(const std::string& path, BYTE*& buffer, uint32_t& fileSize);
	bool parse(const std::string& path, BYTE* buffer, size_t bufSize);
	void resetParser();
	
public:
	PEParser m_parser;
	
private:
	bool openFile(const char* path, HANDLE& handle);
	bool mapFile(HANDLE handle, BYTE* &buf);
	void* displayErrorString(DWORD);
};

void* MetadataEx::displayErrorString(DWORD error)
{
	LPVOID lpMsgBuf;
	if (!FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf,
		0,
		NULL))
	{
		return NULL;
	}

	return lpMsgBuf;
}

void MetadataEx::resetParser()
{
	m_parser.reset();
}

bool MetadataEx::openFile(const char* path, HANDLE& handle)
{
	handle = CreateFileA(
		path,    // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (handle == INVALID_HANDLE_VALUE)
	{
		wchar_t* errorString = (wchar_t*)displayErrorString(GetLastError());
		std::wcout << "Unable to open file " << path <<
			" for reading, err: " << GetLastError() << ": " << errorString << "\n";
		LocalFree(errorString);
		return false;
	}

	return true;
}

bool MetadataEx::mapFile(HANDLE handle, BYTE*& buf)
{
	bool ret = false;
	void* start = NULL;
	HANDLE hMapFile;

	hMapFile = CreateFileMapping(
		handle,
		NULL,                    // default security
		PAGE_READONLY,           // read/write access
		0,                       // maximum object size (high-order DWORD)
		0,                       // maximum object size (low-order DWORD)
		NULL);                   // name of mapping object

	if (hMapFile == NULL)
	{
		wchar_t* errorString = (wchar_t*)displayErrorString(GetLastError());
		printf("Could not create file mapping object, err: %d, msg: '%S'\n",
			GetLastError(), errorString);
		LocalFree(errorString);
		return false;
	}

	start = (void*)MapViewOfFile(
		hMapFile,		// handle to map object
		FILE_MAP_READ,	// read permission
		0, 0, 0);

	if (start == NULL)
	{
		std::cout << "Could not map view of file, err: " <<
			GetLastError() << std::endl;
	}
	else
	{
		buf = (BYTE*)start;
		std::cout << "Successfully mapped file in process address space.\n";
		ret = true;
	}

	if ((hMapFile != 0) && (hMapFile != INVALID_HANDLE_VALUE))
		CloseHandle(hMapFile);

	return ret;
}

bool MetadataEx::loadFile(const std::string& path, BYTE*& buffer, uint32_t& fileSize)
{
	HANDLE handle;
	if (!openFile(path.c_str(), handle))
		return false;

	auto ret =  mapFile(handle, buffer);
	fileSize = GetFileSize(handle, NULL);

	if (handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}

	return ret;
}

bool MetadataEx::parse(const std::string& path, BYTE* buffer, size_t bufSize)
{
	return m_parser.parse(path, buffer, bufSize);
}

int main()
{
	std::string fileName("C:\\Windows\\system32\\calc.exe");
	BYTE* fileData = nullptr;
	uint32_t fileSize = 0;
	version_values_t version_info;

	MetadataEx extractor;
	auto load_ok = extractor.loadFile(fileName, fileData, fileSize);
	auto parse_ok = load_ok ? extractor.m_parser.parse(fileName, fileData, fileSize) : false;
	auto resource_ok = parse_ok ? extractor.m_parser.parseResourceDir((int)RT_VERSION): false;
	auto version_ok = resource_ok ? extractor.m_parser.parseVersionInfo(version_info) : false;

	if (version_ok)
	{
		std::cout << "\n*************************" << std::endl;
		for (auto i : version_info)
		{
			std::wcout << i.first << " = " << i.second << std::endl;
		}
		std::cout << "*************************" << std::endl;
	}
	
	return 0;
}
