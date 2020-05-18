#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>

//namespace uc
//{
    enum PEfileType
    {
        PE_TYPE_NONE  = 0x0,
        PE_TYPE_32BIT = 0x1,
        PE_TYPE_64BIT = 0x2
    };

    typedef struct
    {
        char name[IMAGE_SIZEOF_SHORT_NAME + 1];
        unsigned long startRva;
        unsigned long endRva;
        int sectionHdrIndex;
        IMAGE_SECTION_HEADER* pSectionHdr;
    } resource_section_header_t;

    typedef struct
    {
        IMAGE_RESOURCE_DIRECTORY* pDir;
        IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry;
    }resource_tree_level_t;

    typedef struct
    {
        resource_section_header_t hdr;
        unsigned long datadirRva;
        unsigned long offset;
        resource_tree_level_t levels[3];
        IMAGE_RESOURCE_DATA_ENTRY* pData;
        BYTE* pDataBuffer;
        std::vector<std::pair<std::wstring, std::wstring>> resources;
    } resource_section_info_t;

using version_value_t = std::pair<std::wstring, std::wstring>;
using version_values_t = std::vector<version_value_t>;

#define ALIGN_32BIT_BOUNDRY(i)		(i) = ((i) + 0x3) & (~0x3)
#define VERINFO_PARSE_ERR           "VersionInfo parse error: "
#define PE_PARSE_ERR				"Not a valid PE: "

#define VS_VERSION_STRING			(L"VS_VERSION_INFO")
#define VS_VERSION_STRING_LEN		(sizeof (VS_VERSION_STRING))/sizeof(wchar_t)

#define FILE_INFO_STRING            (L"StringFileInfo")
#define FILE_INFO_STRING_LEN		sizeof (FILE_INFO_STRING)/sizeof(wchar_t)

#define VAR_FILE_INFO_STRING        (L"VarFileInfo")
#define VAR_FILE_INFO_STRING_LEN    sizeof (VAR_FILE_INFO_STRING)/sizeof(wchar_t)

#define ENG_LANG_CODE_STRING        (L"09")

#pragma pack(1)
	typedef struct version_info_st {
		UINT16 length;
		UINT16 val_length;
		UINT16 type;
		wchar_t key[VS_VERSION_STRING_LEN];
		BYTE* opaque;
	} version_info_t;
#pragma pack()

#pragma pack(1)
	typedef struct string_file_info_st {
		UINT16 length;
		UINT16 val_length;
		UINT16 type;
		wchar_t key[FILE_INFO_STRING_LEN];
		char* opaque;
	} string_file_info_t;
#pragma pack()

#pragma pack(1)
	typedef struct string_tbl_st {
		UINT16 length;
		UINT16 val_length;
		UINT16 type;
		wchar_t key[8];
		char* opaque;
	} string_tbl_t;
#pragma pack()

#pragma pack(1)
	typedef struct string_st {
		UINT16 length;
		UINT16 val_length;
		UINT16 type;
		wchar_t opaque[1];
	} string_t;
#pragma pack()

    class PEParser
    {
        public:
            void reset();
            bool parse(const std::string& filepath,
				const BYTE* pBuffer,
				const uint32_t bufSize);

			bool getResourceSection();
            bool parseResourceDir(int resourceId);
            bool parseVersionInfo(version_values_t& vi);
			uint32_t getSubsystem();

        private:
            static constexpr DWORD BUF_SIZE = (8 * 1024); // 8k
            std::string m_fileName;
            PEfileType m_flags;
            uint32_t m_numSections;
			uint32_t m_numDataDirectories;
			uint32_t m_subSystem;
			uint32_t m_bufSize;

            IMAGE_DOS_HEADER* m_pDosHdr;				/* Dos header */
            IMAGE_NT_HEADERS* m_pPeHdr;					/* PE header */
            IMAGE_NT_HEADERS32* m_pNtHdr32;				/* Nt header 32-bit */
            IMAGE_NT_HEADERS64* m_pNtHdr64;				/* Nt header 64-bit */
            IMAGE_FILE_HEADER* m_pFileHdr;				/* File header */
            IMAGE_OPTIONAL_HEADER32* m_pOptionalHdr32;	/* Optional header 32 bit*/
            IMAGE_OPTIONAL_HEADER64* m_pOptionalHdr64;	/* Optional header 64 bit */
            IMAGE_SECTION_HEADER* m_pSectionTable;		/* Section table */
            resource_section_info_t	m_resourceSection;

        private:
			bool parsePE32();
			bool parsePE64();
            bool getResourceDirectoryAndEntry(
                    BYTE* base,
                    unsigned long offset,
                    IMAGE_RESOURCE_DIRECTORY** dir,
                    IMAGE_RESOURCE_DIRECTORY_ENTRY** entry);
    };
//}

