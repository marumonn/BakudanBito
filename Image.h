#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

class Image {
public:
	Image(const char* fileName);
	~Image();
	int width() const;
	int height() const;
	void draw(
		int dstX,//書く位置
		int dstY,
		int srcX,//画像のどこを切り出すか
		int srcY,
		int width,
		int height) const;
	void draw() const;
private:
	int mWidth;
	int mHeight;
	unsigned* mData;
};
#endif // !INCLUDED_IMAGE_H
