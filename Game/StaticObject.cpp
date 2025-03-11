﻿#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Game/StaticObject.h"
#include "Image.h"

//デフォルト壁。壁にしておけば抜けることはない。
StaticObject::StaticObject() : 
	mFlags(0),
	mBombOwner(0),
	mCount(0)
{
}

void StaticObject::SetFlag(unsigned f) {
	mFlags |= f;
}

void StaticObject::resetFlag(unsigned f) {
	mFlags &= ~f;
}

bool StaticObject::checkFlag(unsigned f) const {
	//f番目のビットが立っているか？
	return (mFlags & f) ? true : false;
}

void StaticObject::draw(int x, int y, const Image* image) const {
	int srcX = -1; //ソース画像のx座標
	int srcY = -1; //ソース画像のy座標
	bool floor = false;

	if (mFlags & FLAG_WALL) {
		srcX = 48;
		srcY = 16;
	}
	else if (mFlags & FLAG_BRICK) {
		if (mFlags & (FLAG_FIRE_X | FLAG_FIRE_Y)) { //焼けてる
			srcX = 0;
			srcY = 48;
		}
		else {
			srcX = 0;
			srcY = 32;
		}
	}
	else {
		srcX = 16;
		srcY = 32;
		floor = true;
	}

	image->draw(x * 16, y * 16, srcX, srcY, 16, 16);
	
	//以下オプション描画
	if (floor) {
		srcX = -1; //番兵
		if ((mFlags & FLAG_BOMB) && !(mFlags & FLAG_EXPLODING)) {
			srcX = 32;
			srcY = 32;
		}
		else if (mFlags & FLAG_ITEM_BOMB) {
			srcX = 32;
			srcY = 0;
		}
		else if (mFlags & FLAG_ITEM_POWER) {
			srcX = 48;
			srcY = 0;
		}
		if (srcX != -1) {
			image->draw(x * 16, y * 16, srcX, srcY, 16, 16);
		}
	}
}

void StaticObject::drawExplosion(int x, int y, const Image* image) const {
	int srcX = -1;
	int srcY = -1;
	if (!(mFlags & FLAG_WALL) && !(mFlags & FLAG_BRICK)) { //壁の上には爆風は描かない
		if (mFlags & FLAG_EXPLODING) {
			srcX = 48;
			srcY = 32;
		}
		else if (mFlags & FLAG_FIRE_X) {
			if (mFlags & FLAG_FIRE_Y) {
				//XもYも燃えてたら交差の画像にする
				srcX = 48;
				srcY = 32;
			}
			else {
				srcX = 0;
				srcY = 16;
			}
		}
		else if (mFlags & FLAG_FIRE_Y) {
			//XかつYは上で調べてる
			srcX = 16;
			srcY = 16;
		}
	}
	if (srcX != -1) {
		image->draw(x * 16, y * 16, srcX, srcY, 16, 16);
	}
}