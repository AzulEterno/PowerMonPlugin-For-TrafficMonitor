#pragma once
#include "pch.h"
#include "DataManager.h"


class ValueUnitStringFormatter
{
protected:
	wchar_t pwr_unit_str[PWR_UNIT_STR_MAXLEN] = L"W",
		electric_capcaity_unit_str[PWR_UNIT_STR_MAXLEN] = L"Wh",
		voltage_unit_str[PWR_UNIT_STR_MAXLEN] = L"V";

public:

	static int FormatFloatValue(wchar_t* out_val_text, int safe_length,
		float value, const wchar_t* unit_str,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = 2,
		int fixed_decimal_places = -1
	) {
		const wchar_t* format_str = nullptr;

		if (fabs(value) < 1e-5) {
			if (zero_value_alternative) {
				swprintf_s(out_val_text, safe_length, L"%s", zero_value_alternative);
			}
			else {

				swprintf_s(out_val_text, safe_length, L"0 %s", unit_str);
			}
			return 0;
		}

		// Build format string based on decimal places and force_sign flag

		int decimal_places = ((fixed_decimal_places < 0) ?
			max(floor(log10(fabs(value))), 0) : fixed_decimal_places);

		const int format_buffer_size = 16;
		static wchar_t format_buffer[format_buffer_size];
		if (force_sign) {
			swprintf_s(format_buffer, format_buffer_size, L"%%+.%df %%s", decimal_places);
		}
		else {
			swprintf_s(format_buffer, format_buffer_size, L"%%.%df %%s", decimal_places);
		}
		format_str = format_buffer;



		//OutputDebugString(format_buffer);
		// Format the output string
		swprintf_s(out_val_text, safe_length, format_str, value, unit_str);

		return 0;
	}

	int FormatPowerWattsStringFromMili(wchar_t* out_val_text, int safe_length,
		int value,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = true,
		int maximal_adaptive_decimal_places = 2,
		int fixed_decimal_places = -1
	) const {
		return FormatFloatValue(out_val_text, safe_length,
			value / 1000.0, GetPowerUnitString(), zero_value_alternative,
			force_sign, maximal_adaptive_decimal_places,
			fixed_decimal_places);

	}
	int FormatEnergyWattsHourStringFromMili(wchar_t* out_val_text, int safe_length,
		int value,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = 2,
		int fixed_decimal_places = -1
	) const {
		return FormatFloatValue(out_val_text, safe_length,
			value / 1000.0, GetPowerCapacityString(), zero_value_alternative,
			force_sign, maximal_adaptive_decimal_places, fixed_decimal_places);

	}

	int FormatElectricVoltageStringFromMili(wchar_t* out_val_text, int safe_length,
		int value,
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = 3,
		int fixed_decimal_places = -1
	)const {

		return FormatFloatValue(out_val_text, safe_length,
			value / 1000.0, GetVoltageString(), zero_value_alternative,
			force_sign, maximal_adaptive_decimal_places, fixed_decimal_places);
	}

	int FormatTimerStringFromSeconds(wchar_t* out_val_text, int safe_length,
		int seconds,
		const wchar_t* zero_value_alternative = nullptr
	) const {
		if (seconds < 0) {
			return swprintf_s(out_val_text, safe_length, L"?");
		}
		return swprintf_s(out_val_text, safe_length, L"%d:%02d", (int)floor(seconds / 3600),
			(int)floor(seconds / 60) % 60);

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

	bool SetPowerUnitStr(const LPCWSTR new_val) {

		StrCpyNW(pwr_unit_str, new_val, PWR_UNIT_STR_MAXLEN);
		return true;
	}

	bool SetElectricCapacityUnitStr(const LPCWSTR new_val) {
		StrCpyNW(electric_capcaity_unit_str, new_val,
			PWR_UNIT_STR_MAXLEN);
		return true;
	}

	bool SetElectricVoltageUnitStr(const LPCWSTR new_val) {
		StrCpyNW(voltage_unit_str, new_val,
			PWR_UNIT_STR_MAXLEN);
		return true;
	}

	int LoadFromSettings(const SettingData setting_data) {

		SetPowerUnitStr(setting_data.pwr_unit_str);
		SetElectricCapacityUnitStr(setting_data.electric_capacity_unit_str);

		return 0;
	}

};

