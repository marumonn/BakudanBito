#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "Game/State.h"

namespace Sequence {
	namespace Game {

		Play::Play() {
			//何もしない
		}
		Play::~Play() {
			//何もしない
		}

		//ゲーム本体
		void Play::update(Parent* parent) {
			Framework f = Framework::instance();

			State* state = parent->state();

			bool cleared = state->hasCleared();
			bool die1P = !state->isAlive(0);
			bool die2P = !state->isAlive(1);
			//とりあえずデバッグコマンドでテストする。
			if (f.isKeyTriggered('1')) {
				//1Pが殺す、つまり2Pが死ぬ
				die2P = true;
			}
			else if (f.isKeyTriggered('2') || f.isKeyTriggered('x')) {
				//2Pが殺す
				die1P = true;
			}
			else if (f.isKeyTriggered('c')) {
				cleared = true;
			}

			//スペースが押されたらポーズへ行く
			if (f.isKeyTriggered(' ')) {
				parent->moveTo(Parent::NEXT_PAUSE);
			}

			//クリアしたら上へ報告
			if (parent->mode() == Parent::MODE_1P) {
				if (cleared && !die1P) {
					parent->moveTo(Parent::NEXT_CLEAR);
				}
				else if (die1P) {
					parent->moveTo(Parent::NEXT_FAILURE);
				}
			}
			else {
				//二人用
				if (die1P || die2P) { //どっちかが死んでたら勝敗判定
					parent->moveTo(Parent::NEXT_JUDGE);
					if (die1P && die2P) {
						parent->setWinner(Parent::PLAYER_NONE); //両方死亡
					}
					else if (die1P) {
						parent->setWinner(Parent::PLAYER_2);
					}
					else {
						parent->setWinner(Parent::PLAYER_1);
					}
				}
			}
			if (f.isKeyTriggered(' ')) {
				parent->moveTo(Parent::NEXT_PAUSE);
			}

			//更新
			state->update();
			//描画
			state->draw();
		}

	}//namespace Game
}//namespace Sequence