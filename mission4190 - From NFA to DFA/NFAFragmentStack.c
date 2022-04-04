#include "NFAFragmentStack.h"


/*
���ܣ�
	��ʼ��ջ��

������
	pS -- ջ��ָ��
*/
void InitNFAFragmentStack(NFAFragmentStack* pS)
{
	pS->top = -1;
}

/*
���ܣ�
	��Ԫ����ջ��

������
	pS -- ջ��ָ��
	Elem -- ��ջ��Ԫ��
  
����ֵ��
	��
*/
void PushNFAFragment(NFAFragmentStack* pS, NFAFragment Elem)
{
	//
	// ջ������ջʧ�ܡ�
	//
	if (MAX_STACK_LENGTH - 1 <= pS->top)
	{
		return;
	}
		
	pS->top++;
	pS->buffer[pS->top] = Elem; // ��Ԫ�ز���ջ��
	
	return;
}

/*
���ܣ�
	��ջ��Ԫ�س�ջ

������
	pS -- ջ��ָ��
  
����ֵ��
	�����ջ�ɹ����س�ջԪ�ص�ֵ��
	�����ջʧ�ܷ��� -1��
*/
NFAFragment PopNFAFragment(NFAFragmentStack* pS)
{
	int pos;
	NFAFragment fragment = {0, 0};
	
	//
	// ջΪ�գ���ջʧ��
	//	
	if (NFAFragmentStackEmpty(pS))
	{
		return fragment;
	}
	
	pos = pS->top;
	pS->top--;
	
	return pS->buffer[pos];
}

/*
���ܣ�
	�ж�ջ�Ƿ�Ϊ�ա�

������
	pQ -- ջ��ָ��
  
����ֵ��
	���ջ�շ��� 1���棩
	���ջ�ǿշ��� 0���٣�
*/
int NFAFragmentStackEmpty(NFAFragmentStack* pS)
{
	return -1 == pS->top ? 1 : 0;
}

