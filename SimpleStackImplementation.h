// 簡易スタック クラステンプレート（ソース部）

#ifndef ___Class_SimpleStackImplementation
#define ___Class_SimpleStackImplementation

//--- 明示的コンストラクタ ---//
template<class Type>
SimpleStack<Type>::SimpleStack(int sz) : size(sz), ptr(0)
{
	stk = new Type[size];
}

//--- デストラクタ ---//
template<class Type>
SimpleStack<Type>::~SimpleStack()
{
	delete[] stk;
}

//--- プッシュ ---//
template<class Type>
Type& SimpleStack<Type>::push(const Type& x)
{							
	if (ptr >= size)								// スタックが満杯
		//throw Overflow();
	    printf("Error: SimpleStack Overflow!\n");
	return stk[ptr++] = x;
}

//--- ポップ ---//
template<class Type>
Type SimpleStack<Type>::pop()
{
	if (ptr <= 0)									// スタックは空
	    printf("Error: SimpleStack Empty!\n");
	//		throw Empty();
	return stk[--ptr];
}

#endif
