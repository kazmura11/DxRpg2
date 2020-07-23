#include "Util/CsvMapReader.h"
#include <fstream>
#include <cstring>

namespace Util {
	CsvMapReader::CsvMapReader()
		: size_(0), data_(NULL), mapSize_(0), mapData_{'\0'}
	{
	}

	int CsvMapReader::getMapData(int x, int y)
	{
		return mapData_[y * XBlock + x];
	}

	void CsvMapReader::load(const char* filename)
	{
		mapSize_ = YBlock * XBlock;
		// open text file
		std::ifstream ifs(filename);
		if (ifs.fail())
		{
			throw("%s Fatal Error. Cannnot read file." __FILE__);
		}
		else
		{
			// calculate size
			ifs.seekg(0, std::ifstream::end);
			size_ = static_cast<int>(ifs.tellg());
			// rewind file pointer
			ifs.seekg(0, std::ifstream::beg);
			data_ = new char[size_];
			data_[size_ - 1] = '\0';
			// read whole data
			ifs.read(data_, size_);
		}
		// parse csv simply
		const char* Delim = ",\n";
		int i = 0;
		for (char* token = strtok(data_, Delim);
			token != NULL && i < mapSize_;
			token = strtok(0, Delim))
		{
			if (token != NULL)
			{
				// string to integer
				mapData_[i] = atoi(token);
				i++;
			}
		}
		delete[] data_;
		data_ = NULL;
	}
}