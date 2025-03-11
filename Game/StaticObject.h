#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;

//ゲーム上で動かないオブジェクトを表す
class StaticObject {
public:
	enum Flag {
		FLAG_WALL = (1 << 0), //コンクリート
		FLAG_BRICK = (1 << 1), //レンガ
		FLAG_BOMB = (1 << 2), //爆弾
		FLAG_ITEM_BOMB = (1 << 3), //爆弾アイテム
		FLAG_ITEM_POWER = (1 << 4), //爆風アイテム
		FLAG_FIRE_X = (1 << 5), //横方向炎
		FLAG_FIRE_Y = (1 << 6), //縦方向炎
		FLAG_EXPLODING = (1 << 7), //爆発中
	};
	StaticObject();

	//床、壁、レンガを描画し、爆弾やアイテムがあればそれも描画する
	void draw(int x, int y, const Image*) const;
	//爆風を描画
	void drawExplosion(int x, int y, const Image*) const;

	bool checkFlag(unsigned) const;
	void SetFlag(unsigned);
	void resetFlag(unsigned);

	int mCount; //なにかのカウント (爆弾設置、爆発、焼かれ始め)
	//爆弾専用
	DynamicObject* mBombOwner; //爆弾の持ち主
private:
	unsigned mFlags;
};

#endif