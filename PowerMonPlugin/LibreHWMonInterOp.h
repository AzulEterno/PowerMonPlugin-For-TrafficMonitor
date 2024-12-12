#include "pch.h"
#include "DataManager.h"
#include "ValueUnitStringFormatter.h"

#pragma once

#if WINRT_USE_FLAG
using namespace System;
using namespace LibreHardwareMonitor::Hardware;


//using namespace Windows::Foundation::Collections;
namespace InterOpLibreHWMon {
	//将CRL的String类型转换成C++的std::wstring类型
	static std::wstring ClrStringToStdWstring(System::String^ str)
	{
		if (str == nullptr)
		{
			return std::wstring();
		}
		else
		{
			const wchar_t* chars = (const wchar_t*)(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str)).ToPointer();
			std::wstring os = chars;
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
			return os;
		}
	}




	//自定义Visitor成员确保只便利我们想要的内容
	public ref class PowerSensorUpdateVisitor : IVisitor
	{

	protected:
		bool _cpu_mon_enable, _gpu_mon_enable;
	public:
		bool SetCpuMonitorEnable(bool enable) {
			_cpu_mon_enable = enable;
			return true;
		}
		bool SetGpuMonitorEnable(bool enable) {
			_gpu_mon_enable = enable;
			return true;
		}

		virtual void VisitComputer(IComputer^ computer);
		virtual void VisitHardware(IHardware^ hardware);
		virtual void VisitSensor(ISensor^ sensor);
		virtual void VisitParameter(IParameter^ parameter);
	};

	//托管类成员， 无法承载C++自有数据结构
	public ref class LibreHWMonInterOpProxy
	{
	private:
		static LibreHWMonInterOpProxy^ __instance{};

	protected:
		bool _cpu_mon_enable, _gpu_mon_enable;



		Computer^ _p_IO_Computer{};
		PowerSensorUpdateVisitor^ _power_sensor_update_visitor{};
	public:
		static LibreHWMonInterOpProxy^ Instance()
		{
			if (__instance == nullptr)
			{
				//Alter initalization maybe?
				__instance = gcnew LibreHWMonInterOpProxy(true);
			}
			return __instance;
		}

		//_power_sensor_update_visitor^ _power_sensor_update_visitor{};

		PowerSensorUpdateVisitor^ GetUpdateVistor() {
			return _power_sensor_update_visitor;
		}

		Computer^ GetComputorInfoInstance() {
			return _p_IO_Computer;
		}


		int CallUpdateComputorInfo() {

			_p_IO_Computer->Accept(_power_sensor_update_visitor);
			return 0;
		}

		LibreHWMonInterOpProxy(bool auto_init) {
			if (auto_init) {
				PrepareInterOpObjects();
			}
		}

		~LibreHWMonInterOpProxy() {
			ReleaseResources();
		}

		int SyncFromSettingData(const SettingData& setting_data) {

			_cpu_mon_enable = setting_data.enable_cpu_monitor;
			_gpu_mon_enable = setting_data.enable_gpu_monitor;

			_power_sensor_update_visitor->SetCpuMonitorEnable(_cpu_mon_enable);
			_power_sensor_update_visitor->SetGpuMonitorEnable(_gpu_mon_enable);



			return SetInterOpComputorSettings();

		}

		int PrepareInterOpObjects() {
			ReleaseResources();
			_p_IO_Computer = gcnew Computer();
			_power_sensor_update_visitor = gcnew PowerSensorUpdateVisitor();

			return 0;
		}

		int ReleaseResources() {
			if (_p_IO_Computer) {
				_p_IO_Computer->Close();
				_p_IO_Computer = nullptr;
			}

			if (_power_sensor_update_visitor) {
				delete _power_sensor_update_visitor;
				_power_sensor_update_visitor = nullptr;
			}

			return 0;
		}

		int SetInterOpComputorSettings() {
			if (!_p_IO_Computer) {

				return -1;
			}

			_p_IO_Computer->IsCpuEnabled = _cpu_mon_enable;
			_p_IO_Computer->IsGpuEnabled = _gpu_mon_enable;

			_p_IO_Computer->IsControllerEnabled = false;
			_p_IO_Computer->IsBatteryEnabled = false;
			_p_IO_Computer->IsMemoryEnabled = false;
			_p_IO_Computer->IsMotherboardEnabled = false;
			_p_IO_Computer->IsStorageEnabled = false;
			_p_IO_Computer->IsPsuEnabled = false;




			return 0;
		}

		int OpenInterOpComputorObject() {
			if (!_p_IO_Computer) {

				return -1;
			}

			_p_IO_Computer->Open();

			return 0;
		}
	};

	//Exact copy of items from LibreHWMonInterOp
	public enum SensorTypeCopy
	{
		Voltage, // V
		Current, // A
		Power, // W
		Clock, // MHz
		Temperature, // °C
		Load, // %
		Frequency, // Hz
		Fan, // RPM
		Flow, // L/h
		Control, // %
		Level, // %
		Factor, // 1
		Data, // GB = 2^30 Bytes
		SmallData, // MB = 2^20 Bytes
		Throughput, // B/s
		TimeSpan, // Seconds 
		Energy, // milliwatt-hour (mWh)
		Noise, // dBA
		Humidity // %
	};
	static std::wstring GetSensorTypeUnitString(SensorTypeCopy sensorType) {
		switch (sensorType) {
		case SensorTypeCopy::Voltage:
			return L"V";
		case SensorTypeCopy::Current:
			return L"A";
		case SensorTypeCopy::Power:
			return L"W";
		case SensorTypeCopy::Clock:
			return L"MHz";
		case SensorTypeCopy::Temperature:
			return L"°C";
		case SensorTypeCopy::Load:
		case SensorTypeCopy::Control:
		case SensorTypeCopy::Level:
			return L"%";
		case SensorTypeCopy::Frequency:
			return L"Hz";
		case SensorTypeCopy::Fan:
			return L"RPM";
		case SensorTypeCopy::Flow:
			return L"L/h";
		case SensorTypeCopy::Factor:
			return L"1";
		case SensorTypeCopy::Data:
			return L"GB";
		case SensorTypeCopy::SmallData:
			return L"MB";
		case SensorTypeCopy::Throughput:
			return L"B/s";
		case SensorTypeCopy::TimeSpan:
			return L"Seconds";
		case SensorTypeCopy::Energy:
			return L"mWh";
		case SensorTypeCopy::Noise:
			return L"dBA";
		case SensorTypeCopy::Humidity:
			return L"%";
		default:
			return L"Unknown";
		}
	}

	class HardwareRepresentiveBase;

	public class SensorRepresentiveBase {

	protected:
		UINT64 _update_iter_index = 0;
		std::wstring _identity = L"";
		std::wstring _name = L"";

		SensorTypeCopy _sensor_type;
		bool _has_value = false;
		double _value = -1;
		HardwareRepresentiveBase* _p_owner = nullptr;
	public:
		static std::set<std::wstring> FilterSensorWithFunction(
			const std::map<std::wstring, SensorRepresentiveBase> sensors,
			std::function<bool(const SensorRepresentiveBase&)> select_function
		) {
			std::set<std::wstring> valid_indexes = {};
			for (auto& [key, sensor] : sensors) {
				if (sensor.IsValid() && select_function(sensor)) {
					valid_indexes.insert(key);
				}
			}

			return valid_indexes;
		}

		const static std::function<bool(const SensorRepresentiveBase&)> select_power_sensor_fun;

		SensorRepresentiveBase() {};
		// Default constructor
		SensorRepresentiveBase(HardwareRepresentiveBase* p_owner, std::wstring identity,
			std::wstring name, SensorTypeCopy sensor_type, double value = -1, INT64 update_iter_index = 0)
			:_p_owner(p_owner),
			_identity(identity),
			_sensor_type(sensor_type),
			_name(name),
			_value(value), _update_iter_index(update_iter_index) {
			if (value != -1) {
				_has_value = true;
			}
		}

		bool IsValid() const {
			return _identity.length() > 0 && _p_owner != nullptr;
		}

		bool HasValue() const {
			return _has_value;
		}

		void SetValue(bool has_value) {
			_has_value = has_value;
		}

		void SetValue(const double& new_val, const INT64 update_index) {
			_has_value = true;
			_value = new_val;
			_update_iter_index = update_index;
		}

		INT64 GetUpdateIterIndex()const {
			return _update_iter_index;
		}

		SensorTypeCopy GetSensorType() const {
			return _sensor_type;
		}

		double GetValue() const {
			if (!HasValue()) {
				return -1;
			}
			return _value;
		}
		std::wstring GetId() const {
			return _identity;
		}
		std::wstring GetName() const {
			return _name;
		}
		bool operator==(const SensorRepresentiveBase& other) const;
		bool IsSensorMatch(const std::wstring& identity, SensorTypeCopy sensor_type) const;

	};


	public class HardwareRepresentiveBase {
	protected:
		UINT64 _update_iter_index = 0;
		std::wstring _identity = L"";
		std::wstring _name = L"";

		std::map<std::wstring, SensorRepresentiveBase> _sensors = {};

		bool _has_package_power_sensor = false;

	public:

		bool GetHasPackagePowerSensor() const {
			return _has_package_power_sensor;
		}

		void SetHasPackagePowerSensor(bool has_p_sensor) {
			_has_package_power_sensor = has_p_sensor;
		}


		INT64 GetUpdateIterIndex()const {
			return _update_iter_index;
		}

		void SetUpdateIterIndex(INT64 update_iter_index) {
			_update_iter_index = update_iter_index;
		}

		HardwareRepresentiveBase() {};
		// Constructor with required ID and optional name
		HardwareRepresentiveBase(const std::wstring& identity, const std::wstring& name = L"",
			UINT64 update_iter_index = 0)
			: _identity(identity), _name(name), _update_iter_index(update_iter_index) {}

		bool IsValid()const {
			return _identity.length() > 0;
		}

		bool CheckSensorIdExist(const std::wstring check_id)const {
			return _sensors.contains(check_id);
		}

		std::map<std::wstring, SensorRepresentiveBase>& GetSensorsMap() {
			return _sensors;
		}

		std::map<std::wstring, SensorRepresentiveBase> GetSensorsMapReadOnly() const {
			return _sensors;
		}

		std::vector<SensorRepresentiveBase> SelectSensorsByKey(
			const std::set<std::wstring> keys)const {
			std::vector<SensorRepresentiveBase> result = {};

			for (auto& [sensor_key, sensor] : _sensors) {
				if (keys.contains(sensor_key)) {
					result.push_back(sensor);
				}
			}


			return result;
		}

		std::wstring GetId() const {
			return _identity;
		}
		std::wstring GetName() const {
			return _name;
		}
		bool operator==(const HardwareRepresentiveBase& other) const;
		std::set<std::wstring> GetSelectedSensorKeyList(
			std::function<bool(const SensorRepresentiveBase&)> select_function
		) const {
			return SensorRepresentiveBase::FilterSensorWithFunction(_sensors, select_function);

		}

	};



	public class CPUHardwareRep : public HardwareRepresentiveBase {
	protected:
		UINT64 _thread_count = 0;
		UINT64 _phy_core_count = 0;
		INT64 _cpu_id = 0;
	public:
		CPUHardwareRep() {};
		// Constructor with required ID and optional name
		CPUHardwareRep(const std::wstring& identity, const std::wstring& name = L"",const INT64& cpu_id = 0, UINT64 thread_count = 0,
			UINT64 phy_core_count = 0, UINT64 update_iter_index = 0)
			: HardwareRepresentiveBase(identity, name, update_iter_index),
			_thread_count(thread_count), _phy_core_count(phy_core_count) {

			
		}

		INT64 GetCpuIndex() const {

			return _cpu_id;
		}
	};

	public class GPUHardwareRep : public HardwareRepresentiveBase {
	protected:
		UINT64 _VRAM_size = 0;

		INT64 _intergated_gpu_cpu_id = -1;

	public:
		GPUHardwareRep() {};
		// Constructor with required ID and optional name
		GPUHardwareRep(const std::wstring& identity, const std::wstring& name = L"",
			UINT64 VRAM_size = 0, INT64 intergated_gpu_cpu_id = -1, UINT64 update_iter_index = 0)
			: HardwareRepresentiveBase(identity, name, update_iter_index), _VRAM_size(VRAM_size), _intergated_gpu_cpu_id(intergated_gpu_cpu_id) {
		}


		INT64 GetIntergatedCpuId() const {
			return _intergated_gpu_cpu_id;
		}
		INT64 GetVRamSize() const {
			return _VRAM_size;
		}

	};

	//非托管类，可以自由使用C++数据结构
	public class HardwareSensorDataProvider {
	protected:
		std::map<std::wstring, CPUHardwareRep>
			_cpu_map = {};
		std::map<std::wstring, GPUHardwareRep> _gpu_map = {};

		//Structure
		//
		UINT64 _update_index = 0;

		INT64 _cpu_id_counter = 0;
		float _cpu_total_pwr = 0, _gpu_total_pwr = 0;

	public:

		std::map<std::wstring, CPUHardwareRep>& GetCPUSensorDataMap() {
			return _cpu_map;
		}
		std::map<std::wstring, GPUHardwareRep>& GetGPUSensorDataMap() {
			return _gpu_map;
		}



		CPUHardwareRep* GetCPUHardwareRepByIndex(const INT64& index) {
			for (auto& pair : _cpu_map) {
				if (pair.second.GetCpuIndex() == index) {

					return &pair.second;
				}
			}

			return  nullptr;
		}

		HardwareSensorDataProvider(bool auto_load_settings = true) {
			if (auto_load_settings) {
				SyncFromSettingData(g_data.m_setting_data);
				LibreHWMonInterOpProxy::Instance()->OpenInterOpComputorObject();
			}

		}
		~HardwareSensorDataProvider() {

		}

		int SyncFromSettingData(const SettingData& setting_data) {

			LibreHWMonInterOpProxy::Instance()->SyncFromSettingData(setting_data);


			return 0;

		}

		int GetLoadedCPUCount() {
			return _cpu_map.size();
		}

		bool ResetAllRecordedValues() {
			std::map<std::wstring, CPUHardwareRep>::iterator
				cpu_sensor_iterator;
			for (cpu_sensor_iterator = _cpu_map.begin();
				cpu_sensor_iterator != _cpu_map.end();
				cpu_sensor_iterator++)
			{
				std::map<std::wstring, SensorRepresentiveBase>::iterator sensor_iter;
				for (sensor_iter = cpu_sensor_iterator->second.GetSensorsMap().begin();
					sensor_iter != cpu_sensor_iterator->second.GetSensorsMap().end();
					sensor_iter++) {
					//sensor_iter->second = -1;

				}
			}

			//delete& cpu_sensor_iterator;


			std::map<std::wstring, GPUHardwareRep>::iterator
				gpu_sensor_iterator;
			for (gpu_sensor_iterator = _gpu_map.begin();
				gpu_sensor_iterator != _gpu_map.end();
				gpu_sensor_iterator++)
			{
				std::map<std::wstring, SensorRepresentiveBase>::iterator sensor_iter;
				for (sensor_iter = gpu_sensor_iterator->second.GetSensorsMap().begin();
					sensor_iter != gpu_sensor_iterator->second.GetSensorsMap().end();
					sensor_iter++) {
					//sensor_iter->second = -1;

				}
			}

			_cpu_total_pwr = 0, _gpu_total_pwr = 0;
			return true;
		}

		bool RecordCPUSensorsValueByHardware(IHardware^ hw, std::map<std::wstring, CPUHardwareRep>& val_store) {
			auto sensor_list = hw->Sensors;

			std::wstring hwid = ClrStringToStdWstring(hw->Identifier->ToString());

			if (!val_store.contains(hwid)) {
				val_store[hwid] = CPUHardwareRep(hwid, ClrStringToStdWstring(hw->Name), _cpu_id_counter++);
			}

			val_store[hwid].SetUpdateIterIndex(_update_index);

			auto& hw_rep = val_store[hwid];
			for (int sen_iter = 0; sen_iter < sensor_list->Length; sen_iter++) {
				ISensor^ sensor = sensor_list[sen_iter];

				if (false && sensor->SensorType != SensorType::Power) {
					continue;
				}
				std::wstring sensor_id = ClrStringToStdWstring(sensor->Identifier->ToString());

				SensorTypeCopy sen_type = static_cast<SensorTypeCopy>(static_cast<int>(sensor->SensorType));
				//val_store[sensor_name] = ;
				if (hw_rep.CheckSensorIdExist(sensor_id) && hw_rep.GetSensorsMap()[sensor_id].IsSensorMatch(
					sensor_id, sen_type)) {
					auto& sensor_rep = hw_rep.GetSensorsMap()[sensor_id];

					if (sensor->Value.HasValue) {
						sensor_rep.SetValue(sensor->Value.Value,
							_update_index);
					}
					else {
						sensor_rep.SetValue(false);
					}

				}
				else {


					hw_rep.GetSensorsMap()[sensor_id] = SensorRepresentiveBase(
						&hw_rep, sensor_id, ClrStringToStdWstring(sensor->Name),
						sen_type,
						sensor->Value.HasValue ? sensor->Value.Value : -1, _update_index);
				}
			}
			return true;
		}

		bool RecordGPUSensorsValueByHardware(IHardware^ hw, std::map<std::wstring, GPUHardwareRep>& val_store) {
			auto sensor_list = hw->Sensors;

			std::wstring hwid = ClrStringToStdWstring(hw->Identifier->ToString()),
				hw_name = ClrStringToStdWstring(hw->Name);

			if (!val_store.contains(hwid)) {
				INT64 cpu_bind_id = -1;
				if (hwid.find(L"integrated") != std::wstring::npos) {
					cpu_bind_id = 0;
				}
				else if (hwid.find(L"gpu-amd") != std::wstring::npos) {

					CPUHardwareRep* lp_cpu = GetCPUHardwareRepByIndex(0);

					std::wstring amd_gpu_name = hw_name,
						header = L"AMD ";

					if (amd_gpu_name.find(header) == 0) {
						amd_gpu_name = amd_gpu_name.substr(header.length());
					}

					if (lp_cpu && lp_cpu->GetName().find(amd_gpu_name) != std::wstring::npos) {

						cpu_bind_id = 0;
					}
				}
				val_store[hwid] = GPUHardwareRep(hwid, hw_name, 0, cpu_bind_id);
			}

			val_store[hwid].SetUpdateIterIndex(_update_index);

			auto& hw_rep = val_store[hwid];
			for (int sen_iter = 0; sen_iter < sensor_list->Length; sen_iter++) {
				ISensor^ sensor = sensor_list[sen_iter];

				if (false && sensor->SensorType != SensorType::Power) {
					continue;
				}
				std::wstring sensor_id = ClrStringToStdWstring(sensor->Identifier->ToString());

				SensorTypeCopy sen_type = static_cast<SensorTypeCopy>(static_cast<int>(sensor->SensorType));
				//val_store[sensor_name] = ;
				if (hw_rep.CheckSensorIdExist(sensor_id) && hw_rep.GetSensorsMap()[sensor_id].IsSensorMatch(
					sensor_id, sen_type)) {
					auto& sensor_rep = hw_rep.GetSensorsMap()[sensor_id];

					hw_rep.GetSensorsMap()[sensor_id].SetValue(sensor->Value.HasValue ? sensor->Value.Value : -1,
						_update_index);
				}
				else {


					hw_rep.GetSensorsMap()[sensor_id] = SensorRepresentiveBase(
						&hw_rep, sensor_id, ClrStringToStdWstring(sensor->Name),
						sen_type,
						sensor->Value.HasValue ? sensor->Value.Value : -1, _update_index);
				}
			}
			return true;
		}

		double GetCPUTotalPower() {
			double power_cal = 0.;
			for (auto& [cpu_key, cpu_instance] : _cpu_map) {
				power_cal += GetDevicePower(cpu_instance);
			}
			return power_cal;
		}

		double GetDevicePower(HardwareRepresentiveBase& hw) {
			const auto& wanted_sensor_keys = hw.GetSelectedSensorKeyList(SensorRepresentiveBase::select_power_sensor_fun);
			//First Scan if there's package power. Otherwise add all power meters.
			const auto& gpu_sensors = hw.SelectSensorsByKey(wanted_sensor_keys);

			double device_power = 0.;

			hw.SetHasPackagePowerSensor(false);

			for (const auto& sensor : gpu_sensors) {
				if (sensor.GetName().find(L"Package") != std::wstring::npos) {
					device_power = sensor.GetValue();
					hw.SetHasPackagePowerSensor(true);
					break;
				}

				device_power += sensor.GetValue();
			}


			return device_power;
		}

		double GetGPUTotalPower(bool skip_intergated_gpu = false) {
			double power_cal = 0.;



			for (auto& [gpu_key, gpu_instance] : _gpu_map) {

				//Skip the integrated GPU if necessary.
				if (skip_intergated_gpu && gpu_instance.GetIntergatedCpuId() >= 0) {
					continue;
				}

				power_cal += GetDevicePower(gpu_instance);;

			}


			return power_cal;
		}

		double GetSmartCaculatePowerMeter() {

			return GetCPUTotalPower() + GetGPUTotalPower(true);
		}

		int CallUpdateInfo() {

			ResetAllRecordedValues();
			LibreHWMonInterOpProxy::Instance()->CallUpdateComputorInfo();

			Computer^ comp_info = LibreHWMonInterOpProxy::Instance()->GetComputorInfoInstance();
			auto hw_list = comp_info->Hardware;
			//
			{

				for (int com_hw_iter = 0; com_hw_iter < hw_list->Count; com_hw_iter++) {

					// @ts-ignore
					IHardware^ hw_instance = (IHardware^)hw_list[com_hw_iter];

					//查找硬件类型
					switch (hw_instance->HardwareType)
					{
					case HardwareType::Cpu:
					{

						RecordCPUSensorsValueByHardware(hw_instance, _cpu_map);


					}
					break;
					case HardwareType::GpuNvidia:case HardwareType::GpuAmd:case HardwareType::GpuIntel:
					{
						RecordGPUSensorsValueByHardware(hw_instance, _gpu_map);
					}
					break;
					default:
					{
						std::wstringstream wss;
						wss << "Unsupported Hardware: " << ClrStringToStdWstring(hw_instance->GetReport()) << std::endl;

						OutputDebugString(wss.str().c_str());

					}
					break;
					}

				}
			}

			_update_index++;
			return 0;
		}

		std::wstring GetPowerSummaryStr(const ValueUnitStringFormatter& vusf) {
			std::wstringstream wss_res, wss_cpu, wss_gpu;

			const static int max_format_len = 256;
			static wchar_t format_str_buffer[max_format_len] = L"";

			double power_cal = 0.;
			if (g_data.m_setting_data.enable_cpu_monitor)
			{
				for (auto& [cpu_key, cpu_instance] : _cpu_map) {
					double device_pwr = GetDevicePower(cpu_instance);

					vusf.FormatPowerWattsString(format_str_buffer, max_format_len, device_pwr, nullptr, false, -1, 3);

					wss_cpu << L"\t" <<
						cpu_instance.GetName();
					wss_cpu << L": " << format_str_buffer
						<< std::endl;

					power_cal += device_pwr;
				}
				vusf.FormatPowerWattsString(format_str_buffer, max_format_len, power_cal, nullptr, false, -1, 3);
				wss_res << L"CPU: <" << format_str_buffer <<
					L">" << std::endl << wss_cpu.str();
			}

			power_cal = 0.;
			if (g_data.m_setting_data.enable_cpu_monitor)
			{
				for (auto& [gpu_key, gpu_instance] : _gpu_map) {
					double device_pwr = GetDevicePower(gpu_instance);
					vusf.FormatPowerWattsString(format_str_buffer, max_format_len, device_pwr, nullptr, false, -1, 3);
					wss_gpu << L"\t" <<
						gpu_instance.GetName() << L": " << format_str_buffer << std::endl;

					power_cal += device_pwr;

				}
				vusf.FormatPowerWattsString(format_str_buffer, max_format_len, power_cal, nullptr, false, -1, 3);
				wss_res << L"GPU: <" << format_str_buffer <<
					L">" << std::endl << wss_gpu.str();
			}

			return wss_res.str();
		}
	};

};


#endif