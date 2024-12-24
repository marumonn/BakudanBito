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
		else if (mGame) {
			mGame->update(this);
		}
		else if (mGameOver) {
			mGameOver->update(this);
		}
		else if (mEnding) {
			mEnding->update(this);
		}
		else {
			HALT("bakana");
		}

		//遷移判定
		//面セレクトへ

		switch (mNextSequence)
		{
		case NEXT_TITLE:
			//チェックして解放
			ASSERT(!mTitle && (mGame || mGameOver || mEnding));
			SAFE_DELETE(mGame);
			SAFE_DELETE(mGameOver);
			SAFE_DELETE(mEnding);
			mTitle = new Title();
			break;
		case NEXT_GAME:
			ASSERT(mTitle && !mGameOver && !mGame && !mEnding);
			SAFE_DELETE(mTitle);
			mGame = new Game::Parent();
			break;
		case NEXt_GAME_OVER:
			ASSERT(mGame && !mGameOver && !mEnding && !mTitle);
			SAFE_DELETE(mGame);
			mGameOver = new GameOver();
			break;
		case NEXT_ENDING:
			ASSERT(mGame && !mGameOver && !mEnding && !mTitle);
			SAFE_DELETE(mGame);
			mEnding = new Ending();
		}
		//シーケンスの更新が終わったら、戻す
		mNextSequence = NEXT_NONE; 
	}

	void Parent::moveTo(NextSequence next) {
		ASSERT(mNextSequence == NEXT_NONE); //これ以外ありえない
		mNextSequence = next;
	}

	void Parent::setMode(Mode mode) {
		mMode = mode;
	}

	Parent::Mode Parent::mode() const {
		return mMode;
	}

}//namespace Sequence