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
			bool die1P = false;
			bool die2P = false;
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
				if (die1P || die2P) {
					//どっちかが死んでたら勝敗判定
					parent->moveTo(Parent::NEXT_JUDGE);
				}
			}

			//更新
			int dx = 0;
			int dy = 0;
			if (f.isKeyOn('a')) {
				dx -= 1;
			}
			else if (f.isKeyOn('s')) {
				dx += 1;
			}
			else if (f.isKeyOn('w')) {
				dy -= 1;
			}
			else if (f.isKeyOn('z')) {
				dy += 1;
			}

			//更新
			state->update();
			//描画
			state->draw();
		}

	}//namespace Game
}//namespace Sequence