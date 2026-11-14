// ResourceUsageGraphItems.h — API v6 resource usage trend graph items
// These items display CPU/GPU/memory usage as TM-native trend line graphs.
// Data comes from ITMPlugin::OnMonitorInfo() (TrafficMonitor host), NOT from
// LibreHardwareMonitor, so these work on ALL platforms including ARM64.
#pragma once

#include "PluginInterface.h"
#include "pch.h"
#include "DataManager.h"

// Forward declaration — PowerMon includes this header, so we use a late-bound
// accessor to avoid a circular include.  The implementation file resolves the
// actual PowerMon instance.
class PowerMon;

// A single item class parameterized by which MonitorInfo field to read.
// Three instances are created (CPU, GPU, memory) and registered in GetItem().
class ResourceUsageGraphMonItem : public IPluginItem {
public:
	enum class Source {
		CpuUsage,
		GpuUsage,
		MemoryUsage,
	};

	explicit ResourceUsageGraphMonItem(Source src = Source::CpuUsage)
		: m_source(src) {}

	void SetSource(Source src) { m_source = src; }

	// --- IPluginItem overrides ---

	const wchar_t* GetItemName() const override;
	const wchar_t* GetItemId() const override;
	const wchar_t* GetItemLableText() const override;
	const wchar_t* GetItemValueText() const override;
	const wchar_t* GetItemValueSampleText() const override;

	// API v6: trend line graph (zero-draw — TM renders the graph)
	int IsDrawResourceUsageGraph() const override { return 1; }
	float GetResourceUsageGraphValue() const override;

private:
	Source m_source;

	// Read the cached value (0–100) from PowerMon's MonitorInfo cache.
	int GetUsagePercent() const;
};
