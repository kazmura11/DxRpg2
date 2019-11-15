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
			// マップ画面の初期化処理
			Map::Map(SharedCharacterStatus *scs, int stage)
				: hasChanged_(true), mapStage_(stage), nextSequence_(NextMap),
				 rl_(Util::ResourceLoader::getInstance()),
				 cmr_(std::make_unique<Util::CsvMapReader>()),
				 scs_(scs)
			{
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
				UNREFERENCED_PARAMETER(parent);
				if (hasChanged_)
				{
					initMapState();
					initCharState();
				}
				else
				{
					// マップの描画
					drawMap();
					// キャラクタの描画
					drawCharacter();
				}
				// マップ位置の計算及び歩行状態の決定
				moveCharacter(parent);
				// マップ変更判定
				changeMap(parent);
			}

			// マップ画面の初期化処理
			void Map::initMapState()
			{
				cmr_->load(MapFile[mapStage_]);
				// マップの状態から通過フラグを作成
				for (int y = 0; y < YBlock; y++)  // 縦の15区間
				{
					for (int x = 0; x < XBlock; x++)  // 横の20区間
					{
						if (cmr_->getMapData(x, y) > (0 | 1))  // 通れない場所なら
						{
							isPassable_[y * XBlock + x] = NoThrough;  // 通れないフラグを立てる
						}
						// 通れる場所なら
						else
						{
							// 四隅 + その一つ内周なら(これが移動時に万が一重なるバグを回避する手段)
							// やりすぎ感があるがまあいいか
							if (y == 0 || y == 1 || y == YBlock - 2 || y == YBlock - 1
								|| x == 0 || x == 1 || x == XBlock - 2 || x == XBlock - 1)
							{
								// 町キャラが画面遷移しないようにする
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
				// キャラクターを配置する。
				int tx = mapMainChar_->getX();  // 計算用Y座標
				int ty = mapMainChar_->getY();   // 計算用X座標
				int dir = mapMainChar_->getDir();  // 向き
				int kind = 0;  // キャラクタの種類 0は主人公
				mapMainChar_->initMapState(tx, ty, dir, kind);
				// 主人公か町の人がいるところは通れない場所とする
				isPassable_[ty / BlockLen * XBlock + tx / BlockLen] = NoThrough;
				for (int i = 0; i < CompCharMax;)
				{
					// 四隅とその２つ内周(-4からさらに-1していることに注意[GetRandの特性])を避ける
					int rx = DxLib::GetRand(XBlock - 5) + 2;
					int ry = DxLib::GetRand(YBlock - 5) + 2;
					// 町の人で乱数で決めた場所が通れない場所なら
					if (isPassable_[ry * XBlock + rx] >= MainCharOnly)
					{
						continue;	// やり直し
					}
					tx = rx * BlockLen;
					ty = ry * BlockLen;
					dir = DxLib::GetRand(3);		// 向きをランダムに決定 0-3
					kind = i % (CharKindMax - 1) + 1;		// 人の種類を決定 ※0は主人公  1-3
					mapCityChar_[i]->initMapState(tx, ty, dir, kind);
					// 主人公か町の人がいるところは通れない場所とする
					isPassable_[ty / BlockLen * XBlock + tx / BlockLen] = NoThrough;
					i++;	// 位置決めが成功したときのみカウンタ増加
				}
				hasChanged_ = false;
			}

			// キャラの移動制御
			void Map::moveCharacter(Parent *parent)
			{
				// 主人公 敵に遭遇
				if (mapMainChar_->move(isPassable_))
				{
					// 敵に遭遇したら画面キャプチャ＆音楽停止＆モード遷移
					// このタイミングで呼ばないとキャプチャできない(ClearDrawScreenの前)
					rl_.captureImgMap();
					// 遷移先設定
					parent->moveTo(Parent::NextBattle);
				}
				mapMainChar_->setImg(
					rl_.getHdlImgChar(
						mapMainChar_->getKind(),
						mapMainChar_->getAnimePicPos()
					)
				);

				// 町の人
				for (int i = 0; i < CompCharMax; i++)
				{
					mapCityChar_[i]->move(isPassable_);
					// 分割イメージをセット
					mapCityChar_[i]->setImg(
						rl_.getHdlImgChar(
							mapCityChar_[i]->getKind(),
							mapCityChar_[i]->getAnimePicPos()
						)
					);
				}
			}

			// マップ切り替え
			void Map::changeMap(Parent *parent)
			{
				if (mapMainChar_->getX() <= 0
					|| mapMainChar_->getY() <= 0
					|| mapMainChar_->getX() >= MapWidth - BlockLen
					|| mapMainChar_->getY() >= MapHeight - BlockLen)
				{
					// 右に出たら
					if (mapMainChar_->getX() >= MapWidth - BlockLen)
					{
						// yそのままで一番左の一個右へ
						mapMainChar_->setX(BlockLen);
						// マップ変更
						mapStage_++;
					}
					// 左に出たら
					if (mapMainChar_->getX() <= 0)
					{
						// yそのままで一番の右一個左へ
						mapMainChar_->setX(MapWidth - BlockLen * 2);
						// マップ変更
						mapStage_--;
					}
					// 必要があれば実装
					// 上、下
					// マップ変更
					hasChanged_ = true;
					parent->setMapStage(mapStage_);
				}
			}

			// マップの描画
			void Map::drawMap()
			{
				for (int y = 0; y < YBlock; y++)  // 縦の区間個数分ループ
				{
					// 主人公を中心とする座標で画像の左端/上端開始点を求める。
					int ofsY = WindHeight / 2 - BlockLen / 2 - mapMainChar_->getY();  // 主人公相対座標
					for (int x = 0; x < XBlock; x++)  // 横の区間個数
					{
						int ofsX = WindWidth / 2 - BlockLen / 2 - mapMainChar_->getX();  // 主人公相対座標
						// いったん芝生を描画
						DxLib::DrawGraph(
							x * BlockLen + ofsX,	// 描画相対位置
							y * BlockLen + ofsY,				   // 描画相対位置
							rl_.getHdlImgMapTip(0), TRUE);
						// 本来、芝生でなく他の何かの描画であれば
						if (cmr_->getMapData(x, y) != 0)
						{								// その画像を描画
							DxLib::DrawGraph(
								x * BlockLen + ofsX,
								y * BlockLen + ofsY,
								rl_.getHdlImgMapTip(cmr_->getMapData(x, y)), TRUE);
						}
					}
				}
			}

			// キャラクタの描画
			void Map::drawCharacter()
			{
				// キャラがスライドしている分、重なりを考慮して上から描画 ただし、この考慮は32x48のキャラチップの時のみ
				for (int y = 0; y < YBlock; y++)
				{
					int ty = mapMainChar_->getY();
					// 調査しているところと同じ範囲の座標なら描画
					// こうしないと手前のキャラが前に描画されない（重なりがおかしくなる）
					// 画面奥から順に描画するこの方法をZソートという
					if (y == ty / BlockLen)
					{
							// 主人公
							// 調査しているところと同じ範囲の座標なら描画
							// 真ん中からの相対位置
							DxLib::DrawGraph(
								WindWidth / 2 - BlockLen / 2,	// 中心
								// キャラが縦長の48pixcelのため、
								// 32pixelからはみ出る16pixel分上にずらしている。
								WindHeight / 2 - BlockLen / 2 - SlidePicVal,  // 中心 + スライド分
								mapMainChar_->getImg(), TRUE);
					}
					for (int i = 0; i < CompCharMax; i++)
					{
						ty = mapCityChar_[i]->getY();
						// 調査しているところと同じ範囲の座標なら描画
						// こうしないと手前のキャラが前に描画されない（重なりがおかしくなる）
						// 画面奥から順に描画するこの方法をZソートという
						if (y == ty / BlockLen)
						{
							// 主人公以外
							// 真ん中からの相対位置
							int tx = mapCityChar_[i]->getX();
							int ofsY = WindHeight / 2 - BlockLen / 2 - mapMainChar_->getY();  // 主人公相対座標
							int ofsX = WindWidth / 2 - BlockLen / 2 - mapMainChar_->getX();  // 主人公相対座標
							// x,yは左上頂点の座標で、
							// x:マップを1個分x座標に追加しているためのずらし
							// 主人公が動いた分もずらす
							DxLib::DrawGraph(
								tx + ofsX,
								// キャラが縦長の48pixcelのため、
								// 32pixelからはみ出る16pixel分上にずらしている。
								ty + ofsY - SlidePicVal,	 // ブロック分のずらしはキャラについて
								mapCityChar_[i]->getImg(), TRUE);
						}
					}
				}
			}
		}
	}
}
