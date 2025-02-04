#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"

#include "Image.h"

using namespace std;
using namespace GameLib;

namespace { //名前なし名前空間

	//マップの広さ
	const int WIDTH = 19;
	const int HEIGHT = 15;

	//適当なステージデータ
	struct StageData {
		int mEnemyNumber; //敵の数
		int mBrickRate; //レンガの確率(％)
		int mItemPowerNumber; //爆風アイテムの数
		int mItemBombNumber; //爆弾アイテムの数
	};

	StageData gStageData[] = {
		{2,90,4,6,},
		{3,80,1,0,},
		{6,30,0,1,},
	};
} //名前なし名前空間

State::State(int stageID) :
	mImage(0),
	mDynamicObjects(0),
	mDynamicObjectNumber(0),
	mStageID(stageID) {
	Framework f = Framework::instance(); //後で何度か使う、GetRandomなど
	mStaticObjects.setSize(WIDTH, HEIGHT);

	mImage = new Image("data/image/bakudanBitoImage.dds"); //全部が入っている画像を切り出して利用する

	const StageData& stageData = gStageData[mStageID];
	int n = HEIGHT * WIDTH; //マス目の総数

	//レンガのブロックを記録
	unsigned* brickList = new unsigned[n];
	//本当にレンガになった数をカウント
	int brickNumber = 0;
	//床も記録する
	unsigned* floorList = new unsigned[n];
	int floorNumber = 0;

	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			StaticObject& o = mStaticObjects(x, y);
			if (x == 0 || y == 0 || (x == WIDTH - 1) || (y == HEIGHT - 1)) {
				//端っこは全部コンクリート
				o.SetFlag(StaticObject::FLAG_WALL);
			}
			else if ((x % 2 == 0) && (y % 2 == 0)) {
				//縦と横を一つ飛ばしでコンクリート
				o.SetFlag(StaticObject::FLAG_WALL);
			}
			else if (y + x < 4) {
				//左上3マスは床なので何もしない
			}
			else if ((mStageID == 0) && ((y + x) > (WIDTH + HEIGHT - 6))) {
				//2人用なら、右下3マスも空けておく。よって何もしない
			}
			else {
				//残りはレンガか床である。100面サイコロを振って決める。
				//床の場合は処理がいらないので、レンガのif文のみ。

				if (f.getRandom(100) < stageData.mBrickRate) {
					o.SetFlag(StaticObject::FLAG_BRICK);

					//レンガだったら記録しておく。
					brickList[brickNumber] = (x << 16) + y;//xとyの構造体を作るのは面倒なので、xとyを同時に記録できる方法を採用。32bitの変数を16ビットずつ使ってxとyを同時に記録している。
					++brickNumber;
				}
				else {
					//プレイヤー周辺の3マス以外の床を記録して保持
					floorList[floorNumber] = (x << 16) + y;
					++floorNumber;
				}
			}
		}
	}

	//レンガにアイテムを仕込む。
	int powerNumber = stageData.mItemPowerNumber;
	int bombNumber = stageData.mItemBombNumber;

	//方法は、レンガリストの i 番目を適当なものと取り替えて、そこにアイテムを入れる。
	for (int i = 0; i < powerNumber + bombNumber; ++i) {
		int swapped = f.getRandom(brickNumber - 1 - i) + i;//自分か、自分より後ろと取り替える。こうしないと、既に入れたマスにもう一度アイテムを入れてしまう場合がある。
		
		//交換処理
		unsigned tmp = brickList[i];
		brickList[i] = brickList[swapped];
		brickList[swapped] = tmp;

		//x座標とy座標記録時に、1つの変数に16bitずつ記録していたので、それのperse
		int x = brickList[i] >> 16; //上位16ビットを取り出す
		int y = brickList[i] & 0xffff; //下位16ビットを取り出す

		StaticObject& o = mStaticObjects(x, y);
		if (i < powerNumber) {
			o.SetFlag(StaticObject::FLAG_ITEM_POWER);
		}
		else {
			o.SetFlag(StaticObject::FLAG_ITEM_BOMB);
		}
	}
	SAFE_DELETE_ARRAY(brickList);

	//動的オブジェクトを確保
	int playerNumber = (mStageID == 0) ? 2 : 1;
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[mDynamicObjectNumber];

	//プレイヤー配置。位置はゲーム開始時はいつも同じ
	mDynamicObjects[0].set(1, 1, DynamicObject::TYPE_1P);
	if (mStageID == 0) {
		//2Pもいる場合
		mDynamicObjects[1].set(WIDTH - 2, HEIGHT - 2, DynamicObject::TYPE_2P);
	}

	//床に敵を仕込む。やり方はアイテムとほとんど同じ。
	for (int i = 0; i < enemyNumber; ++i) {
		int swapped = f.getRandom(floorNumber - 1 - i) + i;

		//入れ替え
		unsigned tmp = floorList[i];
		floorList[i] = floorList[swapped];
		floorList[swapped] = tmp;

		int x = floorList[i] >> 16;
		int y = floorList[i] & 0xffff;
		//プレイヤーはすでに初期化している前提
		mDynamicObjects[playerNumber + i].set(x, y, DynamicObject::TYPE_ENEMY);
	}
	SAFE_DELETE_ARRAY(floorList);
}

State::~State() {
	SAFE_DELETE(mImage);
	SAFE_DELETE_ARRAY(mDynamicObjects);
}

void State::draw() const {
	//背景描画
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			mStaticObjects(x, y).draw(x, y, mImage); //staticObjectクラスは座標情報を持たないので、描画時に座標を渡す
		}
	}
	//前景描画
	for (int i = 0; i < mDynamicObjectNumber; ++i) {
		mDynamicObjects[i].draw(mImage);
	}
	//TODO:爆風描画
}

void State::update() {
	//TODO:
}

bool State::hasCleared() const {
	//TODO:
	return false;
}

bool State::isAlive1P() const {
	//TODO:
	return true;
}

bool State::isAlive2P() const {
	//TODO:
	return true;
}