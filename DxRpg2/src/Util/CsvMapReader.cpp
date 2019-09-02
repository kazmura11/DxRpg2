#include "Util/CsvMapReader.h"
#include <fstream>
#include <cstring>

namespace Util {
	CsvMapReader::CsvMapReader()
		: size_(0), data_(NULL), mapSize_(0)
	{
		for (int i = 0; i < YBlock * XBlock; i++)
		{
			mapData_[i] = '\0';
		}
	}

	int CsvMapReader::getMapData(int x, int y)
	{
		return mapData_[y * XBlock + x];
	}

	void CsvMapReader::load(const char* filename)
	{
		mapSize_ = YBlock * XBlock;
		// テキストモードでオープン
		std::ifstream ifs(filename);
		if (ifs.fail())
		{
			//exit(EXIT_FAILURE);	// Fatal Error
			throw("%s Fatal Error. Cannnot read file." __FILE__);
		}
		else
		{
			// サイズの計測
			ifs.seekg(0, std::ifstream::end);
			size_ = static_cast<int>(ifs.tellg());
			// ファイルポインタ巻き戻し
			ifs.seekg(0, std::ifstream::beg);
			// NULL文字を付加
			data_ = new char[size_];
			data_[size_ - 1] = '\0';
			// 一気に読み込む
			ifs.read(data_, size_);
		}
		// CSVパース
		const char* Delim = ",\n";
		int i = 0;
		for (char* token = strtok(data_, Delim);
			token != NULL && i < mapSize_;
			token = strtok(0, Delim))
		{
			// 余計なカンマがあった時の対策
			if (token != NULL)
			{
				// 数値に変換する
				mapData_[i] = atoi(token);
				i++;
			}
		}
		delete[] data_;
		data_ = NULL;
	}
}