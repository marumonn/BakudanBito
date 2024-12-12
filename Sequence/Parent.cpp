#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Title.h"

namespace Sequence {
	Parent* Parent::mInstance = 0;

	void Parent::create() {
		ASSERT(!mInstance); //0������Ǝ~�܂�
		mInstance = new Parent();
	}

	void Parent::destroy() {
		ASSERT(mInstance);
		SAFE_DELETE(mInstance);
	}
	//�V���O���g��
	Parent* Parent::instance() {
		return mInstance;
	}
	//�����o�ϐ��̏�����
	//�|�C���^�n�́A0�����Ă���
	Parent::Parent() :
		mNextSequence(NEXT_NONE),
		mMode(MODE_NONE),
		mTitle(0),
		mGame(0),
		mGameOver(0),
		mEnding(0) {
		//�ŏ��Ƀ^�C�g�������
		mTitle = new Title();
	}

	Parent::~Parent() {
		//�c���Ă���Ώ���
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