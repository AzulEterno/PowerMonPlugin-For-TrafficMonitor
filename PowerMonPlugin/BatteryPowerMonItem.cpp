#include "pch.h"
#include "BatteryPowerMonItem.h"
#include "DataManager.h"










const wchar_t* BatteryPowerMonItem::GetItemValueText() const {
	return DetailFormatItemValueText();
}

const wchar_t* BatteryPowerMonItem::DetailFormatItemValueText(
	const wchar_t* zero_value_alternative,
	bool force_sign,
	int maximal_adaptive_decimal_places,
	int fixed_decimal_places,
	int value_unit_space
) const
{
	static wchar_t out_val_text[PrintValueStrBufferSize] = L"";
	//wchar_t output_str[16];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			_lpFormatHandler->FormatPowerWattsStringFromMili(
				out_val_text,
				PrintValueStrBufferSize,
				_lpBatteryInfoHandler->GetBatteryStatusPowerRate(),
				zero_value_alternative,
				force_sign,
				maximal_adaptive_decimal_places,
				fixed_decimal_places,
				value_unit_space
			);

		}
		else {

			swprintf_s(out_val_text, _lpFormatHandler->GetNaNString());
		}
	}

	return out_val_text;
}







const wchar_t* BatteryPercentageMonItem::GetItemValueText() const
{
	return DetailFormatItemValueText();
}

const wchar_t* BatteryPercentageMonItem::DetailFormatItemValueText(
	const wchar_t* zero_value_alternative,
	bool force_sign,
	int maximal_adaptive_decimal_places,
	int fixed_decimal_places,
	int value_unit_space) const
{
	static wchar_t out_val_text[PrintValueStrBufferSize] = L"";
	//wchar_t output_str[16];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			BYTE value = _lpBatteryInfoHandler->GetSystemBatteryLifePercent();
			if (value != 255) {
				_lpFormatHandler->FormatPercentageString(
					out_val_text,
					PrintValueStrBufferSize,
					value,
					zero_value_alternative,
					force_sign,
					maximal_adaptive_decimal_places,
					fixed_decimal_places,
					value_unit_space
				);
			}
			else {
				swprintf_s(out_val_text, L"%s", g_data.StringRes(IDS_UNKNOWN).GetString());
			}
		}
		else {

			swprintf_s(out_val_text, _lpFormatHandler->GetNaNString());
		}
	}

	return out_val_text;
}



const wchar_t* BatteryCapacityMonItem::GetItemValueText() const
{
	return DetailFormatItemValueText();
}

const wchar_t* BatteryCapacityMonItem::DetailFormatItemValueText(const wchar_t* zero_value_alternative,
	bool force_sign,
	int maximal_adaptive_decimal_places,
	int fixed_decimal_places,
	int value_unit_space) const
{
	static wchar_t out_val_text[PrintValueStrBufferSize] = L"";
	//wchar_t output_str[16];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			auto value = _lpBatteryInfoHandler->GetBatteryCapacity();
			//CFPRT_FormatRealValue(out_val_text, PrintValueStrBufferSize, value, L"Wh");

			_lpFormatHandler->FormatEnergyWattsHourStringFromMili(out_val_text,
				PrintValueStrBufferSize, value,
				zero_value_alternative, force_sign,
				maximal_adaptive_decimal_places, fixed_decimal_places, value_unit_space);
		}
		else {

			swprintf_s(out_val_text, _lpFormatHandler->GetNaNString());
		}
	}

	return out_val_text;
}

const wchar_t* BatteryVoltageMonItem::GetItemValueText() const
{
	return DetailFormatItemValueText();
}

const wchar_t* BatteryVoltageMonItem::DetailFormatItemValueText(
	const wchar_t* zero_value_alternative,
	bool force_sign,
	int maximal_adaptive_decimal_places,
	int fixed_decimal_places, int value_unit_space
) const
{
	static wchar_t out_val_text[PrintValueStrBufferSize] = L"";
	//wchar_t output_str[16];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			auto value = _lpBatteryInfoHandler->GetBatteryStatusVoltage();
			_lpFormatHandler->FormatElectricVoltageStringFromMili(out_val_text,
				PrintValueStrBufferSize, value,
				zero_value_alternative, force_sign,
				maximal_adaptive_decimal_places, fixed_decimal_places, value_unit_space);
		}
		else {

			swprintf_s(out_val_text, _lpFormatHandler->GetNaNString());
		}
	}

	return out_val_text;
}

const wchar_t* BatteryTimeMonItem::GetItemValueText() const
{
	return DetailFormatItemValueText();
}

const wchar_t* BatteryTimeMonItem::DetailFormatItemValueText(
	const wchar_t* zero_value_alternative,
	bool force_sign, int maximal_adaptive_decimal_places,
	int fixed_decimal_places, int value_unit_space
) const
{
	static wchar_t out_val_text[PrintValueStrBufferSize] = L"";
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			switch (_lpBatteryInfoHandler->GetACLineStatus()) {
			case 1:
			{
				if (_lpBatteryInfoHandler->GetBatteryFullLifeTime() == -1) {
					swprintf_s(out_val_text, _lpFormatHandler->GetNaNString());
				}
				else {
					_lpFormatHandler->FormatTimerStringFromSeconds(out_val_text, PrintValueStrBufferSize,
						_lpBatteryInfoHandler->GetBatteryFullLifeTime());
				}

			}break;
			case 0:
			{
				if (_lpBatteryInfoHandler->GetBatteryLifeTime() == -1) {
					swprintf_s(out_val_text, _lpFormatHandler->GetNaNString());
				}
				else {
					_lpFormatHandler->FormatTimerStringFromSeconds(out_val_text, PrintValueStrBufferSize,
						_lpBatteryInfoHandler->GetBatteryLifeTime());
				}

			}break;
			default:
			{
				swprintf_s(out_val_text, _lpFormatHandler->GetNaNString());
			}break;


			}
		}
		else {

			swprintf_s(out_val_text, _lpFormatHandler->GetNaNString());
		}
	}

	return out_val_text;
}

