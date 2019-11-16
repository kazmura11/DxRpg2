#define _CRT_SECURE_NO_WARNINGS
#include "Sequence/Game/Map/Map.h"
#include "Sequence/Game/Parent.h"
#include "Map/AbstractMapCharacter.h"
#include "Util/ResourceLoader.h"
#include "Util/CsvMapReader.h"

namespace Sequence
{
	namespace Game
	{
		namespace Map
		{
			Map::Map(SharedCharacterStatus *scs, int stage)
				: hasChanged_(true), mapStage_(stage), nextSequence_(NextMap),
				 rl_(Util::ResourceLoader::getInstance()),
				 cmr_(std::make_unique<Util::CsvMapReader>()),
				 scs_(scs)
			{
				// create character instance
				mapMainChar_ = std::make_unique<::Map::MapMainCharacter>();
				for (int i = 0; i < CompCharMax; i++)
				{
					mapCityChar_.push_back(std::move(
						std::make_unique<::Map::MapCityCharacter>()));
				}
				for (int i = 0; i < YBlock * XBlock; i++)
				{
					isPassable_[i] = Through;
				}
			}

			Map::~Map()
			{
			}

			void Map::update(Parent *parent)
			{
				if (hasChanged_)
				{
					initMapState();
					initCharState();
				}
				else
				{
					drawMap();
					drawCharacter();
				}
				moveCharacter(parent);
				changeMap(parent);
			}

			void Map::initMapState()
			{
				cmr_->load(MapFile[mapStage_]);
				// set passable data
				for (int y = 0; y < YBlock; y++)  // vertical 15
				{
					for (int x = 0; x < XBlock; x++)  // horizontal 15
					{
						if (cmr_->getMapData(x, y) > 1)  // if not passable
						{
							isPassable_[y * XBlock + x] = NoThrough;  // set not passable
						}
						// if passable
						else
						{
							// 4 corners + and its inner blocks (outer 2 blocks)
							// avoid moving to "map change block".
							if (y == 0 || y == 1 || y == YBlock - 2 || y == YBlock - 1
								|| x == 0 || x == 1 || x == XBlock - 2 || x == XBlock - 1)
							{
								// Only main character can move to "map change block".
								isPassable_[y * XBlock + x] = MainCharOnly;
							}
							else
							{
								isPassable_[y * XBlock + x] = Through;
							}
						}
					}
				}
			}

			void Map::initCharState()
			{
				// deploy character each position
				int tx = mapMainChar_->getX();
				int ty = mapMainChar_->getY();
				int dir = mapMainChar_->getDir();
				int kind = 0;  // 0 for main character
				mapMainChar_->initMapState(tx, ty, dir, kind);
				// set not passable
				isPassable_[ty / BlockLen * XBlock + tx / BlockLen] = NoThrough;
				for (int i = 0; i < CompCharMax;)
				{
					// 4 corners + and its inner block (outer 2 blocks) => not passable
					// Note: GetRand(10) returns 0-10 number so, you want to 0-9, then GetRand(9)
					// -4 => 0,1,MaxPos-2, MaxPos-1
					int rx = DxLib::GetRand(XBlock - 5) + 2;
					int ry = DxLib::GetRand(YBlock - 5) + 2;
					if (isPassable_[ry * XBlock + rx] == MainCharOnly
						|| isPassable_[ry * XBlock + rx] == NoThrough)
					{
						continue;	// recalculate...
					}
					tx = rx * BlockLen;
					ty = ry * BlockLen;
					dir = DxLib::GetRand(3);		// direction of charchip 0-3
					kind = i % (CharKindMax - 1) + 1;		// character kind 1-3
					mapCityChar_[i]->initMapState(tx, ty, dir, kind);
					isPassable_[ty / BlockLen * XBlock + tx / BlockLen] = NoThrough;
					i++;
				}
				hasChanged_ = false;
			}

			void Map::moveCharacter(Parent *parent)
			{
				// move main character and if encountered enemy
				bool encountered = mapMainChar_->move(isPassable_);
				if (encountered)
				{
					// must call captureImgMap before "ClearDrawScreen"
					rl_.captureImgMap();
					// update scene
					parent->moveTo(Parent::NextBattle);
				}
				mapMainChar_->setImg(
					rl_.getHdlImgChar(
						mapMainChar_->getKind(),
						mapMainChar_->getAnimePicPos()
					)
				);

				// city character
				for (int i = 0; i < CompCharMax; i++)
				{
					mapCityChar_[i]->move(isPassable_);
					// update character image
					mapCityChar_[i]->setImg(
						rl_.getHdlImgChar(
							mapCityChar_[i]->getKind(),
							mapCityChar_[i]->getAnimePicPos()
						)
					);
				}
			}

			void Map::changeMap(Parent *parent)
			{
				if (mapMainChar_->getX() <= 0
					|| mapMainChar_->getY() <= 0
					|| mapMainChar_->getX() >= MapWidth - BlockLen
					|| mapMainChar_->getY() >= MapHeight - BlockLen)
				{
					// move to right edge change block
					if (mapMainChar_->getX() >= MapWidth - BlockLen)
					{
						mapMainChar_->setX(BlockLen);
						mapStage_++;
					}
					// move to left edge change block
					if (mapMainChar_->getX() <= 0)
					{
						mapMainChar_->setX(MapWidth - BlockLen * 2);
						mapStage_--;
					}
					// change map
					hasChanged_ = true;
					parent->setMapStage(mapStage_);
				}
			}

			void Map::drawMap()
			{
				for (int y = 0; y < YBlock; y++)  // vertical loop
				{
					// decide left edge and top edge coordinate
					//   when the main character's position coordinate is center.
					int centerX = WindWidth / 2 - BlockLen / 2;
					int centerY = WindHeight / 2 - BlockLen / 2;
					int ofsY = centerY - mapMainChar_->getY();  // relative Y coodinate
					for (int x = 0; x < XBlock; x++)
					{
						int ofsX = centerX - mapMainChar_->getX();
						// draw objects in the offset position from main character (=center)
						// draw lawn
						DxLib::DrawGraph(
							x * BlockLen + ofsX,
							y * BlockLen + ofsY,
							rl_.getHdlImgMapchip(0), TRUE);
						// draw other objects
						if (cmr_->getMapData(x, y) != 0)
						{
							DxLib::DrawGraph(
								x * BlockLen + ofsX,
								y * BlockLen + ofsY,
								rl_.getHdlImgMapchip(cmr_->getMapData(x, y)), TRUE);
						}
#ifdef _DEBUG
						char buf1[2] = { '\0' };
						char buf2[2] = { '\0' };
						int slide1 = BlockLen / 2 - 10;
						int slide2 = BlockLen / 2;
						sprintf(buf1, "%d", cmr_->getMapData(x, y));
						sprintf(buf2, "%d", isPassable_[y * XBlock + x]);
						int color1 = DxLib::GetColor(255, 255, 255);
						int color2 = isPassable_[y * XBlock + x] == 2 ? DxLib::GetColor(255, 0, 0) :
								DxLib::GetColor(0, 0, 255);
						DxLib::DrawString(
							x * BlockLen + ofsX + slide1,
							y * BlockLen + ofsY + slide1,
							buf1,color1);
						DxLib::DrawString(
							x * BlockLen + ofsX + slide2,
							y * BlockLen + ofsY + slide2,
							buf2, color2);
#endif
					}
				}
			}

			void Map::drawCharacter()
			{
				/**
				 * if use 32x48 charchip, should be draw from above considering overwrap images.
				 * it is so called "Z-sort"
				 * 
				 */
				for (int y = 0; y < YBlock; y++)
				{
					int centerX = WindWidth / 2 - BlockLen / 2;
					int centerY = WindHeight / 2 - BlockLen / 2;
					int ty = mapMainChar_->getY();
					// if hit the main character's block, then draw images
					if (y == ty / BlockLen)
					{
						DxLib::DrawGraph(
							centerX,	// central coordinate
							// if vertical length is 48px then minus 16px.
							// if vertical length is 32px, no need to minus.
							centerY - SlidePicVal,
							mapMainChar_->getImg(), TRUE);
					}
					for (int i = 0; i < CompCharMax; i++)
					{
						ty = mapCityChar_[i]->getY();
						// if hit the city character's block, then draw images
						if (y == ty / BlockLen)
						{
							int tx = mapCityChar_[i]->getX();
							int ofsY = centerY - mapMainChar_->getY();  // offset from main character Y coodinate
							int ofsX = centerX - mapMainChar_->getX();  // offset from main character X coodinate
							// draw city character in the offset position from main character (=center)
							DxLib::DrawGraph(
								tx + ofsX,
								// if vertical length is 48px then minus 16px.
								// if vertical length is 32px, no need to minus.
								ty + ofsY - SlidePicVal,
								mapCityChar_[i]->getImg(), TRUE);
						}
					}
				}
			}
		}
	}
}
