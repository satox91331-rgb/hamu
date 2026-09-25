// �X�^�b�N �N���X�e���v���[�g�i�v�f���Œ�̔z��ɂ������j

#ifndef ___Class_ArrayStack
#define ___Class_ArrayStack

#include "Stack.h"

//===== �v�f���Œ�̔z��ɂ��X�^�b�N �N���X�e���v���[�g =====//
template <class Type> class ArrayStack : public Stack<Type> {
	static const int size = 10;		// �X�^�b�N�̗e�ʁi�z��̗v�f���j
	int ptr;						// �X�^�b�N�|�C���^
	Type stk[size];					// �X�^�b�N�̖{��

public:
	//--- �R���X�g���N�^ ---//

	ArrayStack() : ptr(0), stk{ 0 } {}

	//--- �f�X�g���N�^ ---//
	~ArrayStack() { }

	//--- �v�b�V�� ---//
	void push(const Type& x) {
		if (ptr >= size)				// �X�^�b�N�͖��t
//satox			throw typename Stack<Type>::Overflow();
		stk[ptr++] = x;
	}

	//--- �|�b�v ---//
	Type pop() {
		if (ptr <= 0)					// �X�^�b�N�͋�
//satox						throw typename Stack<Type>::Empty();
		return stk[--ptr];
	}
};

#endif
