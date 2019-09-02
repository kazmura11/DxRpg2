#include "Board.h"
#include "StdCommon.h"
#include "Util/ResourceLoader.h"

namespace Battle
{
	Board::Board()
	{
	}

	Board::~Board()
	{
	}

	void Board::graph()
	{
		for (int i = 0; i < BoardMax; i++)
		{
			if (board_[i].flag)
			{
				if (board_[i].cnt == 120)
				{
					board_[i].flag = false;
					continue;
				}
				// メッセージ部
				DxLib::DrawGraph(10, 26, Util::ResourceLoader::getInstance().getHdlImgEtc(1), TRUE);
				DxLib::DrawFormatString(14, 28,
					DxLib::GetColor(255, 255, 255), "%s", board_[i].message);
				board_[i].cnt++;
				break;
			}
		}
	}

	void Board::add(char *message)
	{
		int i = 0;
		for (i = 0; i < BoardMax - 1; i++)
		{
			if (!board_[i].flag)
			{
				break;
			}
		}
		board_[i].flag = true;
		board_[i].cnt = 0;
		strcpy(board_[i].message, message);
	}
}