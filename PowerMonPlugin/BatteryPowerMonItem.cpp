#include "pch.h"
#include "BatteryPowerMonItem.h"
#include "DataManager.h"

#include "PowerMon.h"









const wchar_t* BatteryPowerMonItem::GetItemValueText() const
{
	static wchar_t out_val_text[PrintValueBuffer];
	//wchar_t output_str[16];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			_lpFormatHandler->FormatPowerWattsStringFromMili(
				out_val_text,
				PrintValueBuffer,
				_lpBatteryInfoHandler->GetBatteryStatusPowerRate());

		}
		else {

			swprintf_s(out_val_text, L"NaN");
		}
	}

	return out_val_text;
}

const wchar_t* BatteryPowerMonItem::GetItemValueSampleText() const
{
	return GetItemValueText();
}






const wchar_t* BatteryPercentageMonItem::GetItemValueText() const
{
	static wchar_t out_val_text[PrintValueBuffer];
	//wchar_t output_str[16];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			BYTE value = _lpBatteryInfoHandler->GetSystemBatteryLifePercent();
			if (value != 255) {
				swprintf_s(out_val_text, L"%d %%", value);
			}
			else {
				swprintf_s(out_val_text, L"%s", g_data.StringRes(IDS_UNKNOWN).GetString());
			}
		}
		else {

			swprintf_s(out_val_text, L"NaN");
		}
	}

	return out_val_text;
}

const wchar_t* BatteryPercentageMonItem::GetItemValueSampleText() const
{
	return GetItemValueText();
}



const wchar_t* BatteryCapacityMonItem::GetItemValueText() const
{
	static wchar_t out_val_text[PrintValueBuffer];
	//wchar_t output_str[16];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			auto value = _lpBatteryInfoHandler->GetBatteryCapacity();
			//FormatFloatValue(out_val_text, PrintValueBuffer, value, L"Wh");

			_lpFormatHandler->FormatEnergyWattsHourStringFromMili(out_val_text,
				PrintValueBuffer, value);
		}
		else {

			swprintf_s(out_val_text, L"NaN");
		}
	}

	return out_val_text;
}

const wchar_t* BatteryVoltageMonItem::GetItemValueText() const
{
	static wchar_t out_val_text[PrintValueBuffer];
	//wchar_t output_str[16];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			auto value = _lpBatteryInfoHandler->GetBatteryStatusVoltage();
			_lpFormatHandler->FormatElectricVoltageStringFromMili(out_val_text,
				PrintValueBuffer, value);
		}
		else {

			swprintf_s(out_val_text, L"NaN");
		}
	}

	return out_val_text;
}

const wchar_t* BatteryTimeMonItem::GetItemValueText() const
{
	static wchar_t out_val_text[PrintValueBuffer];
	if (!ReadyToPrint()) {
		swprintf_s(out_val_text, g_data.StringRes(IDS_NO_DATA_SOURCE));
	}
	else {

		//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
		if (_lpBatteryInfoHandler->has_battery()) {

			switch (_lpBatteryInfoHandler->GetACLineStatus()) {
			case 1:
			{
				_lpFormatHandler->FormatTimerStringFromSeconds(out_val_text, PrintValueBuffer,
					_lpBatteryInfoHandler->GetBatteryFullLifeTime());
			}break;
			case 0:
			{
				_lpFormatHandler->FormatTimerStringFromSeconds(out_val_text, PrintValueBuffer,
					_lpBatteryInfoHandler->GetBatteryLifeTime());
			}break;
			default:
			{
				swprintf_s(out_val_text, L"NaN");
			}break;


			}
		}
		else {

			swprintf_s(out_val_text, L"NaN");
		}
	}

	return out_val_text;
}

