#include "pch.h"
#include "PowerMonItem.h"
#include "DataManager.h"

#include "PowerMon.h"




const wchar_t* PowerMonItem::GetItemName() const
{
	return g_data.StringRes(IDS_PLUGIN_ITEM_NAME);
}

const wchar_t* PowerMonItem::GetItemId() const
{
	//TODO: 在此返回插件的唯一ID，建议只包含字母和数字
	return L"BatteryPowerMon";
}

const wchar_t* PowerMonItem::GetItemLableText() const
{
	return g_data.StringRes(IDS_BATTERY_POWER_DISPLAY_LABEL).GetString();
}

const wchar_t* PowerMonItem::GetItemValueText() const
{
	static wchar_t out_val_text[32];
	//wchar_t output_str[16];

	PowerMon& inf_obj = PowerMon::Instance();
	//MessageBox(NULL, L"Instance got.", L"GetPowerMonItem", MB_OK);
	if (inf_obj.on_battery) {

		float value = inf_obj.qry_res_inf.Rate / (float)(1000);
		if (fabs(value) > 10) {
			swprintf_s(out_val_text, L"%.1f %s", value, pwr_unit_str);
		}
		else {
			swprintf_s(out_val_text, L"%.2f %s", value, pwr_unit_str);
		}
		//return (const wchar_t *)output_str;
		//MessageBox(NULL, out_val_text, L"GetPowerMonValue", MB_OK);
	}
	else {
		//MessageBox(NULL, L"Not formatting data due to on_battery value", L"Warning", MB_OK);
		swprintf_s(out_val_text, L"NaN");
	}


	return out_val_text;
}

const wchar_t* PowerMonItem::GetItemValueSampleText() const
{
	return PowerMonItem::GetItemValueText();
}

bool PowerMonItem::IsCustomDraw() const
{
	//TODO: 根据是否由插件自绘返回对应的值
	return false;
}

int PowerMonItem::GetItemWidthEx(void* hDC) const
{
	//绘图句柄
	CDC* pDC = CDC::FromHandle((HDC)hDC);
	//TODO: 如果插件需要自绘，则在此修改显示区域的宽度
	return 40;
}

void PowerMonItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)
{
	//绘图句柄
	CDC* pDC = CDC::FromHandle((HDC)hDC);
	//矩形区域
	CRect rect(CPoint(x, y), CSize(w, h));

	std::wstring current{ GetItemLableText(),GetItemValueText() };
	pDC->DrawText(current.c_str(), rect, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	//TODO: 在此添加绘图代码
}

int PowerMonItem::OnMouseEvent(MouseEventType type, int x, int y, void* hWnd, int flag) {
	if (false && type == MT_DBCLICKED) {
		PowerMon& inf_obj = PowerMon::Instance();
		std::wstringstream info_str;


		info_str << "Current Battery Handler:" << std::hex << (void*)inf_obj.hBattery << std::endl;
		info_str << "Current PSP_DEVICE_INTERFACE_DETAIL_DATA:" << std::hex << (void*)inf_obj.pdidd << std::endl;
		info_str << "Current BATTERY_WAIT_STATUS:" << std::hex << (void*)&inf_obj.qry_bws_inf << std::endl;
		info_str << "Current BATTERY_TAG:" << inf_obj.qry_bws_inf.BatteryTag << std::endl;
		info_str << "Debug Mode:" << inf_obj.is_dbg << std::endl;
		info_str << "Press OK to reset battery driver." << std::endl;


		int msgboxID = MessageBox(NULL, info_str.str().c_str(), L"调试信息窗口",
			MB_OK | MB_ICONINFORMATION);

		if (msgboxID == IDOK) {
			inf_obj.free_res_mem();
			inf_obj.update_battery_base_info();
		}
		return 1;

	}

	return 0;
};