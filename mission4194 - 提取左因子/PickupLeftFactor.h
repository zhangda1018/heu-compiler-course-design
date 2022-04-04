#ifndef PICKUPLEFTFACTOR_H_
#define PICKUPLEFTFACTOR_H_

//
// �ڴ˴����� C ��׼��ͷ�ļ�
//

#include <stdio.h>

//
// �ڴ˴���������ͷ�ļ�
//

//
// �ڴ˴��������ݽṹ
//

#define MAX_STR_LENGTH 64

struct _Rule;
typedef struct _RuleSymbol
{
	struct _RuleSymbol* pNextSymbol; // ָ����һ�� Symbol
	struct _RuleSymbol* pOther;		 // ָ����һ�� Select
	int isToken;					 // �Ƿ�Ϊ�ս����1 ��ʾ�ս����0 ��ʾ���ս��
	char TokenName[MAX_STR_LENGTH];	 // �ս�������ơ�isToken Ϊ 1 ʱ�������Ч
	struct _Rule* pRule;			 // ָ�� Symbol ��Ӧ�� Rule��isToken Ϊ 0 ʱ�������Ч
}RuleSymbol;

typedef struct _Rule
{
	char RuleName[MAX_STR_LENGTH];	  // �ķ�������	
	struct _RuleSymbol* pFirstSymbol; // ָ���ķ��ĵ�һ�� Select �ĵ�һ�� Symbol
	struct _Rule* pNextRule;		  // ָ����һ���ķ�
}Rule;

//
// �ڴ˴���������
//

RuleSymbol* GetSymbol(RuleSymbol* pSelect, int index);
int LeftFactorMaxLength(RuleSymbol* pSelectTemplate);
int SymbolCmp(RuleSymbol* pRuleSymbol1, RuleSymbol* pRuleSymbol2);
int NeedPickup(RuleSymbol* pSelectTemplate, int Count, RuleSymbol* pSelect);
void AddSelectToRule(Rule* pRule, RuleSymbol* pRuleSymbol);
void GetUniqueRuleName(Rule* pHead, char* pRuleName);
void PickupLeftFactor(Rule* pHead);
void FreeSelect(RuleSymbol* pSelect);

Rule* InitRules();
Rule* InitRules_CI();
Rule* CreateRule(const char* pRuleName);
RuleSymbol* CreateSymbol();
Rule* FindRule(Rule* pHead, const char* RuleName);
void PrintRule(Rule* pHead);
RuleSymbol* CopySymbol(const RuleSymbol* pSymbolTemplate);
RuleSymbol* CopySelect(RuleSymbol* pSelectTemplate);

//
// �ڴ�����ȫ�ֱ���
//

extern const char* VoidSymbol;
extern const char* Postfix;

#endif /* PICKUPLEFTFACTOR_H_ */
