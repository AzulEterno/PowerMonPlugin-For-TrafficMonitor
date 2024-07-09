#pragma once
#include "PluginInterface.h"


static wchar_t out_val_text[20];

class PowerMonItem : public IPluginItem
{
private:
    
    //PowerMon &pwr_mon = PowerMon::Instance();
public:
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
