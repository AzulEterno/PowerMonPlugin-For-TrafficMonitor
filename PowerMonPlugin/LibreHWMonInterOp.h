#include "pch.h"
#include "DataManager.h"
#pragma once

using namespace LibreHardwareMonitor::Hardware;

namespace InterOpLibreHWMon {

	public ref class PowerSenUpdateVisitor : IVisitor
	{
	public:
		virtual void VisitComputer(IComputer^ computer);
		virtual void VisitHardware(IHardware^ hardware);
		virtual void VisitSensor(ISensor^ sensor);
		virtual void VisitParameter(IParameter^ parameter);
	};

	ref class LibreHWMonInterOpProxy
	{
	private:
		static LibreHWMonInterOpProxy^ _instance{};

	protected:
		bool cpu_mon_enable, gpu_mon_enable;


		Computer^ p_IO_Computer;
		PowerSenUpdateVisitor^ updateVisitor{};
	public:
		static LibreHWMonInterOpProxy^ Instance()
		{
			if (_instance == nullptr)
			{
				//Alter initalization maybe?
				_instance = gcnew LibreHWMonInterOpProxy();
			}
			return _instance;
		}

		//UpdateVisitor^ updateVisitor{};


		~LibreHWMonInterOpProxy() {
			ReleaseResources();
		}

		int SyncFromSettingData(SettingData setting_data) {

			cpu_mon_enable = setting_data.enable_cpu_monitor;
			gpu_mon_enable = setting_data.enable_gpu_monitor;

			return 0;

		}

		int PrepareInterOpComputorObject() {
			ReleaseResources();
			p_IO_Computer = gcnew Computer();

			return 0;
		}

		int ReleaseResources() {
			if (p_IO_Computer) {
				p_IO_Computer->Close();
				p_IO_Computer = nullptr;
			}

			return 0;
		}

		int SetInterOpComputorSettings() {
			p_IO_Computer->IsCpuEnabled = cpu_mon_enable;
			p_IO_Computer->IsGpuEnabled = gpu_mon_enable;

			p_IO_Computer->IsControllerEnabled = false;
			p_IO_Computer->IsBatteryEnabled = false;
			p_IO_Computer->IsMemoryEnabled = false;
			p_IO_Computer->IsMotherboardEnabled = false;
			p_IO_Computer->IsStorageEnabled = false;
			p_IO_Computer->IsPsuEnabled = false;

			return 0;
		}

	};

};