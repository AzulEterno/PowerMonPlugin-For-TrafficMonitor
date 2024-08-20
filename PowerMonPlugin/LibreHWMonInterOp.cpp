#include "pch.h"
#include "LibreHWMonInterOp.h"


namespace InterOpLibreHWMon {
	void PowerSenUpdateVisitor::VisitComputer(IComputer^ computer)
	{
		computer->Traverse(this);
	}


	void PowerSenUpdateVisitor::VisitHardware(IHardware^ hardware)
	{
		hardware->Update();
		for each (IHardware ^ subHardware in hardware->SubHardware)
		{
			subHardware->Accept(this);
		}
	}

	void PowerSenUpdateVisitor::VisitSensor(ISensor^ sensor)
	{
	}

	void PowerSenUpdateVisitor::VisitParameter(IParameter^ parameter)
	{
	}
}