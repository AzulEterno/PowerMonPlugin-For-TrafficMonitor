# Plan: 进度条显示功能

> **状态**: 草案
> **日期**: 2026-11-14
> **目标**: 为 PowerMon 插件的传感器显示项添加进度条/条形图渲染模式

---

## 1. 背景

TrafficMonitor 插件接口（已对齐到 **API v7**，对应 TrafficMonitor V1.85+）提供两种可视化增强通道：

**通道 A：资源占用趋势图（API v6，零绘制代码）**

- `IPluginItem::IsDrawResourceUsageGraph()` → 返回 `1` 启用趋势图
- `IPluginItem::GetResourceUsageGraphValue()` → 返回 0.0–1.0 的值
- TrafficMonitor 自动在任务栏项目中绘制趋势折线图

**通道 B：自定义绘制（进度条/条形图）**

- `IPluginItem::IsCustomDraw()` → 返回 `true` 接管绘制
- `IPluginItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)` → GDI 绘制入口
- `IPluginItem::GetItemWidth()` / `GetItemWidthEx(void* hDC)` → 控制显示宽度

Battery 插件 (`BatteryItem.cpp:82-93`) 已用 `CDrawCommon::DrawRoundRect()` 在 HDC 上绘制了电量填充条作为先例。

## 2. 适合进度条显示的传感器

| 传感器 | 数据类型 | 进度条适合度 | 说明 |
|---|---|---|---|
| BatteryPercentageMonItem | 0–100% | ★★★ | 天然百分比，最佳候选 |
| MonitorInfo::cpu_usage | 0–100% | ★★★ | 主机提供的 CPU 占用率 |
| MonitorInfo::gpu_usage | 0–100% | ★★★ | 主机提供的 GPU 占用率 |
| MonitorInfo::memory_usage | 0–100% | ★★★ | 主机提供的内存占用率 |
| CPUPowerMonItem | 瓦特 | ★★ | 需要映射到 TDP 上限，可配置 |
| GPUPowerMonItem | 瓦特 | ★★ | 需要映射到 TDP 上限，可配置 |
| SmartPowerMeterMonItem | 瓦特 | ★ | 自适应值，映射上限不直观 |
| BatteryPowerMonItem | 瓦特 | ★ | 充放电功率，双向，不适合单条 |
| BatteryCapacityMonItem | mWh | ★ | 绝对容量，非百分比 |
| BatteryVoltageMonItem | V | ★ | 绝对电压，非百分比 |
| BatteryTimeMonItem | hh:mm:ss | ✗ | 时间值，不适合进度条 |

## 3. 设计方案

### 3.1 新增 `ProgressBarRenderer` 辅助类

独立的 GDI 绘制辅助类，不耦合到任何特定 Item：

```cpp
// ProgressBarRenderer.h
class ProgressBarRenderer {
public:
    // 绘制水平进度条
    // percent: 0.0–100.0
    // bar_color: 填充颜色（可根据百分比动态计算）
    void DrawHorizontalBar(
        CDC* pDC,
        int x, int y, int w, int h,
        double percent,
        COLORREF bar_color,
        COLORREF bg_color,
        COLORREF border_color,
        bool dark_mode
    );

    // 根据百分比自动计算颜色 (绿→黄→红)
    static COLORREF ColorForPercent(double percent);
};
```

**颜色梯度逻辑：**
- 0–50%: 绿色 `RGB(76, 175, 80)` → 黄色 `RGB(255, 193, 7)`
- 50–80%: 黄色 → 橙色 `RGB(255, 87, 34)`
- 80–100%: 橙色 → 红色 `RGB(244, 67, 54)`

### 3.2 显示模式枚举

```cpp
enum class DisplayMode : int {
    TextOnly = 0,      // 当前行为：纯文本 (label + value)
    BarOnly = 1,       // 仅进度条（无文本）
    TextAndBar = 2,    // 文本 + 进度条（水平排列）
    BarWithValue = 3,  // 进度条内叠加文字
};
```

### 3.3 新增 `SensorDisplayConfig` 结构

每个传感器独立配置显示模式：

```cpp
struct SensorDisplayConfig {
    DisplayMode battery_pct_mode = DisplayMode::TextOnly;
    DisplayMode cpu_usage_mode   = DisplayMode::BarOnly;
    DisplayMode gpu_usage_mode   = DisplayMode::BarOnly;
    DisplayMode mem_usage_mode   = DisplayMode::BarOnly;
    DisplayMode cpu_power_mode   = DisplayMode::TextOnly;
    DisplayMode gpu_power_mode   = DisplayMode::TextOnly;

    // 功率传感器的 TDP 映射上限（W），用于瓦特→百分比转换
    int cpu_tdp_for_bar = 125;   // 默认 125W
    int gpu_tdp_for_bar = 250;   // 默认 250W

    // 进度条外观
    int bar_height_px = 10;      // 进度条高度（96 DPI 基准）
    int bar_total_width_px = 60; // TextAndBar/BarOnly 模式下总宽度
    bool bar_gradient_color = true; // 使用渐变色 vs 固定色
};
```

### 3.4 INI 配置键

在 `[config]` 节新增：

```ini
; === 显示模式 (0=文本, 1=仅进度条, 2=文本+进度条, 3=进度条叠字) ===
battery_pct_display_mode=0    ; 电池百分比显示模式
cpu_usage_display_mode=1      ; CPU 占用率显示模式
gpu_usage_display_mode=1      ; GPU 占用率显示模式
mem_usage_display_mode=1      ; 内存占用率显示模式
cpu_power_display_mode=0      ; CPU 功耗显示模式
gpu_power_display_mode=0      ; GPU 功耗显示模式

; === 功率→进度条映射 ===
cpu_tdp_for_bar=125           ; CPU TDP 上限（W），用于功率→百分比转换
gpu_tdp_for_bar=250           ; GPU TDP 上限（W）

; === 进度条外观 ===
bar_height_px=10              ; 进度条高度（96 DPI）
bar_total_width_px=60         ; 进度条区域总宽度
bar_gradient_color=1          ; 1=渐变色(绿→黄→红), 0=固定色
```

### 3.5 新增 IPluginItem 子类

为每个适合进度条的传感器新增一个 `*BarMonItem` 类：

```
BatteryPercentageBarMonItem     — 电池百分比进度条
CpuUsageBarMonItem              — CPU 占用率进度条
GpuUsageBarMonItem              — GPU 占用率进度条
MemoryUsageBarMonItem           — 内存占用率进度条
CpuPowerBarMonItem              — CPU 功耗进度条（瓦特→百分比 via TDP）
GpuPowerBarMonItem              — GPU 功耗进度条（瓦特→百分比 via TDP）
```

**关键实现要点（以 `BatteryPercentageBarMonItem` 为例）：**

```cpp
class BatteryPercentageBarMonItem : public BatteryGrpMonBaseCLS {
    ProgressBarRenderer _renderer;

public:
    const wchar_t* GetItemName() const override {
        return L"Battery Percentage Bar";
    }
    const wchar_t* GetItemId() const override {
        return L"BatteryPctBar";
    }

    bool IsCustomDraw() const override { return true; }

    int GetItemWidth() const override {
        return g_data.m_setting_data.bar_total_width_px;
    }

    void DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode) override {
        CDC* pDC = CDC::FromHandle((HDC)hDC);
        double pct = GetBatteryPercentage(); // 0.0–100.0

        COLORREF bar_color = g_data.m_setting_data.bar_gradient_color
            ? ProgressBarRenderer::ColorForPercent(pct)
            : RGB(76, 175, 80);

        _renderer.DrawHorizontalBar(
            pDC, x, y, w, h, pct,
            bar_color,
            dark_mode ? RGB(40, 40, 40) : RGB(220, 220, 220),
            dark_mode ? RGB(80, 80, 80) : RGB(180, 180, 180),
            dark_mode
        );

        // BarWithValue 模式下叠加百分比文字
        if (mode == DisplayMode::BarWithValue) {
            CString text;
            text.Format(L"%.0f%%", pct);
            pDC->SetTextColor(dark_mode ? RGB(255, 255, 255) : RGB(0, 0, 0));
            pDC->SetBkMode(TRANSPARENT);
            pDC->DrawText(text, CRect(x, y, x + w, y + h),
                          DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
};
```

### 3.6 CPU/GPU/内存占用率数据来源

TrafficMonitor 主机通过 `ITMPlugin::OnMonitorInfo(const MonitorInfo&)` 传递 CPU/GPU/内存占用率。API v7 还提供了 `ITrafficMonitor::GetMonitorValue(MonitorItem)` 作为替代数据获取方式（需在 `OnInitialize()` 中保存 `ITrafficMonitor*` 指针）。当前 `PowerMon` 类未重写此方法。需要：

1. `PowerMon` 重写 `OnMonitorInfo()`，缓存 `cpu_usage`、`gpu_usage`、`memory_usage`
2. 新增的 `*BarMonItem` 类从 `PowerMon::Instance()` 读取这些缓存值

> **注意**：`MonitorInfo` 结构在 API v7 中新增了 `cpu_freq` 字段（CPU 频率），可用于后续扩展。

```cpp
// PowerMon.cpp
void PowerMon::OnMonitorInfo(const MonitorInfo& info) {
    m_cached_monitor_info = info;
}
```

### 3.7 选项对话框更新

在 **HardwareSensorPage** 中新增一组控件：

**新增控件区域：「显示模式设置」**

| 控件类型 | 标签 | 关联配置键 |
|---|---|---|
| ComboBox | 电池百分比显示 | `battery_pct_display_mode` |
| ComboBox | CPU 占用率显示 | `cpu_usage_display_mode` |
| ComboBox | GPU 占用率显示 | `gpu_usage_display_mode` |
| ComboBox | 内存占用率显示 | `mem_usage_display_mode` |
| ComboBox | CPU 功耗显示 | `cpu_power_display_mode` |
| ComboBox | GPU 功耗显示 | `gpu_power_display_mode` |
| SpinEdit | CPU TDP 上限 (W) | `cpu_tdp_for_bar` |
| SpinEdit | GPU TDP 上限 (W) | `gpu_tdp_for_bar` |
| CheckBox | 进度条渐变色 | `bar_gradient_color` |
| SpinEdit | 进度条宽度 (px) | `bar_total_width_px` |

ComboBox 选项：`文本 | 仅进度条 | 文本+进度条 | 进度条+文字`

**对话框布局调整**：HardwareSensorPage 当前已有传感器树视图和功率过滤控件。新增「显示模式设置」组放在传感器树视图上方或新建一个「显示」分组。如果控件过多，考虑在对话框中增加第四个选项页「显示设置」。

## 4. 实现步骤

### Phase 0: 资源占用趋势图（低成本，立即可用）

1. 为 CPU/GPU/内存占用率的 Item 实现 `IsDrawResourceUsageGraph()` → 返回 `1`
2. 实现 `GetResourceUsageGraphValue()` → 返回缓存的占用率值（0.0–1.0）
3. `PowerMon` 重写 `OnMonitorInfo()` 缓存主机数据
4. 测试：验证 TrafficMonitor 自动在任务栏项目中绘制趋势折线图

> 此阶段无需任何自定义绘制代码，完全由 TrafficMonitor 渲染。

### Phase 1: 基础设施

1. 新增 `ProgressBarRenderer.h/.cpp` — GDI 进度条绘制辅助类
2. 在 `SettingData` 中添加所有新配置字段
3. 在 `DataManager::LoadConfig()` / `SaveConfig()` 中读写新 INI 键
4. `PowerMon` 重写 `OnMonitorInfo()` 缓存主机数据

### Phase 2: 电池百分比进度条

5. 新增 `BatteryPercentageBarMonItem` 类
6. 在 `PowerMon` 中注册该 Item（`GetItem()` 索引扩展）
7. 测试：验证在任务栏窗口和主窗口中的绘制效果

### Phase 3: 系统占用率进度条

8. 新增 `CpuUsageBarMonItem`、`GpuUsageBarMonItem`、`MemoryUsageBarMonItem`
9. 在 `PowerMon` 中注册
10. 测试：验证 OnMonitorInfo 数据链路

### Phase 4: 功耗进度条

11. 新增 `CpuPowerBarMonItem`、`GpuPowerBarMonItem`（带 TDP 映射）
12. 在 `PowerMon` 中注册
13. 测试：验证 TDP 映射和边界情况

### Phase 5: 选项对话框

14. HardwareSensorPage 资源文件中新增控件
15. 实现控件与 SettingData 的双向绑定
16. 运行时切换显示模式的即时刷新

### Phase 6: 文档与测试

17. 更新 README.md / README.zh.md 配置说明
18. 多 DPI 下的渲染测试（96/120/144/192 DPI）
19. 深色/浅色模式下的颜色对比度验证

## 5. 风险和注意事项

- **GDI 性能**：`DrawItem()` 被高频调用（~1s 间隔），GDI FillRect 足够快，无需双缓冲
- **DPI 缩放**：`GetItemWidth()` 返回值基于 96 DPI，主程序自动放大；`DrawItem()` 中坐标已是缩放后的实际像素
- **任务栏窗口兼容性**：任务栏窗口高度有限（通常 ~20px），进度条高度需自适应
- **向后兼容**：新增 Item 不影响已有 Item 的索引；新 INI 键缺失时使用默认值
- **TDP 映射精度**：用户配置的 TDP 上限可能不准确，建议提供常见 CPU/GPU 预设值
- **ARM64 限制**：CPU/GPU 功耗进度条在 ARM64 上不可用（与现有传感器限制一致）；但 CPU/GPU/内存**占用率**进度条在 ARM64 上可用（数据来自主机 MonitorInfo）

## 6. 开放问题

- [ ] 进度条是否需要动画过渡效果？（当前 1s 刷新间隔下跳变可能突兀）
- [ ] 是否需要支持垂直进度条？（任务栏窗口可能是垂直布局）
- [ ] 是否需要自定义进度条颜色的选项？（当前为固定渐变）
- [ ] TDP 预设值列表：是否内置常见 CPU/GPU 的 TDP 值供下拉选择？
