#include "BatteryPowerMonItem.h"
#include "LibreHWMonInterOp.h"
#pragma once


#if WINRT_USE_FLAG
class HardwareSensorMonBase :public IPluginItem {


protected:
	ValueUnitStringFormatter* _lpFormatHandler = nullptr;
	InterOpLibreHWMon::HardwareSensorDataProvider* _lpHWSensorDP = nullptr;
public:

	bool ReadyToPrint() const {
		return  _lpHWSensorDP;
	}

	bool SetDataSource(
		ValueUnitStringFormatter* lpFormatHandler,
		InterOpLibreHWMon::HardwareSensorDataProvider* lpHWSensorDP

	) {

		if (lpFormatHandler && lpHWSensorDP) {
			_lpFormatHandler = lpFormatHandler;
			_lpHWSensorDP = lpHWSensorDP;

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
};

class CPUPowerMonItem :public HardwareSensorMonBase {


public:

	virtual const wchar_t* GetItemName() const override {
		return g_data.StringRes(IDS_PLUGIN_CPU_POWER_METER_ITEM_NAME);
	};
	virtual const wchar_t* GetItemId() const override {
		return L"CPUPowerMon";
	};
	virtual const wchar_t* GetItemLableText() const override {
		return  g_data.StringRes(IDS_POWER_DISPLAY_LABEL);
	};
	virtual const wchar_t* GetItemValueText() const override;
	virtual const wchar_t* GetItemValueSampleText() const override {
		return GetItemValueText();
	};
};


class GPUPowerMonItem :public HardwareSensorMonBase {


public:

	virtual const wchar_t* GetItemName() const override {
		return g_data.StringRes(IDS_PLUGIN_GPU_POWER_METER_ITEM_NAME);
	};
	virtual const wchar_t* GetItemId() const override {
		return L"GPUPowerMon";
	};
	virtual const wchar_t* GetItemLableText() const override {
		return  g_data.StringRes(IDS_POWER_DISPLAY_LABEL);
	};
	virtual const wchar_t* GetItemValueText() const override;
	virtual const wchar_t* GetItemValueSampleText() const override {
		return GetItemValueText();
	};
};


class SmartPowerMeterMonItem :public BatteryGrpMonBaseCLS {

protected:

	InterOpLibreHWMon::HardwareSensorDataProvider* _lpHWSensorDP = nullptr;
public:

	bool ReadyToPrint() const {
		return _lpBatteryInfoHandler && _lpFormatHandler && _lpHWSensorDP;
	}

	bool SetDataSource(
		ValueUnitStringFormatter* lpFormatHandler,
		BatteryInfoHandler* lpBateryInfoHandler,
		InterOpLibreHWMon::HardwareSensorDataProvider* lpHWSensorDP

	) {
		BatteryGrpMonBaseCLS::SetDataSource(lpFormatHandler, lpBateryInfoHandler);

		if (lpHWSensorDP) {
			_lpHWSensorDP = lpHWSensorDP;

		}
		return ReadyToPrint();
	}


	virtual const wchar_t* GetItemName() const override {
		return g_data.StringRes(IDS_PLUGIN_SMART_POWER_METER_ITEM_NAME);
	};
	virtual const wchar_t* GetItemId() const override {
		return L"SmartPowerMeterMon";
	};
	virtual const wchar_t* GetItemLableText() const override {
		return  g_data.StringRes(IDS_POWER_DISPLAY_LABEL);
	};
	virtual const wchar_t* GetItemValueText() const override;
	virtual const wchar_t* GetItemValueSampleText() const override {
		return GetItemValueText();
	};

};

#endif