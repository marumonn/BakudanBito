#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"
#include "Game/State.h"

namespace Sequence {
	namespace Game {

		Failure::Failure() : mImage(0), mCount(0) {
			mImage = new Image("data/image/dummy.dds");
		}

		Failure::~Failure() {
			SAFE_DELETE(mImage);
		}

		void Failure::update(Parent* parent) {
			if (mCount == 60) { //1秒待つ 
				if (parent->lifeNumber() == 0) {
					//ライフがなければゲームオーバー
					parent->moveTo(Parent::NEXT_GAME_OVER);
				}
				else {
					parent->moveTo(Parent::NEXT_READY);
				}
			}
			//描画
			//まずゲーム画面を描画
			parent->state()->draw();
			//上に失敗画像を描画
			mImage->draw();
			//失敗テキストを出す
			Framework::instance().drawDebugString(0, 0, "ｱｰｯ!");

			++mCount;
		}

	} //namespace Game
} //namespace Sequence
