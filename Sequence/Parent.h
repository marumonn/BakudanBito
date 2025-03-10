﻿#ifndef INCLUDED_SEQUENCE_PARENT_H
#define INCLUDED_SEQUENCE_PARENT_H

namespace Sequence {
	namespace Game {
		class Parent;
	}
	//あとでポインタ変数が使えるように宣言
	class Title;
	class GameOver;
	class Ending;

	class Parent {
	public:
		enum NextSequence {
			NEXT_TITLE,
			NEXT_GAME,
			NEXt_GAME_OVER,
			NEXT_ENDING,

			NEXT_NONE,
		};

		enum Mode {
			MODE_1P,
			MODE_2P,
			MODE_NONE,
		};

		void update();
		//遷移したいシーケンスのEnumを引数に渡す
		void moveTo(NextSequence);
		Mode mode() const;
		void setMode(Mode);

		static void create();
		static void destroy();
		static Parent* instance();

	private:
		Parent();
		~Parent();

		NextSequence mNextSequence; //次に行きたいシーケンス
		Mode mMode;

		Title* mTitle;
		Game::Parent* mGame;
		GameOver* mGameOver;
		Ending* mEnding;

		static Parent* mInstance;
	};
} //namespace Sequence

#endif