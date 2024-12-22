#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "State.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Parent.h"

namespace Sequence {
	namespace Game {

		Judge::Judge() : mImage(0), mCursorPosistion(0) {
			mImage = new Image("data/image/dummy.dds");
		}

		Judge::~Judge() {
			SAFE_DELETE(mImage);
		}

		void Judge::update(Parent* parent) {
			Framework f = Framework::instance();
			if (f.isKeyTriggered('w')) {
				--mCursorPosistion;
				if (mCursorPosistion < 0) {
					//マイナスは最大値二ループ
					mCursorPosistion = 1;
				}
			}
			else if (f.isKeyTriggered('z')) {
				++mCursorPosistion;
				if (mCursorPosistion > 1) {
					//1を超えたら0二ループ
					mCursorPosistion = 0;
				}
			}
			else if (f.isKeyTriggered(' ')) {
				if (mCursorPosistion == 0) {
					//続けて勝負
					parent->moveTo(Parent::NEXT_READY);
				}
				else if (mCursorPosistion == 1) {
					//タイトルへ
					parent->moveTo(Parent::NEXT_TITLE);
				}
			}

			//描画
			//まずゲーム画面
			parent->state()->draw();
			//上に重ねる
			mImage->draw();

			//まずどっちが買ったのか表示
			f.drawDebugString(0, 0, "[ｼｮｳﾊｲｹｯﾃｲ]");
			f.drawDebugString(0, 1, "1Pﾉ ｶﾁ!");
			//メニュー
			f.drawDebugString(1, 3, "ﾏﾀﾞ ｺﾛｼｱｳ");
			f.drawDebugString(1, 4, "ﾔﾒﾃ ﾀｲﾄﾙ ｶﾞﾒﾝ ﾍ");
			//カーソルを書く。メニューに重なるように。
			f.drawDebugString(0, mCursorPosistion + 3, ">");
		}
	}//namespace Game
}//namespace Sequence