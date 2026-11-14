#include "pch.h"
#include "ResourceUsageGraphItems.h"
#include "PowerMon.h"   // for PowerMon::Instance() and GetCachedMonitorInfo()

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

int ResourceUsageGraphMonItem::GetUsagePercent() const
{
	const auto& mi = PowerMon::Instance().GetCachedMonitorInfo();
	switch (m_source) {
	case Source::CpuUsage:    return mi.cpu_usage;
	case Source::GpuUsage:    return mi.gpu_usage;
	case Source::MemoryUsage: return mi.memory_usage;
	}
	return 0;
}

// ---------------------------------------------------------------------------
// IPluginItem — display metadata
// ---------------------------------------------------------------------------

const wchar_t* ResourceUsageGraphMonItem::GetItemName() const
{
	switch (m_source) {
	case Source::CpuUsage:    return g_data.StringRes(IDS_PLUGIN_CPU_USAGE_GRAPH_ITEM_NAME);
	case Source::GpuUsage:    return g_data.StringRes(IDS_PLUGIN_GPU_USAGE_GRAPH_ITEM_NAME);
	case Source::MemoryUsage: return g_data.StringRes(IDS_PLUGIN_MEM_USAGE_GRAPH_ITEM_NAME);
	}
	return L"";
}

const wchar_t* ResourceUsageGraphMonItem::GetItemId() const
{
	switch (m_source) {
	case Source::CpuUsage:    return L"CpuUsageGraph";
	case Source::GpuUsage:    return L"GpuUsageGraph";
	case Source::MemoryUsage: return L"MemUsageGraph";
	}
	return L"";
}

const wchar_t* ResourceUsageGraphMonItem::GetItemLableText() const
{
	switch (m_source) {
	case Source::CpuUsage:    return g_data.StringRes(IDS_CPU_USAGE_GRAPH_LABEL);
	case Source::GpuUsage:    return g_data.StringRes(IDS_GPU_USAGE_GRAPH_LABEL);
	case Source::MemoryUsage: return g_data.StringRes(IDS_MEM_USAGE_GRAPH_LABEL);
	}
	return L"";
}

const wchar_t* ResourceUsageGraphMonItem::GetItemValueText() const
{
	static wchar_t buf[16];
	int pct = GetUsagePercent();
	if (pct < 0) pct = 0;
	if (pct > 100) pct = 100;
	swprintf_s(buf, L"%d%%", pct);
	return buf;
}

const wchar_t* ResourceUsageGraphMonItem::GetItemValueSampleText() const
{
	return L"50%";
}

// ---------------------------------------------------------------------------
// API v6 — trend line graph value
// ---------------------------------------------------------------------------

float ResourceUsageGraphMonItem::GetResourceUsageGraphValue() const
{
	int pct = GetUsagePercent();
	if (pct < 0) pct = 0;
	if (pct > 100) pct = 100;
	return static_cast<float>(pct) / 100.0f;
}
