#include "RegexpToNFA.h"
#include "RegexpToPost.h"
#include "NFAFragmentStack.h"
#include "OutputResult.h"
#include <stdlib.h>

NFAFragmentStack FragmentStack;	// ջ�����ڴ��� NFA Ƭ��

// char* regexp = "ab";		// �� 1
// char* regexp = "a|b";	// �� 2
// char* regexp = "a*";		// �� 3
// char* regexp = "a?";		// �� 4
// char* regexp = "a+";		// �� 5
// char* regexp = "a(a|1)*";// �� 6
// char* regexp = "(aa|b)*a(a|bb)*";// �� 7
char* regexp = "(a|b)*a(a|b)?"; 	// �� 8

char regexp_ci[256];
// NodeList �洢���нڵ��ָ��
NFAState* NodeList[256];
// cnt ��ʾ�ڵ�����
int cnt = 0;

int main(int argc, char **argv)
{
	char *post;
	NFAState *start;
	

	// �� CP Lab��ִ�г���ʱ����ʹ������궨������Ĵ��룬�ύ��ҵ��������������ˮ��ʱ�Ż�ʹ�ú궨������Ĵ��롣
	// ����궨������Ĵ������������ˮ��ʱ��input.txt�ļ��ж�ȡ������ʽ��������������֤��
	// ���У�input1.txt �� input8.txt�ļ��а�����������ʽ����1 �� ��8��������ʽ�Ƕ�Ӧ�ġ�
#ifdef CODECODE_CI	
	scanf("%255s", regexp_ci);  
	regexp = regexp_ci;  
#endif
			
	//
	// ��ʼ��ջ
	//
	InitNFAFragmentStack(&FragmentStack);
	
	//
	// ���� re2post ������������ʽ�ַ���ת���ɽ������ĺ�����������
	//
	post = re2post(regexp);
	
	//
	// ���� post2nfa �������������ĺ�����������ת��Ϊ NFA �����ؿ�ʼ״̬
	//
	start = post2nfa(post);
	
	//
	// �� NFA ��ӡ���
	//
	OutputResult(start);
	
	//
	// ��� NFA
	//
	FreeNFA(start);
				
	return 0;
}

/*
���ܣ�
	��ʼ��һ��״̬��
	  
����ֵ��
	״ָ̬�롣
*/
int nstate = 1;  // ״̬��������
NFAState* CreateNFAState()
{
	NFAState *s = (NFAState*)malloc(sizeof(NFAState));

	s->Name = nstate++;
	s->Transform = '\0';
	s->Next1 = NULL;
	s->Next2 = NULL;
	s->AcceptFlag = 0;
	
	// ���½ڵ��ָ������б���
	NodeList[++cnt] = s;
	
	return s;
}

/*
���ܣ�
	����ʼ״̬�ͽ���״̬���һ�� Fragment��
	
������
	StartState -- ��ʼ״̬��
	AcceptState -- ����״̬��	
	  
����ֵ��
	 Fragment �ṹ��ָ�롣
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
const char VoidTrans = '$'; // ��ʾ��ת��
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

void FreeNFA(NFAState* start)
{
	int i;
	for (i = 1; i <= cnt; i++)
	{
		// �������нڵ㲢���
		free(NodeList[i]);
	}
}
