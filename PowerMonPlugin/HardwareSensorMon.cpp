#include "pch.h"
#include "HardwareSensorMon.h"


#if WINRT_USE_FLAG

const wchar_t* SmartPowerMeterMonItem::GetItemValueText() const
{
	return DetailFormatItemValueText();

}

const wchar_t* SmartPowerMeterMonItem::DetailFormatItemValueText(const wchar_t* zero_value_alternative, bool force_sign, int maximal_adaptive_decimal_places, int fixed_decimal_places, int value_unit_space) const
{
	static wchar_t out_val_text[PrintValueStrBufferSize] = L"";

	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->GetACLineStatus() == 0 && _lpBatteryInfoHandler->has_battery()) {

			_lpFormatHandler->FormatPowerWattsStringFromMili(
				out_val_text,
				PrintValueStrBufferSize,
				_lpBatteryInfoHandler->GetBatteryStatusPowerRate(),
				zero_value_alternative, force_sign, maximal_adaptive_decimal_places,
				fixed_decimal_places, value_unit_space
			);

		}
		else {
			auto pwr_val = _lpHWSensorDP->GetSmartCaculatePowerMeter();
			_lpFormatHandler->FormatPowerWattsString(
				out_val_text,
				PrintValueStrBufferSize,
				pwr_val, zero_value_alternative, force_sign, maximal_adaptive_decimal_places,
				fixed_decimal_places, value_unit_space);
		}
	}
	return out_val_text;
}

const wchar_t* CPUPowerMonItem::GetItemValueText() const
{
	return DetailFormatItemValueText();
}

const wchar_t* CPUPowerMonItem::DetailFormatItemValueText(const wchar_t* zero_value_alternative, bool force_sign, int maximal_adaptive_decimal_places, int fixed_decimal_places, int value_unit_space) const
{
	static wchar_t out_val_text[PrintValueStrBufferSize] = L"";

	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		float pwr_val = _lpHWSensorDP->GetCPUTotalPower();
		_lpFormatHandler->FormatPowerWattsString(
			out_val_text,
			PrintValueStrBufferSize,
			pwr_val, (zero_value_alternative == nullptr) ? _lpFormatHandler->GetNaNString() : zero_value_alternative, force_sign, maximal_adaptive_decimal_places,
			fixed_decimal_places, value_unit_space);
	}
	return out_val_text;
}

const wchar_t* GPUPowerMonItem::GetItemValueText() const
{
	return DetailFormatItemValueText();
}

const wchar_t* GPUPowerMonItem::DetailFormatItemValueText(const wchar_t* zero_value_alternative, bool force_sign, int maximal_adaptive_decimal_places, int fixed_decimal_places, int value_unit_space) const
{
	static wchar_t out_val_text[PrintValueStrBufferSize] = L"";

	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		float pwr_val = _lpHWSensorDP->GetGPUTotalPower();
		_lpFormatHandler->FormatPowerWattsString(
			out_val_text,
			PrintValueStrBufferSize,
			pwr_val, (zero_value_alternative == nullptr) ? _lpFormatHandler->GetNaNString() : zero_value_alternative, force_sign, maximal_adaptive_decimal_places,
			fixed_decimal_places, value_unit_space);
	}
	return out_val_text;
}

#endif