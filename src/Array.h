// 配列クラステンプレートArray<>

#ifndef ___ClassTemplate_Array
#define ___ClassTemplate_Array

//===== 配列クラステンプレート =====//
template <class Type> class Array {
	int nelem;		// 配列の要素数
	Type* vec;		// 先頭要素へのポインタ

	//--- 添字の妥当性を判定 ---//
	bool is_valid_index(int idx) { return idx >= 0 && idx < nelem; }

public:
	//----- 添字範囲エラー -----//
	class IdxRngErr {
		const Array* ident;
		int idx;
	public:
		IdxRngErr(const Array* p, int i) : ident(p), idx(i) { }
		int Index() const { return idx; }
	};

	//--- 明示的コンストラクタ ---//
	explicit Array(int size, const Type& v = Type()) : nelem(size) {
		vec = new Type[nelem];
		for (int i = 0; i < nelem; i++)
			vec[i] = v;
	}

	//--- コピーコンストラクタ ---//
	Array(const Array<Type>& x) {
		if (&x == this) {						// 初期化子が自分自身であれば…
			nelem = 0;
			vec = NULL;
		} else {
			nelem = x.nelem;					// 要素数をxと同じにする
			vec = new Type[nelem];				// 配列本体を確保
			for (int i = 0; i < nelem; i++)		// 全要素をコピー
				vec[i] = x.vec[i];
		}
	}

	//--- デストラクタ ---//
    ~Array() { delete[] vec; }

	//--- 要素数を返す ---//
	int size() const { return nelem; }

	//--- 代入演算子= ---//
	Array& operator=(const Array<Type>& x) {
		if (&x != this) {						// 代入元が自分自身でなければ…
			if (nelem != x.nelem) {				// 代入前後の要素数が異なれば…
				delete[] vec;					// もともと確保していた領域を解放
				nelem = x.nelem;				// 新しい要素数
				vec = new Type[nelem];			// 新たに領域を確保
			}
			for (int i = 0; i < nelem; i++)		// 全要素をコピー
				vec[i] = x.vec[i];
		}
		return *this;
	}

	//--- 添字演算子[] ---//
	Type& operator[](int i)	{
		if (!is_valid_index(i))
			throw IdxRngErr(this, i);			// 添字範囲エラー送出
		return vec[i];
	}

	//--- const版添字演算子[] ---//
	const Type& operator[](int i) const {
		if (!is_valid_index(i))
			throw IdxRngErr(this, i);			// 添字範囲エラー送出
		return vec[i];
	}
};

#endif
