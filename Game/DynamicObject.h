#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;
class StaticObject;

class DynamicObject {
public:
	//プレイヤーなのか、敵なのかなどの情報
	enum Type {
		TYPE_PLAYER,
		TYPE_ENEMY,

		TYPE_NONE, //死んでます
	};

	DynamicObject();

	//配列のnewではコンストラクタに引数を渡せないので、初期化のための関数を用意した
	void set(int x , int y , Type);

	void draw(const Image*) const;

	//移動
	void move(const int* wallsX, int* wallsY, int wallNumber);

	//どこのマスに中心があるのか尋ねる。2つの変数を返したいが、配列は定義したくないので、引数にポインタをとっている
	void getCell(int* x, int* y) const;

	//dx , dyだけ移動した際に重なっているマス座標を配列に入れて返す
	//引数はint[4]が２つ。戻り値は入れた数。1,2,4のどれか。
	int getIntersectionCell(int* xArray, int* yArray, int dx, int dy) const;

	//触っている最大4マスを受け取って応答を行う。
	void doCollisionReactionToStatic(StaticObject** o, int cellNumber);
	//動いているオブジェクトとの衝突時処理。
	void doCollisionReactionToDynamic(DynamicObject*);

	//DynamicObjectとの交差判定
	bool isIntersect(const DynamicObject&) const;

	//壁との交差判定
	bool isIntersectWall(int wallCellX, int wallCellY);

	//便利関数群
	
	//爆弾ボタンが押されたかどうか調べる
	bool hasBombButtonPressed() const;
	bool isPlayer() const;
	bool isEnemy() const;
	void die(); //死にます。(mTypeをNONEにすることで表現する)
	bool isDead() const; //死んでますか?

	//直接いじりたそうなものはprivateにしない。頻繁にStateから使うものであるためだ。
	Type mType;
	//プレイヤー専用
	int mBombPower; //爆発力
	int mBombNumber; //爆弾数
	int mPlayerID; //プレイヤー番号
	//爆弾を置く人専用
	int mLastBombX[2];
	int mLastBombY[2];

private:
	//壁との衝突検知。自由に座標を決定できるため、仮移動した結果との比較などに使う。
	static bool isIntersectWall(int x, int y, int wallCellX, int wallCellY);
	//現在フレームの移動量を取得
	void getVelocity(int* dx, int* dy) const;
	//移動方向を取得
	void getDirection(int* dx, int* dy) const;
	
	int mX; //座標(内部単位)
	int mY; //座標(内部単位)
	//敵専用
	int mDirectionX;
	//敵専用
	int mDirectionY;
};
#endif
