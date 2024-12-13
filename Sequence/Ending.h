#ifndef INCLUDE_SEQUENCE_ENDING_H
#define INCLUDE_SEQUENCE_ENDING_H

class Image;
//エンディング
namespace Sequence {
	class Parent;

	class Ending {
	public:
		Ending();
		~Ending();
		void update(Parent*);
	private:
		Image* mImage;
		int mCount;
	};
}//namespace Sequence
#endif
