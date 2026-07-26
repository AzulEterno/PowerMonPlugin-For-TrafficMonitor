# PowerMon Plugin for TrafficMonitor

PowerMon is a [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) plug-in for displaying battery, CPU, GPU, platform, and other power-related sensor information.

> **Open-source announcement — 2026-07-26**
>
> Starting today, this project is fully open source. The complete source code and build files are available here. Issues, bug reports, and contributions are welcome.

## Features

- Battery power, percentage, capacity, voltage, and remaining-time items
- CPU, GPU, and platform power monitoring where supported by the hardware
- Configurable value precision, units, spacing, and display formatting
- Smart power display: battery discharge power on battery, or combined CPU/GPU power when connected to external power

## Install a release

1. Install a compatible version of [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor). Its plug-in system is available in TrafficMonitor 1.82 and later.
2. Download the ZIP for your CPU architecture from this repository’s **Releases** page. **x64 is recommended**; ARM64 and ARM64EC builds are less thoroughly tested.
3. Close TrafficMonitor, then copy `PowerMonPlugin.dll` into the `plugins` directory next to `TrafficMonitor.exe`.
4. If the release includes `LibreHardwareMonitorLib.dll`, copy that file next to `TrafficMonitor.exe` — **not** into `plugins`. Keep the files from the same release together; incompatible library versions can prevent the plug-in from loading.
5. Start TrafficMonitor. Open **More Functions → Plugin Management** to confirm that PowerMon loaded, then enable its items in the display settings.

If TrafficMonitor does not start after installation, remove `PowerMonPlugin.dll` from its `plugins` directory and restart it. Please include your TrafficMonitor version, Windows version, CPU architecture, and any error message when reporting an issue.

## Build from source

### Prerequisites

- Windows
- Visual Studio 2022 or Visual Studio Build Tools with the C++ desktop workload (MSVC v143)
- Windows SDK
- .NET Framework 4.7.2 targeting pack

The repository includes the compatible `LibreHardwareMonitorLib.dll` under `PowerMonPlugin/lib`.

Build the default x64 release from PowerShell:

```powershell
.\shell\build-powermon.ps1 -Configuration Release -Platform x64
```

Or open `TrafficMonitorPlugins.sln` in Visual Studio, select the `PowerMonPlugin` project, choose `Release` and the target platform (`Win32`, `x64`, `ARM64`, or `ARM64EC`), and build it. The output is written to:

```text
bin\PowerMonPlugin\Release\<platform>\PowerMonPlugin.dll
```

## License and third-party software

This repository includes code and assets originating from the TrafficMonitor plug-in ecosystem. Preserve the existing notices when redistributing or modifying that code.

`LibreHardwareMonitorLib.dll` is third-party software distributed under the Mozilla Public License 2.0. Its source code and license are available from the [LibreHardwareMonitor project](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor).

## 历史说明

**2026-07-26：本项目从今天起全面开源。** 源代码、构建文件和发布流程现已公开，欢迎提交 Issue、Bug 报告和贡献。
