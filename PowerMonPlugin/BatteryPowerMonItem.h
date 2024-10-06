
#include "PluginInterface.h"
#include "pch.h"
#include "BatteryInfoHandler.h"
#include "DataManager.h"
#include "ValueUnitStringFormatter.h"
#pragma once
class BatteryGrpMonBaseCLS : public IPluginItem {

protected:
	//wchar_t pwr_unit_str[UNIT_STR_MAXLEN + 1] = L"W";
	ValueUnitStringFormatter* _lpFormatHandler = nullptr;
	//BatteryInfoHandler* _lpBatteryInfoHandler = nullptr;
	BatteryInfoHandler* _lpBatteryInfoHandler = nullptr;
public:

	bool ReadyToPrint() const {
		return _lpBatteryInfoHandler && _lpFormatHandler;
	}

	bool SetDataSource(ValueUnitStringFormatter* lpFormatHandler,
		BatteryInfoHandler* lpBateryInfoHandler

	) {
		if (lpBateryInfoHandler) {

			_lpBatteryInfoHandler = lpBateryInfoHandler;

		}
		if (lpFormatHandler) {
			_lpFormatHandler = lpFormatHandler;

		}
		return ReadyToPrint();
	}

	bool IsCustomDraw() const override {
		return false;
	};
	virtual int GetItemWidthEx(void* hDC) const override {
		//绘图句柄
		CDC* pDC = CDC::FromHandle((HDC)hDC);
		//TODO: 如果插件需要自绘，则在此修改显示区域的宽度
		return 40;
	};
	virtual void DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode) override {
		//绘图句柄
		CDC* pDC = CDC::FromHandle((HDC)hDC);
		//矩形区域
		CRect rect(CPoint(x, y), CSize(w, h));

		std::wstring current{ GetItemLableText(),GetItemValueText() };
		pDC->DrawText(current.c_str(), rect, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

		//TODO: 在此添加绘图代码
	};

	int OnMouseEvent(MouseEventType type, int x, int y, void* hWnd, int flag) {
		return 0;
	};

	virtual const wchar_t* GetItemValueSampleText() const override
	{
		return GetItemValueText();
	}



};


class BatteryPowerMonItem : public BatteryGrpMonBaseCLS
{
protected:

	//PowerMon &pwr_mon = PowerMon::Instance();
public:



	virtual const wchar_t* GetItemName() const override {
		//TODO: 返回插件显示名称？
		return g_data.StringRes(IDS_PLUGIN_BATTERY_POWER_ITEM_NAME);
	};
	virtual const wchar_t* GetItemId() const override {
		//TODO: 在此返回插件的唯一ID，建议只包含字母和数字
		return L"BatteryPowerMon";
	};
	virtual const wchar_t* GetItemLableText() const override {
		return g_data.StringRes(IDS_POWER_DISPLAY_LABEL);
	};
	virtual const wchar_t* GetItemValueText() const override;
	const wchar_t* DetailFormatItemValueText(
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = true,
		int maximal_adaptive_decimal_places = 2,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const;
};

class BatteryPercentageMonItem : public BatteryGrpMonBaseCLS
{
protected:

public:


	virtual const wchar_t* GetItemName() const override {
		return g_data.StringRes(IDS_PLUGIN_BATTERY_PERCENTAGE_ITEM_NAME);
	};
	virtual const wchar_t* GetItemId() const override {
		return L"BatteryPercentageMon";
	};
	virtual const wchar_t* GetItemLableText() const override {
		return  g_data.StringRes(IDS_BATTERY_DISPLAY_LABEL);
	};
	virtual const wchar_t* GetItemValueText() const override;

	const wchar_t* DetailFormatItemValueText(
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = 0,
		int fixed_decimal_places = 0,
		int value_unit_space = -1
	) const;


};


class BatteryCapacityMonItem : public BatteryGrpMonBaseCLS
{
protected:

public:


	virtual const wchar_t* GetItemName() const override {
		return g_data.StringRes(IDS_PLUGIN_BATTERY_CAPACITY_ITEM_NAME);
	};
	virtual const wchar_t* GetItemId() const override {
		return L"BatteryCapacityMon";
	};
	virtual const wchar_t* GetItemLableText() const override {
		return  g_data.StringRes(IDS_BATTERY_CAPACITY_DISPLAY_LABEL);
	};
	virtual const wchar_t* GetItemValueText() const override;

	const wchar_t* DetailFormatItemValueText(
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = 2,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const;


};

class BatteryVoltageMonItem : public BatteryGrpMonBaseCLS
{
protected:

public:


	virtual const wchar_t* GetItemName() const override {
		return g_data.StringRes(IDS_PLUGIN_BATTERY_VOLTAGE_ITEM_NAME);
	};
	virtual const wchar_t* GetItemId() const override {
		return L"BatteryVoltageMon";
	};
	virtual const wchar_t* GetItemLableText() const override {
		return  g_data.StringRes(IDS_BATTERY_VOLTAGE_DISPLAY_LABEL);
	};
	virtual const wchar_t* GetItemValueText() const override;

	const wchar_t* DetailFormatItemValueText(
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = 2,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const;




};

class BatteryTimeMonItem : public BatteryGrpMonBaseCLS
{
protected:

public:


	virtual const wchar_t* GetItemName() const override {
		return g_data.StringRes(IDS_PLUGIN_BATTERY_TIME_ITEM_NAME);
	};
	virtual const wchar_t* GetItemId() const override {
		return L"BatteryTimeMon";
	};
	virtual const wchar_t* GetItemLableText() const override {
		return  g_data.StringRes(IDS_BATTERY_TIME_LABEL);
	};
	virtual const wchar_t* GetItemValueText() const override;
	const wchar_t* DetailFormatItemValueText(
		const wchar_t* zero_value_alternative = nullptr,
		bool force_sign = false,
		int maximal_adaptive_decimal_places = 2,
		int fixed_decimal_places = -1,
		int value_unit_space = -1
	) const;




};
