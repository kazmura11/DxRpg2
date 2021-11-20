#include "Sequence/Game/Battle/Battle.h"
#include "Util/GameController.h"
#include "Util/ResourceLoader.h"
#include "Sequence/Parent.h"
#include "Battle/BattleProcess.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"

namespace Sequence
{
	namespace Game
	{
		namespace Battle
		{

			Battle::Battle(std::shared_ptr<::SharedCharacterStatus> sharedStatus) :
				gc_(Util::GameController::getInstance()),
				rl_(Util::ResourceLoader::getInstance()),
				chr_(std::make_unique<::Battle::BattleCharacter>(sharedStatus)),
				mon_(std::make_unique<::Battle::BattleMonster>()),
				randX_{}, randY_{}, startBattleX_{}, startBattleY_{}
			{
				// キャラ種別を外から設定
				chr_->setKind(0);
				mon_->setKind(0);

				// 中心を外から設定
				int axisChrX, axisChrY;
				DxLib::GetGraphSize(
					rl_.getHdlImgChar(chr_->getKind(), 5),
					&axisChrX, &axisChrY);
				chr_->setCenterX(chr_->getX() + axisChrX / 2);
				chr_->setCenterY(chr_->getY() + axisChrY / 2);

				int axisMonX, axisMonY;
				DxLib::GetGraphSize(
					rl_.getHdlImgMonster(mon_->getKind()),
					&axisMonX, &axisMonY);
				mon_->setCenterX(mon_->getX() + axisMonX / 2);
				mon_->setCenterY(mon_->getY() + axisMonY / 2);

				bp_ = std::make_unique<::Battle::BattleProcess>(chr_.get(), mon_.get());

				for (int i = 0; i < ImgCharDivNum; i++)
				{
					startBattleX_[i] = 0;
					startBattleY_[i] = 0;
					randX_[i] = 0;
					randY_[i] = 0;
				}

				nextSequence_ = NextSequence::NextBattleShatter;   // 最初はコレ
			}

			Battle::~Battle()
			{
			}

			// バトルにおいて、どの関数に処理を渡すかを制御
			void Battle::update(Sequence::Game::Parent *parent)
			{
				switch (nextSequence_)
				{
				case NextSequence::NextBattleShatter:  // 画面が割れて落ちるシーン
					startBattleShatter();
					break;
				case NextSequence::NextBattleEnter:  // 戦闘突入シーン
					startBattleEnter();
					break;
				case NextSequence::NextBattleMain:  // 戦闘シーン
					bp_->main();    // 戦闘メイン処理
					if (bp_->getWinMoveNext())
					{
						nextSequence_ = NextSequence::NextMap;
					}
					if (bp_->getGameoverMoveNext())
					{
						nextSequence_ = NextSequence::NextRestart;
					}
					break;
				case NextSequence::NextBattleDebug:  // DEBUG用
					//debugBattleMode();
					break;
				case NextSequence::NextMap:
					parent->moveTo(Parent::NextSequence::NextMapMain);
					break;
				case NextSequence::NextRestart:
					parent->moveTo(Parent::NextSequence::NextRestart);
					break;
				default:
					exit(EXIT_FAILURE);
					break;
				}
			}

			void Battle::startBattleShatter()
			{
				const double Accel = 1.5;
				const int ShatterNum = 8;
				const int DownSt = 60;
				const int DownEd = 150;
				static int cnt = 0;

				if (cnt == 0)
				{
					// 最初に処理が行われた時、DivNum*DivNum個の画像の初期座標を設定
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							startBattleX_[i * DivisionNum + j] = WindWidth / DivisionNum * static_cast<double>(j);   // 描画する各ｘ座標
							startBattleY_[i * DivisionNum + j] = WindHeight / DivisionNum * static_cast<double>(i);  // 描画する各ｙ座標
							randX_[i * DivisionNum + j] = DxLib::GetRand(30);   // ばらけさせるための乱数
							randY_[i * DivisionNum + j] = DxLib::GetRand(30);   // ばらけさせるための乱数
						}
					}
				}
				// 砕け散るモード(外へ散らばせる)
				if (cnt < ShatterNum)
				{
					// ShatterNumまでの間中心から外へ散らばらせる
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							// 四方八方に飛び散らせる
							startBattleX_[i * DivisionNum + j]
								+= (randX_[i * DivisionNum + j] + 30 * static_cast<double>(j) - 45) / 8.0;
							startBattleY_[i * DivisionNum + j]
								+= (randY_[i * DivisionNum + j] + 30 * static_cast<double>(i) - 45) / 8.0;
						}
					}
				}
				// 落ちる時の乱数生成
				if (cnt == ShatterNum)
				{
					// 落ちる時にばらけさせるための乱数生成
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							randX_[i * DivisionNum + j] = GetRand(40) / 10.0 - 2.0;
							randY_[i * DivisionNum + j] = GetRand(10) / 10.0 + 1.0;
						}
					}
				}

				// 落ちるモード
				if (cnt >= DownSt && cnt <= DownEd)
				{
					// 落ちるモーション
					double t = (cnt - DownSt) * 17 / 1000;
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							startBattleX_[i * DivisionNum + j]
								+= randX_[i * DivisionNum + j];
							// 落ちる時加速度をもって計算。Accelの値で変更可能
							startBattleY_[i * DivisionNum + j]
								+= t * t * Accel * randY_[i * DivisionNum + j] / 2.0 * 10;
						}
					}
				}
				// 150になったら次のシーンへ
				if (cnt == DownEd)
				{
					cnt = 0;
					nextSequence_ = NextSequence::NextBattleEnter;
					return;
				}
				// 分割区画をそれぞれ描画
				for (int i = 0; i < ImgCharDivNum; i++)
				{
					DxLib::DrawGraph(static_cast<int>(startBattleX_[i]),
						static_cast<int>(startBattleY_[i]),
						rl_.getHdlImgStartBattle(i), FALSE);
				}
				cnt++;
			}

			void Battle::startBattleEnter()
			{
				const double StartBattleTime = 120.0;
				const int Stage1 = 80;
				const int Stage2 = 170;
				const int Stage3 = 255;
				static int cnt = 0;

				if (cnt == 0)
				{
					// オープニング2が始まる瞬間、一旦全部描画して、
					// その画面をキャプチャして反映させる前にクリアする。
					// この１枚のキャプチャした画像でオープニングを作る
					DxLib::DrawGraph(0, 0, rl_.getHdlImgBackGround(0), FALSE);

					// 主人公・左向き画像の読込 0, 5
					DxLib::DrawGraph(::Battle::BattleCharPosX1, ::Battle::BattleCharPosY1,
						rl_.getHdlImgChar(chr_->getKind(), 5), TRUE);

					// 敵キャラ読込
					DxLib::DrawGraph(::Battle::BattleMonPosX1, ::Battle::BattleMonPosY1,
						rl_.getHdlImgMonster(mon_->getKind()), TRUE);

					rl_.setImageStartBattle(0, WindWidth, WindHeight);

					// 画像キャプチャ
					DxLib::GetDrawScreenGraph(0, 0, WindWidth, WindHeight,
						rl_.getHdlImgStartBattle(0));
					DxLib::ClearDrawScreen();
				}
				if (cnt > Stage1 && cnt <= Stage2)
				{
					DxLib::DrawRotaGraph(-50 + cnt, 100, 2.0, 0, rl_.getHdlImgStartBattle(0), FALSE);
				}
				if (cnt > Stage2 && cnt <= Stage3)
				{
					DxLib::DrawRotaGraph(500 - cnt, 400, 2.0, 0, rl_.getHdlImgStartBattle(0), FALSE);
				}
				if (cnt > Stage3 && cnt <= Stage3 + StartBattleTime)
				{
					int t = cnt - Stage3;
					DxLib::DrawExtendGraph(
						static_cast<int>((WindWidth / 2 + WindWidth / DivisionNum)
							- (WindWidth / 2 + static_cast<double>(WindWidth / DivisionNum))
							* static_cast<double>(t / StartBattleTime)),
						0,
						static_cast<int>(WindWidth * static_cast<double>(t) / StartBattleTime),
						static_cast<int>(WindHeight * static_cast<double>(t) / StartBattleTime),
						rl_.getHdlImgStartBattle(0),
						TRUE
					);
				}
				if (cnt == Stage3 + StartBattleTime)
				{	// 時間になったらオープニングを終了して戦闘へ
					cnt = 0;
					nextSequence_ = NextSequence::NextBattleMain;
					return;
				}
				cnt++;
			}
		}
	}
	
}
