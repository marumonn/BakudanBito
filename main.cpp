#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Parent.h"

//ユーザ実装関数。中身はmainLoop()に丸投げ

namespace GameLib {
	void Framework::update() {
		if (!Sequence::Parent::instance()) {
			Sequence::Parent::create();
			setFrameRate(60);
		}
		Sequence::Parent::instance()->update();
		//終了判定( q が押されたか、マウスで×ボタンが押されたか
		if (isKeyOn('q')) {
			requestEnd();
		}
		if (isEndRequested()) {
			Sequence::Parent::destroy();
		}
	}
}//namespace GameLib