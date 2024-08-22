#include "pch.h"
#include "LibreHWMonInterOp.h" 


namespace InterOpLibreHWMon {
	void PowerSensorUpdateVisitor::VisitComputer(IComputer^ computer)
	{
		computer->Traverse(this);
	}


	void PowerSensorUpdateVisitor::VisitHardware(IHardware^ hardware)
	{
		//Filter wanted hardware
		switch (hardware->HardwareType) {
		case HardwareType::Cpu: {
			if (!_cpu_mon_enable) {
				return;
			}
		}break;

		case HardwareType::GpuNvidia: case HardwareType::GpuAmd: case HardwareType::GpuIntel:
		{
			if (!_gpu_mon_enable) {
				return;
			}
		}break;

		default:
			return;
		}
		hardware->Update();

		for each (ISensor ^ sensor in hardware->Sensors) {
			if (sensor->SensorType == SensorType::Power) {
				this->VisitSensor(sensor);
			}
		}
		for each (IHardware ^ subHardware in hardware->SubHardware)
		{
			this->VisitHardware(subHardware);

		}
	}

	void PowerSensorUpdateVisitor::VisitSensor(ISensor^ sensor)
	{

	}

	void PowerSensorUpdateVisitor::VisitParameter(IParameter^ parameter)
	{
	}

	bool HardwareRepresentiveBase::operator==(const HardwareRepresentiveBase& other) const {
		return this->_identity == other._identity;
	}

	bool SensorRepresentiveBase::operator==(const SensorRepresentiveBase& other) const {
		return this->_identity == other._identity && (this->_p_owner != nullptr && other._p_owner != nullptr) &&
			*(this->_p_owner) == *(other._p_owner);
	}


	bool SensorRepresentiveBase::IsSensorMatch(const std::wstring& identity, SensorTypeCopy sensor_type) const {
		return identity == this->_identity && sensor_type == this->_sensor_type;
	}

	const std::function<bool(const SensorRepresentiveBase&)> SensorRepresentiveBase::select_power_sensor_fun = (const std::function<bool(const SensorRepresentiveBase&)>)[](const SensorRepresentiveBase& sensor) -> bool {
		return sensor.GetSensorType() == SensorTypeCopy::Power;
	};
}