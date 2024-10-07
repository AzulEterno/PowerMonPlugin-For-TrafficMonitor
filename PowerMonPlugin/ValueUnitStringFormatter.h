#pragma once
#include "pch.h"
#include "DataManager.h"
#include <cmath>
#include <cstdio>
#include <minwindef.h>
#include <Shlwapi.h>
#include <winnt.h>


std::wstring GenerateRepeatString(const std::wstring& str, const int& x);

class ValueUnitStringFormatter
{
protected:
	wchar_t pwr_unit_str[UNIT_STR_MAXLEN] = L"W",
		electric_capcaity_unit_str[UNIT_STR_MAXLEN] = L"Wh",
		voltage_unit_str[UNIT_STR_MAXLEN] = L"V",
		hour_unit_str[UNIT_STR_MAXLEN] = L"H",
		minute_unit_str[UNIT_STR_MAXLEN] = L"M",
		nan_str[UNIT_STR_MAXLEN] = L"NaN";

	int _default_value_unit_space = 1, _default_max_adaptive_decimal_places = 2;

public:

	static int ConecateTwoStrWithRepeatedWord(
		wchar_t* out_val_text,
		int safe_length,
		const wchar_t* first_str,
		const wchar_t* second_str,
		const int repeat_times = 1,
		const wchar_t* interval_word = L" "
	) {
		//Pre-format string with space interval
		swprintf_s(out_val_text, safe_length, L"%s%s%s", first_str,
			GenerateRepeatString(interval_word, repeat_times).c_str(), second_str);

		return 0;
	}


	int CFPRT_FormatRealValue(
		wchar_t* out_val_text,
		int safe_length,
		double value,
		const wchar_t* unit_str,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = -1,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const {
		const wchar_t* format_str = nullptr;

		if (value_unit_space < 0) {
			value_unit_space = _default_value_unit_space;
		}
		const static int format_buffer_size = 24;
		//static wchar_t value_unit_str_pre_format[extended_unit_size] = L"";
		static wchar_t format_buffer[format_buffer_size] = L"",
			formated_value_str[format_buffer_size] = L"";



		if (fabs(value) < 1e-5) {
			if (zero_value_alternative) {
				swprintf_s(out_val_text, safe_length, L"%s", zero_value_alternative);
			}
			else {

				//swprintf_s(out_val_text, safe_length, L"0%s", unit_str);

				ConecateTwoStrWithRepeatedWord(out_val_text, safe_length, L"0", unit_str, value_unit_space);
			}
			return 0;
		}

		if (maximal_adaptive_decimal_places < 0) {
			maximal_adaptive_decimal_places = _default_max_adaptive_decimal_places;
		}

		// Build format string based on decimal places and force_sign flag

		int decimal_places = ((fixed_decimal_places < 0) ?
			max(maximal_adaptive_decimal_places - floor(log10(fabs(value))), 0) : fixed_decimal_places);






		if (force_sign) {
			swprintf_s(format_buffer, format_buffer_size, L"%%+.%dlf", decimal_places);
		}
		else {
			swprintf_s(format_buffer, format_buffer_size, L"%%.%dlf", decimal_places);
		}
		format_str = format_buffer;



		//OutputDebugString(format_buffer);
		// Format the output string
		swprintf_s(formated_value_str, safe_length, format_str, value);
		ConecateTwoStrWithRepeatedWord(out_val_text, safe_length, formated_value_str, unit_str, value_unit_space);


		return 0;
	}

	std::wstring WSTR_FormatFloatValue(
		double value,
		const wchar_t* unit_str,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = -1,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const {
		const int max_safe_str_len = 1024;
		static wchar_t _format_buffer[max_safe_str_len] = L"";

		CFPRT_FormatRealValue(
			_format_buffer,
			max_safe_str_len,
			value,
			unit_str,
			zero_value_alternative,
			force_sign,
			maximal_adaptive_decimal_places,
			fixed_decimal_places,
			value_unit_space
		);

		return std::wstring(_format_buffer);

	}

	int FormatPercentageString(wchar_t* out_val_text, int safe_length,
		double value,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = true,
		int maximal_adaptive_decimal_places = -1,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const {
		return CFPRT_FormatRealValue(
			out_val_text, safe_length,
			value, L"%", zero_value_alternative,
			force_sign, maximal_adaptive_decimal_places,
			fixed_decimal_places, value_unit_space);

	}

	int FormatPowerWattsStringFromMili(wchar_t* out_val_text, int safe_length,
		double mili_watts_value,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = true,
		int maximal_adaptive_decimal_places = -1,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const {
		return CFPRT_FormatRealValue(out_val_text, safe_length,
			mili_watts_value / 1000.0f, GetPowerUnitString(), zero_value_alternative,
			force_sign, maximal_adaptive_decimal_places,
			fixed_decimal_places, value_unit_space);

	}
	int FormatPowerWattsString(wchar_t* out_val_text, int safe_length,
		double watts_value,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = true,
		int maximal_adaptive_decimal_places = -1,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const {
		return CFPRT_FormatRealValue(out_val_text, safe_length,
			watts_value, GetPowerUnitString(), zero_value_alternative,
			force_sign, maximal_adaptive_decimal_places,
			fixed_decimal_places, value_unit_space);

	}
	int FormatEnergyWattsHourStringFromMili(wchar_t* out_val_text, int safe_length,
		int value,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = -1,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const {
		return CFPRT_FormatRealValue(out_val_text, safe_length,
			value / 1000.f, GetPowerCapacityString(), zero_value_alternative,
			force_sign, maximal_adaptive_decimal_places, fixed_decimal_places, value_unit_space);

	}

	int FormatElectricVoltageStringFromMili(wchar_t* out_val_text, int safe_length,
		int value,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = -1,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	)const {

		return CFPRT_FormatRealValue(out_val_text, safe_length,
			value / 1000.0f, GetVoltageString(), zero_value_alternative,
			force_sign, maximal_adaptive_decimal_places, fixed_decimal_places, value_unit_space);
	}

	int FormatTimerStringFromSeconds(
		wchar_t* out_val_text,
		int safe_length,
		INT64 seconds,
		const wchar_t* zero_value_alternative = nullptr
	) const {
		if (seconds < 0) {
			return swprintf_s(out_val_text, safe_length, L"?");
		}
		return swprintf_s(out_val_text, safe_length, L"%lld%s%02lld%s", seconds / 3600, hour_unit_str,
			(seconds / 60) % 60, minute_unit_str);

	}

	ValueUnitStringFormatter(bool auto_load = true) {
		if (auto_load) {
			LoadFromSettings(g_data.m_setting_data);
		}
	}



	const wchar_t* GetPowerUnitString()const {
		return pwr_unit_str;
	}
	const wchar_t* GetPowerCapacityString()const {
		return electric_capcaity_unit_str;
	}
	const wchar_t* GetVoltageString()const {
		return voltage_unit_str;
	}
	const wchar_t* GetHourString()const {
		return hour_unit_str;
	}
	const wchar_t* GetMinuteString()const {
		return minute_unit_str;
	}
	const wchar_t* GetNaNString()const {
		return nan_str;
	}

	bool SetPowerUnitStr(const LPCWSTR new_val) {

		StrCpyNW(pwr_unit_str, new_val, UNIT_STR_MAXLEN);
		return true;
	}

	bool SetElectricCapacityUnitStr(const LPCWSTR new_val) {
		StrCpyNW(electric_capcaity_unit_str, new_val,
			UNIT_STR_MAXLEN);
		return true;
	}

	bool SetElectricVoltageUnitStr(const LPCWSTR new_val) {
		StrCpyNW(voltage_unit_str, new_val,
			UNIT_STR_MAXLEN);
		return true;
	}
	bool SetHourUnitStr(const LPCWSTR new_val) {
		StrCpyNW(hour_unit_str, new_val,
			UNIT_STR_MAXLEN);
		return true;
	}
	bool SetMinuteUnitStr(const LPCWSTR new_val) {
		StrCpyNW(minute_unit_str, new_val,
			UNIT_STR_MAXLEN);
		return true;
	}
	bool SetNaNStr(const LPCWSTR new_val) {
		StrCpyNW(nan_str, new_val,
			UNIT_STR_MAXLEN);
		return true;
	}

	bool SetDefaultUnitStrSpaceCount(const INT64& sp_count) {
		_default_value_unit_space = sp_count;
		return true;
	}
	bool SetDefaultAdaptiveDecimalPlaces(const INT64& sp_count) {
		_default_max_adaptive_decimal_places = sp_count;
		return true;
	}


	int LoadFromSettings(const SettingData setting_data) {

		SetPowerUnitStr(setting_data.pwr_unit_str);
		SetElectricCapacityUnitStr(setting_data.electric_capacity_unit_str);
		SetElectricVoltageUnitStr(setting_data.electric_voltage_unit_str);
		SetHourUnitStr(setting_data.hour_unit_str);
		SetMinuteUnitStr(setting_data.minute_unit_str);
		SetNaNStr(setting_data.nan_str);

		SetDefaultUnitStrSpaceCount(setting_data.default_value_unit_space);
		SetDefaultAdaptiveDecimalPlaces(setting_data.default_max_adaptive_decimal_places);




		return 0;
	}

};

