#include "PostToNFA.h"
#include "NFAFragmentStack.h"
#include <stdlib.h>

NFAFragmentStack FragmentStack;

/*
���ܣ�
	��ʼ��һ�� NFA ״̬��

����ֵ��
	״̬�ṹ��ָ�롣
*/
int nstate = 1;
NFAState* CreateNFAState()
{
	NFAState* s = (NFAState*)malloc(sizeof(NFAState));

	s->Name = nstate++;
	s->Transform = '\0';
	s->Next1 = NULL;
	s->Next2 = NULL;
	s->AcceptFlag = 0;
	
	cnt++;
	NFAStateList[cnt] = s;
	
	return s;
}

/*
���ܣ�
	��ʼ�� NFAFragment �ṹ�塣
	
������
	StartState -- ��ʼ״̬��
	AcceptState -- ����״̬��	
	  
����ֵ��
	 NFAFragment �ṹ��ָ�롣
*/
NFAFragment MakeNFAFragment(NFAState *StartState, NFAState *AcceptState)
{
	NFAFragment n = {StartState, AcceptState};
	return n;
}

/*
���ܣ�
	���������ĺ�������ת��Ϊ NFA��

������
	postfix -- �������ĺ�������ָ�롣
	  
����ֵ��
	NFA �Ŀ�ʼ״ָ̬�롣
*/
NFAState* post2nfa(char *postfix)
{
	char *p;							   		// �α�
	NFAFragment fragment1, fragment2, fm;     	// NFA Ƭ�νṹ�����
	NFAFragment fragment = {0, 0};			   	// ��ʼ�����ڷ��ص� NFA Ƭ��
	NFAState *NewStartState, *NewAcceptState; 	// ��ʼ״̬�ͽ���״ָ̬��
	
	for (p=postfix; *p != '\0'; p++)
	{
		switch (*p)
		{
			default:	// ���쵥�ַ� NFA Ƭ��
				// ���� CreateNFAState �������������µ�״̬
				NewStartState = CreateNFAState();
				NewAcceptState = CreateNFAState();
				// ��ʼ״̬ͨ�����ַ�ת��������״̬
				NewStartState->Transform = *p;
				NewStartState->Next1 = NewAcceptState;
				NewAcceptState->AcceptFlag = 1;
				// ���� MakeNFAFragment ��������һ���µ� NFA Ƭ��, ����ջ
				fm = MakeNFAFragment(NewStartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);
				break;
			case '.':	// �������� NFA Ƭ��
				// ջ��������Ƭ�γ�ջ�������µ� NFA Ƭ��
				fragment2 = PopNFAFragment(&FragmentStack);
				fragment1 = PopNFAFragment(&FragmentStack);
				// Ƭ�� 1 ͨ����ת����Ƭ�� 2 ����
				fragment1.AcceptState->AcceptFlag = 0;
				fragment1.AcceptState->Next1 = fragment2.StartState;
				fragment1.AcceptState->Transform = VoidTrans;
				// ���� MakeNFAFragment ��������һ���µ� NFA Ƭ��, ����ջ
				fm = MakeNFAFragment(fragment1.StartState, fragment2.AcceptState);
				PushNFAFragment(&FragmentStack, fm);
				break;
			case '|':	// ����ѡ�� NFA Ƭ��
				
				//
				// TODO: �ڴ���Ӵ���
				//
				// ���ɿ�ʼ״̬�ͽ���״̬
				NewStartState = CreateNFAState();
				NewAcceptState = CreateNFAState();
				// ջ��������Ƭ�γ�ջ
				fragment2 = PopNFAFragment(&FragmentStack);
				fragment1 = PopNFAFragment(&FragmentStack);
				// ��ʼ״̬ͨ�� eplison ת����������ջ��Ƭ������
				NewStartState->Transform = VoidTrans;
				NewStartState->AcceptFlag = 0;
				NewStartState->Next1 = fragment1.StartState;
				NewStartState->Next2 = fragment2.StartState;
				// ������ջ��Ƭ��ͨ�� eplison ת�������״̬����
				fragment1.AcceptState->Transform = VoidTrans;
				fragment1.AcceptState->AcceptFlag = 0;
				fragment1.AcceptState->Next1 = NewAcceptState;
				fragment2.AcceptState->Transform = VoidTrans;
				fragment2.AcceptState->AcceptFlag = 0;
				fragment2.AcceptState->Next1 = NewAcceptState;
				// ���ý���״̬
				NewAcceptState->AcceptFlag = 1;
				// ���� MakeNFAFragment ��������һ���µ� NFA Ƭ��, ����ջ
				fm = MakeNFAFragment(NewStartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);
				break;
			case '*':	// �����Ǻ� NFA Ƭ��
				
				//
				// TODO: �ڴ���Ӵ���
				//
				// ���ɿ�ʼ״̬�ͽ���״̬
				NewStartState = CreateNFAState();
				NewAcceptState = CreateNFAState();
				// ջ����һ��Ƭ�γ�ջ
				fragment = PopNFAFragment(&FragmentStack);
				// ��ʼ״̬ͨ�� eplison ת�����ջ��Ƭ�κͽ���״̬����
				NewStartState->Transform = VoidTrans;
				NewStartState->AcceptFlag = 0;
				NewStartState->Next1 = fragment.StartState;
				NewStartState->Next2 = NewAcceptState;
				// ��ջ��Ƭ��ͨ�� eplison ת��������ͽ���״̬����
				fragment.AcceptState->Transform = VoidTrans;
				fragment.AcceptState->AcceptFlag = 0;
				fragment.AcceptState->Next1 = fragment.StartState;
				fragment.AcceptState->Next2 = NewAcceptState;
				// ���ý���״̬
				NewAcceptState->AcceptFlag = 1;
				// ���� MakeNFAFragment ��������һ���µ� NFA Ƭ��, ����ջ
				fm = MakeNFAFragment(NewStartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);				
				break;
			case '?':	// �����ʺ� NFA Ƭ��
				
				//
				// TODO: �ڴ���Ӵ���
				//
				// ���ɿ�ʼ״̬�ͽ���״̬
				NewStartState = CreateNFAState();
				NewAcceptState = CreateNFAState();
				// ջ����һ��Ƭ�γ�ջ
				fragment = PopNFAFragment(&FragmentStack);
				// ��ʼ״̬ͨ�� eplison ת�����ջ��Ƭ�κͽ���״̬����
				NewStartState->Transform = VoidTrans;
				NewStartState->AcceptFlag = 0;
				NewStartState->Next1 = fragment.StartState;
				NewStartState->Next2 = NewAcceptState;
				// ��ջ��Ƭ��ͨ�� eplison ת�������״̬����
				fragment.AcceptState->Transform = VoidTrans;
				fragment.AcceptState->AcceptFlag = 0;
				fragment.AcceptState->Next1 = NewAcceptState;
				// ���ý���״̬
				NewAcceptState->AcceptFlag = 1;
				// ���� MakeNFAFragment ��������һ���µ� NFA Ƭ��, ����ջ
				fm = MakeNFAFragment(NewStartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);		
				break;
			case '+':	// ����Ӻ� NFA Ƭ��
				
				//
				// TODO: �ڴ���Ӵ���
				//
				// ���ɽ���״̬
				NewAcceptState = CreateNFAState();
				// ջ����һ��Ƭ�γ�ջ
				fragment = PopNFAFragment(&FragmentStack);
				// ��ջ��Ƭ��ͨ�� eplison ת�������״̬����
				fragment.AcceptState->Transform = VoidTrans;
				fragment.AcceptState->AcceptFlag = 0;
				fragment.AcceptState->Next1 = NewAcceptState;
				// ����״̬ͨ�� eplison ת�����ջ��Ƭ������
				NewAcceptState->AcceptFlag = 1;
				NewAcceptState->Transform = VoidTrans;
				NewAcceptState->Next1 = fragment.StartState;
				// ���� MakeNFAFragment ��������һ���µ� NFA Ƭ��, ����ջ
				fm = MakeNFAFragment(fragment.StartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);
				break;

		}
	}
	
	//
	// �� NFA ���ս����ջ���浽 fragment ��
	//
	fragment = PopNFAFragment(&FragmentStack);

	return fragment.StartState;
}
