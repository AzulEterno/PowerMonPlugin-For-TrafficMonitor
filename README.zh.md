<!-- markdownlint-disable MD033 MD036 -->
# PowerMon — TrafficMonitor 电源监控插件

<div align="center">

**在 TrafficMonitor 任务栏显示中实时监控电池、CPU、GPU 及平台功耗传感器**

[![最新版本](https://img.shields.io/github/v/release/AzulEterno/PowerMonPlugin-For-TrafficMonitor)](https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor/releases/latest)
[![许可证](https://img.shields.io/badge/license-See%20below-blue)](#许可证及第三方软件) <!-- markdownlint-disable-line MD051 -->

[English](README.md) | [中文](README.zh.md)

</div>

---

PowerMon 是一款 [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) 插件，可在任务栏中实时显示电源监控数据。它以 [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) 作为后端传感器库，展示电池状态、CPU/GPU 功耗及智能功耗汇总信息。

> **开源公告 — 2026-07-26**
>
> 本项目已完全开源。完整的源代码和构建文件均在此仓库中。欢迎提交 Issue、Bug 报告及贡献代码。

## ✨ 功能特性

### 电池监控

- **功率** — 实时电池充放电功率（W）
- **电量百分比** — 电池荷电状态（%）
- **容量** — 电池能量容量（mWh）
- **电压** — 电池电压（V）
- **剩余时间** — 预计充满/耗尽时间（hh:mm:ss）

### 硬件功耗传感器

- **CPU 功耗** — 处理器封装功耗（W）
- **GPU 功耗** — 显卡功耗（W）
- **智能功率表** — 自适应显示：
  - 使用电池供电时 → 显示电池放电功率
  - 使用交流电源时 → 显示 CPU + GPU 合计功耗

### 数据质量与过滤

- **功率尖峰过滤** — 拒绝超过可配置阈值的异常读数（默认：1000W）
- **无效值处理** — 传感器返回缺失/负值数据时，显示 NaN 或保留上次有效读数
- **可配置精度** — 小数位数、单位间距及显示格式

### 多架构支持

- **x64**（推荐）— 完整功能构建，支持 C++/CLI 互操作
- **ARM64 / ARM64EC** — 仅原生构建（LibreHardwareMonitor 集成暂不可用于 ARM 平台）

## 📦 安装指南

### 前置条件

1. **TrafficMonitor 1.82 或更高版本** — 插件系统自 1.82 版本引入
2. **Windows 10/11** — 支持 x64、ARM64 或 ARM64EC

### 安装步骤

1. 从 [Releases 页面](https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor/releases/latest) 下载适合您 CPU 架构的最新版本 ZIP 包。**推荐使用 x64 版本**以获得完整功能。

2. 完全关闭 TrafficMonitor。

3. 解压 ZIP 并复制文件：
   - `PowerMonPlugin.dll` → `TrafficMonitor\plugins\`
   - `LibreHardwareMonitorLib.dll` → `TrafficMonitor\`（与 `TrafficMonitor.exe` 同级目录，**不要**放入 `plugins\`）
   - `INSTALL.txt` — 安装说明备忘

4. 启动 TrafficMonitor。打开 **更多功能 → 插件管理** 确认 PowerMon 已加载。

5. 在 **选项 → 显示设置 → 插件项目** 中启用 PowerMon 各项显示。

### 故障排除

如果安装后 TrafficMonitor 无法启动：

- 从 `plugins` 目录中移除 `PowerMonPlugin.dll`
- 重启 TrafficMonitor
- 提交 Issue 并附上：TrafficMonitor 版本、Windows 版本、CPU 架构及错误信息

> **重要提示：** 请确保 `PowerMonPlugin.dll` 和 `LibreHardwareMonitorLib.dll` 来自同一发布版本。不兼容的库版本可能导致插件无法加载。

## ⚙️ 配置说明

PowerMon 的配置文件为 `PowerMonPlugin.dll.ini`，存放于 `PowerMonPlugin.dll` 同目录下。该文件在首次运行时自动创建，包含所有配置项的默认值。

配置方式有两种：**图形界面**（推荐）和**手动编辑 INI 文件**。

### 通过图形界面配置

在 TrafficMonitor 中打开 **选项 → 插件选项 → PowerMon**，进入设置对话框。对话框包含三个选项页：

**选项页 1 — 单位设置**

自定义显示中使用的单位标签和格式参数：

| 配置项 | 默认值 | 说明 |
| --- | --- | --- |
| Watts（功率单位） | `W` | 功率显示的单位标签，可改为 `瓦` 等自定义文字 |
| Watts × Hour（能量单位） | `Wh` | 电池容量显示的单位标签 |
| Voltage（电压单位） | `V` | 电压显示的单位标签 |
| Hour（小时单位） | `H` | 剩余时间中"小时"部分的标签 |
| Minute（分钟单位） | `M` | 剩余时间中"分钟"部分的标签 |
| NaN（无效值占位符） | `NaN` | 传感器无有效数据时的显示文字，可改为 `--` 或 `N/A` |
| 数值与单位间距 | `1` | 数值与单位之间的空格数量（0 = 紧贴，如 `12.5W`；1 = `12.5 W`） |
| 表显示最大小数位数 | `2` | 自适应精度的最大小数位数。实际位数根据数值大小自动调整：数值越大，小数位越少 |

> **提示：** 单位标签长度有限制（内部缓冲区上限），过长的文字会被截断。建议保持简洁（1–3 个字符）。

**选项页 2 — 硬件监控**

启用/禁用传感器及配置数据过滤：

| 配置项 | 默认值 | 说明 |
| --- | --- | --- |
| Enable CPU Monitor | 关闭 | 勾选以启用 CPU 功耗监控。仅在 x64 构建中可用；ARM 构建时此选项不可操作 |
| Enable GPU Monitor | 关闭 | 勾选以启用 GPU 功耗监控。仅在 x64 构建中可用 |
| Max Power (W) 功率过滤 | 关闭 | 勾选启用后，超过下方阈值的读数将被视为异常并过滤 |
| 功率上限阈值 | `1000` | 合理功率上限（瓦特），有效范围 10–100000。超出范围的输入会被自动重置为 1000 |
| Keep Last Valid Value | 关闭 | 关闭 = 无效读数显示 NaN 占位符；开启 = 保留并显示最后一次有效读数 |

该页面底部还有一个**传感器树视图**，实时显示电池、CPU、GPU 各传感器的当前采样数据，方便确认传感器是否正常工作。点击 **Refresh** 按钮可刷新传感器列表。

**选项页 3 — 信息**

显示版本信息（Windows 版本、PowerMon 版本、WinRT 状态）及调试选项。点击 **Repo URL** 可在浏览器中打开项目仓库。

### 手动编辑 INI 文件

直接编辑 `PowerMonPlugin.dll.ini` 文件，所有配置项位于 `[config]` 节下：

```ini
[config]
; === 硬件监控开关 ===
enable_cpu_monitor=0          ; 1=启用 CPU 功耗监控（仅 x64 构建有效）
enable_gpu_monitor=0          ; 1=启用 GPU 功耗监控（仅 x64 构建有效）

; === 功率尖峰过滤 ===
enable_power_filter=0         ; 1=启用过滤，拒绝超过 max_plausible_power_w 的读数
max_plausible_power_w=1000    ; 合理功率上限（瓦特），范围 10–100000
keep_last_valid_value=0       ; 0=无效读数显示 NaN；1=保留上次有效值

; === 显示格式 ===
default_value_unit_space=1             ; 数值与单位之间的空格数
default_max_adaptive_decimal_places=2  ; 自适应精度最大小数位数

; === 自定义单位标签 ===
pwr_unit_str=W                 ; 功率单位
electric_capacity_unit_str=Wh  ; 能量/容量单位
electric_voltage_unit_str=V    ; 电压单位
hour_unit_str=H                ; 时间 — 小时标签
minute_unit_str=M              ; 时间 — 分钟标签
nan_str=NaN                    ; 无效/缺失数据时的占位符
```

编辑后重启 TrafficMonitor 或重新打开插件选项对话框使其生效。

> **注意：** 旧版本配置文件中可能缺少 `enable_power_filter` 键。如果该键缺失但 `max_plausible_power_w` 已存在且值在有效范围内，过滤器会被隐式启用。

### 过滤逻辑详解

功率过滤用于排除传感器异常读数（如瞬间飙升到数千瓦的错误采样）。过滤判断按以下顺序执行：

1. **无采样** — 传感器尚未采集到任何数据，该传感器被跳过，不影响其他传感器
2. **负值或非有限值**（如 -1、Infinity、NaN）— 整个设备的读数被判定为无效
3. **超过上限**（仅在 `enable_power_filter=1` 时）— 功率值超过 `max_plausible_power_w` 的读数同样被视为无效
4. **无效读数的显示** — `keep_last_valid_value=0` 时显示 `nan_str` 配置的占位符（默认 `NaN`）；`keep_last_valid_value=1` 时继续显示上一次有效的采样值

### 启用 CPU/GPU 监控

> **前提条件：** CPU/GPU 功耗监控仅在 **x64 构建** 中可用。ARM64/ARM64EC 构建因缺少 .NET 互操作支持而无法使用此功能。

方法一：打开 TrafficMonitor **选项 → 插件选项 → PowerMon → 硬件监控**，勾选 "Enable CPU Monitor" 和/或 "Enable GPU Monitor"，点击 **确认**。

方法二：编辑 INI 文件，设置 `enable_cpu_monitor=1` 和/或 `enable_gpu_monitor=1`，重启 TrafficMonitor。

启用后，CPU 功耗和 GPU 功耗将作为独立监控项出现在 TrafficMonitor 的插件项目列表中，可在 **选项 → 显示设置 → 插件项目** 中选择显示。

## 🔨 从源代码构建

### 构建前置条件

- **Windows 10/11**
- **Visual Studio 2022** 或 **Build Tools**，需安装：
  - C++ 桌面工作负载（MSVC v143）
  - Windows SDK
  - .NET Framework 4.7.2 目标包

### 使用 PowerShell 脚本构建

```powershell
.\shell\build-powermon.ps1 -Configuration Release -Platform x64
```

支持的平台：`x64`、`Win32`、`ARM64`、`ARM64EC`

### 使用 Visual Studio 构建

1. 打开 `TrafficMonitorPlugins.sln`
2. 选择 `PowerMonPlugin` 项目
3. 选择配置：`Release`
4. 选择平台：`Win32`、`x64`、`ARM64` 或 `ARM64EC`
5. 生成 → 生成解决方案

输出路径：`bin\PowerMonPlugin\Release\<platform>\PowerMonPlugin.dll`

### LibreHardwareMonitor 版本

本插件基于 **LibreHardwareMonitor 0.9.6** 测试并对齐。兼容的 DLL 已打包在 `PowerMonPlugin/lib/` 目录中。使用不同版本可能因传感器类型枚举变更而导致兼容性问题。

## ❓ 常见问题

### 为什么 ARM64 平台不支持 CPU/GPU 硬件监控？

硬件功耗监控（CPU 和 GPU 传感器）依赖 [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor)，而后者使用 **WinRing0** 内核模式驱动来直接访问硬件寄存器（MSR、I/O 端口），以读取功耗和温度传感器数据。WinRing0 目前仅提供 **x86/x64** 架构的构建版本，尚无 ARM64 版本。

这意味着在 ARM64 / ARM64EC Windows 设备上：

- **电池监控** 完全可用 — 它使用 Windows 电源管理 API（`GetSystemPowerStatus`、`SetupDevicePower` 等），无需 WinRing0 驱动。
- **CPU/GPU 功耗监控** 不可用 — 缺少 WinRing0 驱动，LibreHardwareMonitor .NET 程序集无法访问硬件传感器寄存器，因此这些传感器项目不会被注册。

当兼容 ARM 的驱动后端可用后，此限制将会解除。可关注以下 Issue 了解进展：

- [LibreHardwareMonitor Issue #1162 — ARM64 支持](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor/issues/1162)
- [LibreHardwareMonitor Issue #984 — 更新 WinRing0 驱动](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor/issues/984)

## 📝 更新日志

### 近期改进

**2026-11-06** — 功率尖峰过滤与数据质量改进

- 新增可配置功率过滤器，拒绝异常读数（PR #44）
- 新增传感器返回无效数据时保留上次有效值的选项
- 传感器类型枚举与 LibreHardwareMonitor 0.9.6 对齐
- 修复温度单位显示（°C），消除编码依赖

**2026-07-26** — 开源发布

- 完整源代码和构建文件发布
- 安装、配置和源码构建文档

**早期版本**

- 电池监控：功率、百分比、容量、电压、剩余时间
- 通过 LibreHardwareMonitor 实现 CPU 和 GPU 功耗监控
- 智能功率表：根据供电来源自适应显示
- 可配置显示格式：精度、单位、间距
- 多架构支持：x64、ARM64、ARM64EC
- 可滚动选项页面，提升可用性

## 🤝 参与贡献

欢迎贡献！以下方面尤其需要帮助：

- Bug 报告和功能请求
- 文档改进
- 不同硬件配置上的测试
- ARM64/ARM64EC 平台的 LibreHardwareMonitor 互操作构建（需要 ARM 上的 .NET 支持）

请在 GitHub 上提交 Issue 或 Pull Request。

## 📄 许可证及第三方软件

本仓库包含源自 TrafficMonitor 插件生态系统的代码和资源。在再分发或修改时，请保留现有声明。

**LibreHardwareMonitorLib.dll** 是基于 [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/) 许可的第三方软件。其源代码和许可证可在 [LibreHardwareMonitor 项目](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) 中获取。

## 🔗 相关链接

- [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) — 宿主应用程序
- [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) — 硬件传感器库
- [Releases](https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor/releases) — 下载预构建二进制文件

---

<div align="center">

**为 TrafficMonitor 社区倾心打造 ❤️**

</div>
