#pragma once
#include "StdCommon.h"

namespace Util {
	class CsvMapReader
	{
	public:
		CsvMapReader();
		~CsvMapReader() { }
		void load(const char *filename);
		int getMapData(int x, int y);

	private:
		int size_;
		char *data_;
		int mapSize_;
		int mapData_[YBlock * XBlock];
	};
}
