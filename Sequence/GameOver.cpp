#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"

using namespace GameLib;

#include "Sequence/GameOver.h"
#include "Sequence/Parent.h"
#include "Image.h"

namespace Sequence {
	GameOver::GameOver() : mImage(0), mCount(0) {
		mImage = new Image("data/image/dummy.dds");
	}

	GameOver::~GameOver() {
		SAFE_DELETE(mImage);
	}

	void GameOver::update(Parent* parent) {
		if (mCount == 60) { //1秒待つ
			parent->moveTo(Parent::NEXT_TITLE);
		}
		//描画
		mImage->draw();
		//字を出す
		Framework::instance().drawDebugString(0, 0, "ｹﾞｰﾑｵｰﾊﾞｰ");

		++mCount;
	}
}//namespace Sequence