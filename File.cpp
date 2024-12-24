#include "GameLib/GameLib.h"
#include "File.h"
#include <fstream>

using namespace std;

File::File(const char* fileName) : mSize(0), mData(0) {
	ifstream in(fileName, ifstream::binary);
	if (in) {
		in.seekg(0, ifstream::end); //最後に移動
		mSize = static_cast<int>(in.tellg()); //最後の位置を知らせて格納
		in.seekg(0, ifstream::beg); //最初に戻る
		mData = new char[mSize]; //領域確保
		in.read(mData, mSize); //読み込み
	}
	else {
		HALT("can't open file."); //致命傷だから止める
	}
}

File::~File() {
	SAFE_DELETE_ARRAY(mData);
}
//getter
int File::size() const {
	return mSize;
}
//getter
const char* File::data() const {
	return mData;
}

//unsigned取り出し
unsigned File::getUnsigned(int p) const {
	//charもunsigned charも 1バイト
	const unsigned char* up;
	up = reinterpret_cast<const unsigned char*>(mData);
	//unsigned intのデータを取り出すために、unsigned charのデータを8bitずつ取り込む
	unsigned r = up[p];
	r |= up[p + 1] << 8;
	r |= up[p + 2] << 16;
	r |= up[p + 3] << 24;
	return r;
}
