#pragma once
#include "PluginInterface.h"




class PowerMonItem : public IPluginItem
{
private:
	wchar_t pwr_unit_str[PWR_UNIT_STR_MAXLEN + 1] = L"W";
	//PowerMon &pwr_mon = PowerMon::Instance();
public:

	bool SetPowerUnitStr(const LPWSTR new_val) {
		StrCpy(pwr_unit_str, new_val);
		return true;
	}

	virtual const wchar_t* GetItemName() const override;
	virtual const wchar_t* GetItemId() const override;
	virtual const wchar_t* GetItemLableText() const override;
	virtual const wchar_t* GetItemValueText() const override;
	virtual const wchar_t* GetItemValueSampleText() const override;
	virtual bool IsCustomDraw() const override;
	virtual int GetItemWidthEx(void* hDC) const override;
	virtual void DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode) override;

	int OnMouseEvent(MouseEventType type, int x, int y, void* hWnd, int flag);



};
