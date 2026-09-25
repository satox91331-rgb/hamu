// 簡易スタック クラステンプレート（ヘッダ部）

#ifndef ___Class_SimpleStack
#define ___Class_SimpleStack

//===== スタック クラステンプレート =====//
template<class Type> class SimpleStack {
	Type* stk;		// スタックの本体（先頭要素へのポインタ）
	int size;		// スタックの容量
	int ptr;		// スタックポインタ

	SimpleStack(const SimpleStack<Type>&);	  // コピーコンストラクタを無効化
	SimpleStack& operator=(const SimpleStack<Type>&);	// 代入演算子を無効化

public:
	//----- 満杯スタックへのプッシュに対する例外 -----//
	class Overflow { };

	//----- 空のスタックからのポップに対する例外 -----//
	class Empty { };

	// 明示的コンストラクタ
	explicit SimpleStack(int sz);

	// デストラクタ
	~SimpleStack();

	// プッシュ
	Type& push(const Type& x);

	// ポップ
	Type pop();
};

// ソース部をインクルード
#include "SimpleStackImplementation.h"

#endif
