<!-- markdownlint-disable MD033 MD036 -->
# PowerMon Plugin for TrafficMonitor

<div align="center">

**Monitor battery, CPU, GPU, and platform power sensors in TrafficMonitor's taskbar display**

[![Latest Release](https://img.shields.io/github/v/release/AzulEterno/PowerMonPlugin-For-TrafficMonitor)](https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor/releases/latest)
[![License](https://img.shields.io/badge/license-See%20below-blue)](#license-and-third-party-software) <!-- markdownlint-disable-line MD051 -->

[English](README.md) | [中文](README.zh.md)

</div>

---

PowerMon is a [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) plugin that brings real-time power monitoring to your taskbar. It displays battery status, CPU/GPU power consumption, and smart power summaries using [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) as the backend sensor library.

> **Open-source announcement — 2026-07-26**
>
> This project is fully open source. The complete source code and build files are available here. Issues, bug reports, and contributions are welcome.

## ✨ Features

### Battery Monitoring

- **Power** — real-time battery discharge/charge rate (W)
- **Percentage** — battery state of charge (%)
- **Capacity** — battery energy capacity (mWh)
- **Voltage** — battery voltage (V)
- **Time Remaining** — estimated time to full/empty (hh:mm:ss)

### Hardware Power Sensors

- **CPU Power** — processor package power consumption (W)
- **GPU Power** — graphics card power consumption (W)
- **Smart Power Meter** — adaptive display:
  - On battery power → shows battery discharge rate
  - On AC power → shows combined CPU + GPU power draw

### Data Quality & Filtering

- **Power spike filter** — reject implausible readings above configurable threshold (default: 1000W)
- **Invalid value handling** — show NaN or keep last valid reading when sensors return missing/negative data
- **Configurable precision** — decimal places, unit spacing, and display formatting

### Multi-Architecture Support

- **x64** (recommended) — full-featured build with C++/CLI interop
- **ARM64 / ARM64EC** — native-only builds (LibreHardwareMonitor integration not available on ARM)

## 📦 Installation

### Prerequisites

1. **TrafficMonitor 1.85 or later** — the plugin system was introduced in 1.82; resource usage graphs require 1.85+
2. **Windows 10/11** — x64, ARM64, or ARM64EC

### Steps

1. Download the latest release ZIP for your CPU architecture from the [Releases page](https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor/releases/latest). **x64 is recommended** for full functionality.

2. Close TrafficMonitor completely.

3. Extract the ZIP and copy files:
   - `PowerMonPlugin.dll` → `TrafficMonitor\plugins\`
   - `LibreHardwareMonitorLib.dll` → `TrafficMonitor\` (next to `TrafficMonitor.exe`, **not** in `plugins\`)
   - `INSTALL.txt` — reference for later

4. Start TrafficMonitor. Open **More Functions → Plugin Management** to confirm PowerMon loaded.

5. Enable PowerMon items in **Options → Display Settings → Plugin Items**.

### Troubleshooting

If TrafficMonitor fails to start after installation:

- Remove `PowerMonPlugin.dll` from the `plugins` directory
- Restart TrafficMonitor
- Report the issue with: TrafficMonitor version, Windows version, CPU architecture, and any error message

> **Important:** Keep `PowerMonPlugin.dll` and `LibreHardwareMonitorLib.dll` from the same release. Incompatible library versions can prevent the plugin from loading.

## ⚙️ Configuration

PowerMon reads `PowerMonPlugin.dll.ini` from the same directory as `PowerMonPlugin.dll`. The file is created automatically on first run.

### Power Filter Settings

| Key | Default | Description |
| --- | --- | --- |
| `enable_power_filter` | `0` | `1` rejects power readings above `max_plausible_power_w` |
| `max_plausible_power_w` | `1000` | Upper bound in watts (only used when filter is enabled) |
| `keep_last_valid_value` | `0` | `0` = show NaN for invalid readings; `1` = keep last valid value |

### How the Filter Works

- A sensor with no sample is skipped
- A negative or non-finite sample invalidates the whole device reading
- When `enable_power_filter = 1`, readings above `max_plausible_power_w` are also treated as invalid
- Invalid readings display as NaN (default) or hold the last valid value

### Enabling CPU/GPU Monitoring

Open **Options → Plugin Options → PowerMon** to enable/disable CPU and GPU power monitoring. Changes take effect immediately.

## 🔨 Build from Source

### Build Prerequisites

- **Windows 10/11**
- **Visual Studio 2022** or **Build Tools** with:
  - C++ desktop workload (MSVC v143)
  - Windows SDK
  - .NET Framework 4.7.2 targeting pack

### Build with PowerShell Script

```powershell
.\shell\build-powermon.ps1 -Configuration Release -Platform x64
```

Supported platforms: `x64`, `Win32`, `ARM64`, `ARM64EC`

### Build with Visual Studio

1. Open `TrafficMonitorPlugins.sln`
2. Select the `PowerMonPlugin` project
3. Choose configuration: `Release`
4. Choose platform: `Win32`, `x64`, `ARM64`, or `ARM64EC`
5. Build → Build Solution

Output: `bin\PowerMonPlugin\Release\<platform>\PowerMonPlugin.dll`

### LibreHardwareMonitor Version

This plugin is tested and aligned with **LibreHardwareMonitor 0.9.6**. The compatible DLL is bundled in `PowerMonPlugin/lib/`. Using a different version may cause compatibility issues due to sensor type enum changes.

## ❓ FAQ

### Why doesn't ARM64 support CPU/GPU hardware monitoring?

Hardware power monitoring (CPU and GPU sensors) relies on [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor), which in turn uses the **WinRing0** kernel-mode driver to directly access hardware registers (MSRs, I/O ports) for reading power and thermal sensors. WinRing0 is currently only available for **x86/x64** architectures — there is no ARM64 build of the driver.

This means on ARM64 / ARM64EC Windows devices:

- **Battery monitoring** works fully — it uses Windows power APIs (`GetSystemPowerStatus`, `SetupDevicePower`, etc.) and does not require WinRing0.
- **CPU/GPU power monitoring** is unavailable — the LibreHardwareMonitor .NET assembly cannot access the hardware sensor registers without the WinRing0 driver, so those sensor items are not registered.

The situation will improve when an ARM-compatible driver backend becomes available. You can track progress on:

- [LibreHardwareMonitor Issue #1162 — Support for ARM64](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor/issues/1162)
- [LibreHardwareMonitor Issue #984 — Updated WinRing0 driver](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor/issues/984)

## 📝 What's New

### Recent Improvements

**2026-11-06** — Power spike filtering and data quality improvements

- Added configurable power filter to reject implausible readings (PR #44)
- Option to keep last valid value when sensors return invalid data
- Aligned sensor types with LibreHardwareMonitor 0.9.6
- Fixed temperature unit display (°C) to be encoding-independent

**2026-07-26** — Open-source release

- Full source code and build files published
- Documentation for installation, configuration, and building from source

**Earlier**

- Battery monitoring: power, percentage, capacity, voltage, time remaining
- CPU and GPU power monitoring via LibreHardwareMonitor
- Smart Power Meter: adaptive display based on power source
- Configurable display formatting: precision, units, spacing
- Multi-architecture support: x64, ARM64, ARM64EC
- Scrollable options pages for better usability

## 🤝 Contributing

Contributions are welcome! Areas where help is especially useful:

- Bug reports and feature requests
- Documentation improvements
- Testing on different hardware configurations
- ARM64/ARM64EC builds with LibreHardwareMonitor interop (requires .NET on ARM)

Please open an issue or pull request on GitHub.

## 📄 License and Third-Party Software

This repository includes code and assets originating from the TrafficMonitor plugin ecosystem. Preserve the existing notices when redistributing or modifying that code.

**LibreHardwareMonitorLib.dll** is third-party software distributed under the [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/). Its source code and license are available from the [LibreHardwareMonitor project](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor).

## 🔗 Links

- [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) — the host application
- [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) — hardware sensor library
- [Releases](https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor/releases) — download pre-built binaries

---

<div align="center">

**Made with ❤️ for the TrafficMonitor community**

</div>
