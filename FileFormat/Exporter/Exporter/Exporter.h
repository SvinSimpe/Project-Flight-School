#pragma once

#include "data_structures.h"
//
//struct Header
//{
//	UINT size;
//};

class Exporter
{
public:
	Exporter();
	~Exporter();
	
	void RunExporter();
	void WriteToFile();
};

