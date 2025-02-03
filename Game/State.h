#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"

class Image; //宣言だけしておけば、この型のポインタ変数が宣言できる
class StaticObject;


class State {
public:
	//ステージ番号を与えてコンストラクトする。0だと２人用
	static const int STAGE_ID_2PLAYERS = 0;
	State(int stageID);
	~State();
	//入力は中でとる
	void update();
	void draw() const; //描画によって内部状態は変わらない

	//外からとる情報
	bool hasCleared() const;
	//1Pは生きてるか？
	bool isAlive1P() const;
	//2Pは生きてるか？
	bool isAlive2P() const;

private:
	//動かないオブジェクト
	Array2D<StaticObject> mStaticObjects;
	int mStageID;

	Image* mImage; //画像
};
#endif