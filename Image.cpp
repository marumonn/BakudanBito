#include "Image.h"
#include "File.h"

#include "GameLib/Framework.h"
using namespace GameLib;

Image::Image(const char* filename) : 
mWidth(0),
mHeight(0),
mData(0)
{
	File f(filename);
	//12byte目から4byteのデータがheight
	mHeight = f.getUnsigned(12);
	mWidth = f.getUnsigned(16);
	mData = new unsigned[mWidth * mHeight];
	for (int i = 0; i < mWidth * mHeight; ++i) {
		//4バイトずつ配列に格納
		mData[i] = f.getUnsigned(128 + i * 4);
	}
}

Image::~Image() {
	SAFE_DELETE(mData);
}

int Image::width() const {
	return mWidth;
}

int Image::height() const {
	return mHeight;
}

unsigned blend(unsigned src, unsigned dst) {
	//ARGBで表されている
	
	//上位16ビットが全部1の0xff000000と論理積を取り、左シフトして、0-255に収める
	unsigned srcA = (src & 0xff000000) >> 24;

	unsigned srcR = src & 0xff0000;
	unsigned srcG = src & 0x00ff00;
	unsigned srcB = src & 0x0000ff;

	unsigned dstR = dst & 0xff0000;
	unsigned dstG = dst & 0x00ff00;
	unsigned dstB = dst & 0x0000ff;

	unsigned r = (srcR - dstR) * srcA / 255 + dstR;
	unsigned g = (srcG - dstG) * srcA / 255 + dstG;
	unsigned b = (srcB - dstB) * srcA / 255 + dstB;

	//rgbのそれぞれをひとつのunsigned int に論理和してreturn
	return (r & 0xff0000) | (g & 0x00ff00) | b;
}

//アルファブレンドに対応
void Image::draw(
	int dstX, //ウィンドウ中のどこに描画したいか
	int dstY,
	int srcX, //ソース画像の中で、どこの画素値に位置するか
	int srcY,
	int width,
	int height
) const {
	unsigned* vram = Framework::instance().videoMemory();
	int windowWidth = Framework::instance().width();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			//xとyが0なら、ソース画像の左上
			unsigned src = mData[(y + srcY) * mWidth + (x + srcX)];

			//描画位置の画素へのポインタ
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];
			*dst = blend(src , *dst);
		}
	}
}

void Image::draw() const {
	draw(0, 0, 0, 0, mWidth, mHeight);
}