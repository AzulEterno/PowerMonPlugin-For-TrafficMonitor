#include "pch.h"
#include "ValueUnitStringFormatter.h"


std::wstring GenerateRepeatString(const std::wstring& str, const int& x) {
	if (x <= 0) {
		return L""; // Return an empty string if x is 0 or negative
	}

	std::wstring result;
	result.reserve(str.size() * x); // Reserve memory for efficiency

	for (int i = 0; i < x; ++i) {
		result += str; // Append the string x times
	}

	return result;
}