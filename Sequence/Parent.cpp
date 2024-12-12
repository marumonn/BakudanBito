#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Title.h"

namespace Sequence {
	Parent* Parent::mInstance = 0;

	void Parent::create() {
		ASSERT(!mInstance); //0が入ると止まる
		mInstance = new Parent();
	}

	void Parent::destroy() {
		ASSERT(mInstance);
		SAFE_DELETE(mInstance);
	}
	//シングルトン
	Parent* Parent::instance() {
		return mInstance;
	}
	//メンバ変数の初期化
	//ポインタ系は、0を入れている
	Parent::Parent() :
		mNextSequence(NEXT_NONE),
		mMode(MODE_NONE),
		mTitle(0),
		mGame(0),
		mGameOver(0),
		mEnding(0) {
		//最初にタイトルを作る
		mTitle = new Title();
	}

	Parent::~Parent() {
		//残っていれば消す
		SAFE_DELETE(mTitle);
		SAFE_DELETE(mGame);
		SAFE_DELETE(mGameOver);
		SAFE_DELETE(mEnding);
	}

	void Parent::update() {
		if (mTitle) {
			mTitle->update(this);
		}
	}
}