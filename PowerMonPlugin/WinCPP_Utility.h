#pragma once

#include "pch.h"

typedef struct _LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
} LANGCODEPAGE;


typedef struct _FileVersionInfoStruct {
	std::wstring companyName;
	std::wstring fileDescription;
	std::wstring fileVersion;
	std::wstring internalName;
	std::wstring legalCopyright;
	std::wstring originalFilename;
	std::wstring productName;
	std::wstring productVersion;
} FILEVERSIONINFOSTRUCT;

FILEVERSIONINFOSTRUCT GetFileVersionInfoStruct(const std::wstring& filePath) {
	FILEVERSIONINFOSTRUCT versionInfo = {};

	// Get the size of the version information
	DWORD handle = 0;
	DWORD size = GetFileVersionInfoSize(filePath.c_str(), &handle);
	if (size == 0) {
		return versionInfo; // Error or no version info
	}

	// Allocate buffer for version information
	std::vector<BYTE> buffer(size);
	if (!GetFileVersionInfo(filePath.c_str(), handle, size, buffer.data())) {
		return versionInfo; // Error retrieving version info
	}

	// Retrieve the language and code page
	LANGCODEPAGE* translate = {};
	UINT cbTranslate = 0;
	if (!VerQueryValue(buffer.data(), L"\\VarFileInfo\\Translation", (LPVOID*)&translate, &cbTranslate)) {
		return versionInfo; // Error retrieving translation info
	}

	// Retrieve version information strings
	auto queryValue = [&](const wchar_t* name, std::wstring& value) {
		wchar_t subBlock[256];
		swprintf_s(subBlock, L"\\StringFileInfo\\%04x%04x\\%s", translate->wLanguage, translate->wCodePage, name);
		LPVOID pBuffer = nullptr;
		UINT size = 0;
		if (VerQueryValue(buffer.data(), subBlock, &pBuffer, &size) && size > 0) {
			value.assign((wchar_t*)pBuffer, size - 1);
		}
		};

	queryValue(L"CompanyName", versionInfo.companyName);
	queryValue(L"FileDescription", versionInfo.fileDescription);
	queryValue(L"FileVersion", versionInfo.fileVersion);
	queryValue(L"InternalName", versionInfo.internalName);
	queryValue(L"LegalCopyright", versionInfo.legalCopyright);
	queryValue(L"OriginalFilename", versionInfo.originalFilename);
	queryValue(L"ProductName", versionInfo.productName);
	queryValue(L"ProductVersion", versionInfo.productVersion);

	return versionInfo;
}


FILEVERSIONINFOSTRUCT GetSelfFileVersionInfoStructData() {

	HMODULE hModule = nullptr;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCTSTR>(GetFileVersionInfoStruct),  // Replace with an address within the DLL
		&hModule
	);

	TCHAR dllPath[MAX_PATH];
	GetModuleFileName(hModule, dllPath, MAX_PATH);


	return GetFileVersionInfoStruct(dllPath);
}