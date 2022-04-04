#include "PickupLeftFactor.h"
#include <string.h>
#include <stdlib.h>

const char* VoidSymbol = "$"; // "��"
const char* Postfix = "'";

char rule_table_ci[20][256];
char ruleNameArr[20][64];

int main(int argc, char* argv[])
{
	//
	// ���� InitRules ������ʼ���ķ�
	//
#ifdef CODECODE_CI
	Rule* pHead = InitRules_CI();  	// ���д�����������ˮ������
#else
	Rule* pHead = InitRules();		// ���д����� CP Lab ������
#endif

	//
	// �����ȡ������֮ǰ���ķ�
	//
	printf("Before Pickup Left Factor:\n");
	PrintRule(pHead);

	//
	// ���� PickupLeftFactor �������ķ���ȡ������
	//
	PickupLeftFactor(pHead);
	
	//
	// �����ȡ������֮����ķ�
	//
	printf("\nAfter Pickup Left Factor:\n");
	PrintRule(pHead);
	
	return 0;
}

/*
���ܣ�
	�����±��ҵ� Select �е�һ�� Symbol��

������
	pSelect -- Select ָ�롣
	index -- �±ꡣ
	  
����ֵ��
	������ڣ������ҵ��� Symbol ָ�룬���򷵻� NULL��
*/
RuleSymbol* GetSymbol(RuleSymbol* pSelect, int index)
{
	int i = 0;
	RuleSymbol* pRuleSymbol;
	for (pRuleSymbol = pSelect, i = 0; pRuleSymbol != NULL; 
		pRuleSymbol = pRuleSymbol->pNextSymbol, i++)
	{
		if (i == index)
		{
			return pRuleSymbol;
		}
	}

	return NULL;	
}

/*
���ܣ�
	�� SelectTemplate Ϊģ�壬ȷ�������ӵ���󳤶ȡ�

������
	pSelectTemplate -- ��Ϊģ��� Select ָ�롣
	  
����ֵ��
	�����ӵ���󳤶ȣ�������� 0 ˵�������������ӡ�
*/
int LeftFactorMaxLength(RuleSymbol* pSelectTemplate)
{
	int result = 0;									// result �洢�����ӵ���󳤶�
	RuleSymbol* pSelect = pSelectTemplate->pOther;	// �� pSelectTemplate ֮��� Select ��ʼ����
	while (pSelect != NULL)
	{
		RuleSymbol* pSymbol = pSelect;
		int length = 0; // length ��¼��ǰ Select �������ӱ�ƥ��ĳ���
		while (pSymbol != NULL)
		{
			// ͨ���±��ҵ�ģ���ж�Ӧ�� Symbol
			RuleSymbol* pFound = GetSymbol(pSelectTemplate, length);
			// ���δ�ҵ���ֱ������ѭ��
			if (pFound == NULL)
			{
				break;
			}
			// �ȽϽ��Ϊ 1 ֤����ǰ Symbol ��������ģ����ͬ��length �� 1 ֮���������
			if (SymbolCmp(pSymbol, pFound) == 1)
			{
				length++;
			}
			// ������Ϊ 0��֤����ǰ Symbol ��������ģ�岻ͬ������ѭ��
			else
			{
				break;
			}
			pSymbol = pSymbol->pNextSymbol;
		}
		// ��������
		if (length > result)
		{
			result = length;
		}
		pSelect = pSelect->pOther;
	}
	return result;
}

/*
���ܣ�
	�Ƚ�������ͬ����(ͬΪ�ս����ͬΪ���ս��)�� Symbol �Ƿ������ͬ�����֡�

������
	pSymbol1 -- Symbol ָ�롣
	pSymbol2 -- Symbol ָ�롣
	  
����ֵ��
	��ͬ���� 1����ͬ���� 0��
*/
int SymbolCmp(RuleSymbol* pSymbol1, RuleSymbol* pSymbol2)
{
	// ���Ϊ�ս������ֱ�ӱȽ������Ƿ���ͬ
	if (pSymbol1->isToken == 1)
	{
		if (pSymbol2->isToken == 1 && strcmp(pSymbol1->TokenName, pSymbol2->TokenName) == 0)
		{
			return 1;
		}
	}
	// ���Ϊ���ս������Ƚ϶�Ӧ Rule �������Ƿ���ͬ
	else
	{
		if (pSymbol2->isToken == 0 && pSymbol1->pRule != NULL && pSymbol2->pRule != NULL && strcmp(pSymbol1->pRule->RuleName, pSymbol2->pRule->RuleName) == 0)
		{
			return 1;
		}
	}
	return 0;
}

/*
���ܣ�
	ȡ�ķ��е�һ�� Select �� SelectTemplate ���бȽϣ��жϸ� Select �Ƿ���Ҫ��ȡ�����ӡ�

������
	pSelectTemplate -- ��Ϊģ��� Select ָ�롣
	Count -- SelectTemplate ����ȷ���������ӵ�������
	pSelect -- Select ָ�롣
	  
����ֵ��
	��� Select ���������ӷ��� 1�����򷵻� 0��
*/
int NeedPickup(RuleSymbol* pSelectTemplate, int Count, RuleSymbol* pSelect)
{
	int i;
	// ����ģ��͵�ǰ Select���Ƚ�ÿһ����Ӧλ�õ� Symbol �Ƿ���ͬ
	// �������ͬ��֤����Ҫ��ȡ������
	for (i = 0; i < Count; i++)
	{
		RuleSymbol *pSelect1 = GetSymbol(pSelectTemplate, i);
		RuleSymbol *pSelect2 = GetSymbol(pSelect, i);
		if (pSelect2 == NULL)
		{
			return 0;
		}
		if (SymbolCmp(pSelect1, pSelect2) == 0)
		{
			return 0;
		}
	}
	return 1;
}

/*
���ܣ�
	��һ�� Select ���뵽�ķ�ĩβ���� Select Ϊ NULL ʱ�ͽ�һ�����ս�����뵽�ķ�ĩβ��

������
	pRule -- �ķ�ָ�롣
	pNewSelect -- Select ָ�롣
*/
void AddSelectToRule(Rule* pRule, RuleSymbol* pNewSelect)
{
	RuleSymbol *pSelect;
	// ����µ� Select Ϊ���򴴽�һ���µĦ��ս��
	if (pNewSelect == NULL)
	{
		pNewSelect = CreateSymbol();
		pNewSelect->pNextSymbol = NULL;
		pNewSelect->pOther = NULL;
		pNewSelect->isToken = 1;
		strcpy(pNewSelect->TokenName, VoidSymbol);
	}
	pSelect = pRule->pFirstSymbol;
	// ����������һ�� Select Ϊ�յ� Select ָ�룬����ǰ Rule ��ĩβ Select
	while (pSelect != NULL && pSelect->pOther != NULL)
	{
		pSelect = pSelect->pOther;
	}
	// ��� pSelect �ǿ�
	if (pSelect != NULL)
	{
		// ���µ� Select ���뵽ĩβ Select �ĺ���
		pSelect->pOther = pNewSelect;
		pNewSelect->pOther = NULL;
	}
	else
	{
		// ���µ� Select ���뵽��ǰ�� Rule ��
		pRule->pFirstSymbol = pNewSelect;
		pNewSelect->pOther = NULL;
	}
}

/*
���ܣ�
	�� pRuleName ���ķ��е����� RuleName �Ƚ�, �����ͬ������һ����׺��

������
	pHead -- Rule �����ͷָ�롣
	pRuleName -- Rule �����֡�
*/
void GetUniqueRuleName(Rule* pHead, char* pRuleName)
{
	Rule* pRuleCursor = pHead;
	for (; pRuleCursor != NULL;)
	{
		if (0 == strcmp(pRuleCursor->RuleName, pRuleName))
		{
			strcat(pRuleName, Postfix);
			pRuleCursor = pHead;
			continue;
		}
		pRuleCursor = pRuleCursor->pNextRule;
	}	
}

/*
���ܣ�
	�ͷ�һ�� Select ���ڴ档

������
	pSelect -- ��Ҫ�ͷŵ� Select ��ָ�롣
*/
void FreeSelect(RuleSymbol* pSelect)
{
	RuleSymbol *pSymbol = pSelect;
	// ������ǰ Select �µ����� Symbol ������ɾ��
	while (pSymbol != NULL)
	{
		RuleSymbol *pTemp = pSymbol;
		pSymbol = pSymbol->pNextSymbol;
		free(pTemp);
	}
}

/*
���ܣ�
	����һ�� Symbol��

������
	pSymbolTemplate -- ��Ҫ�������� Symbol ָ�롣
	  
����ֵ��
	������õ��� Symbol ��ָ�롣
*/
RuleSymbol* CopySymbol(const RuleSymbol* pSymbolTemplate)
{
	if (pSymbolTemplate == NULL) 
	{
		return NULL;
	}
	// �����µ� Symbol
	RuleSymbol *newSymbol = CreateSymbol();
	// ������Ҫ�������� Symbol ������ Symbol ����Ϣ
	newSymbol->pNextSymbol = pSymbolTemplate->pNextSymbol;
	newSymbol->pOther = pSymbolTemplate->pOther;
	newSymbol->isToken = pSymbolTemplate->isToken;
	strcpy(newSymbol->TokenName, pSymbolTemplate->TokenName);
	newSymbol->pRule = pSymbolTemplate->pRule;
	return newSymbol;
}

/*
���ܣ�
	����һ�� Select��

������
	pSelectTemplate -- ��Ҫ�������� Select ָ�롣
	  
����ֵ��
	������õ��� Select ��ָ�롣
*/
RuleSymbol* CopySelect(RuleSymbol* pSelectTemplate)
{
	RuleSymbol *pSelect = NULL;				// �� Select �����ͷָ��
	RuleSymbol *pSymbol = pSelectTemplate;	// ��ǰ���ڱ������� Symbol
	RuleSymbol *pNewSymbol;					// �� Select �����ĩβԪ��ָ��
	// ����ԭ Select �е����� Symbol����ɸ���
	while (pSymbol != NULL)
	{
		// ���ͷָ��Ϊ�գ���ֱ���滻
		if (pSelect == NULL)
		{
			pSelect = CopySymbol(pSymbol);
			pNewSymbol = pSelect;
		}
		// ���ͷָ��ǿգ����µĽڵ�ӵ�����ĩβ
		else
		{
			pNewSymbol->pNextSymbol = CopySymbol(pSymbol);
			pNewSymbol = pNewSymbol->pNextSymbol;
		}
		pSymbol = pSymbol->pNextSymbol;
	}
	// ��ĩβ�ڵ����һ���ڵ�����Ϊ��
	pNewSymbol->pNextSymbol = NULL;
	return pSelect;
}

/*
���ܣ�
	��ȡ�����ӡ�

������
	pHead -- �ķ���ͷָ�롣
*/
void PickupLeftFactor(Rule* pHead)
{
	Rule* pRule;		    	 // Rule �α�
	int isChange;				 // Rule �Ƿ���ȡ�����ӵı�־
	RuleSymbol* pSelectTemplate; // Select �α�
	Rule* pNewRule; 			 // Rule ָ��
	RuleSymbol* pSelect;		 // Select �α�
	
	do
	{
		isChange = 0;

		for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
		{
			// ȡ Rule �е�һ�� Select ��Ϊģ�壬���� LeftFactorMaxLength ����ȷ�������ӵ���󳤶�
			int Count = 0;
			for (pSelectTemplate = pRule->pFirstSymbol; pSelectTemplate != NULL; pSelectTemplate = pSelectTemplate->pOther)
			{
				if ((Count = LeftFactorMaxLength(pSelectTemplate)) > 0)
					break;
			}

			// ����û�������ӵ� Rule
			if (Count == 0)
				continue; 

			pNewRule = CreateRule(pRule->RuleName); // ������ Rule
			GetUniqueRuleName(pRule, pNewRule->RuleName);
			isChange = 1; // ���ñ�־

			// ���� AddSelectToRule ������ģ��������֮��Ĳ��ּӵ��� Rule ��ĩβ
			// ��ģ��������֮��Ĳ����滻Ϊָ���� Rule �ķ��ս��
			AddSelectToRule(pNewRule, GetSymbol(pSelectTemplate, Count));
			// �洢�����ӵĽ�βָ��
			RuleSymbol *pSelectHead = GetSymbol(pSelectTemplate, Count - 1);
			// ����һ���µ�ָ���µ� Rule ���ս��
			RuleSymbol *pSelectTail = CreateSymbol();
			pSelectTail->isToken = 0;
			pSelectTail->pRule = pNewRule;
			pSelectTail->pOther = NULL;
			pSelectTail->pNextSymbol = NULL;
			// �������ӵ�ĩβ���µķ��ս����������
			pSelectHead->pNextSymbol = pSelectTail;

			// ��ģ��֮���λ��ѭ�����Ұ��������ӵ� Select������ȡ������
			pSelect = pSelectTemplate->pOther;
			RuleSymbol **pSelectPtr = &pSelectTemplate->pOther;
			while (pSelect != NULL)
			{
				if (NeedPickup(pSelectTemplate, Count, pSelect)) // Select ����������
				{
					// ���� AddSelectToRule ������������֮��Ĳ��ּӵ��� Rule ��ĩβ
					// ���� Select �� Rule ���Ƴ����ͷ��ڴ棬���ƶ��α�
					RuleSymbol *pSelectTail = CopySelect(GetSymbol(pSelect, Count));
					AddSelectToRule(pNewRule, pSelectTail);
					*pSelectPtr = pSelect->pOther; 
					FreeSelect(pSelect);
					pSelect = *pSelectPtr;
				}
				else // Select ������������
				{
					// �ƶ��α�
					pSelectPtr = &(pSelect->pOther);
					pSelect = pSelect->pOther;
				}
			}

			// ���� Rule ���뵽�ķ�����
			pNewRule->pNextRule = pRule->pNextRule;
			pRule->pNextRule = pNewRule;
			pRule = pRule->pNextRule;
		}

	} while (isChange == 1);
}

/*
���ܣ�
	ʹ�ø��������ݳ�ʼ���ķ�����

����ֵ��
	�ķ���ͷָ��
*/
typedef struct _SYMBOL
{
	int isToken;
	char Name[MAX_STR_LENGTH];
}SYMBOL;

typedef struct _RULE_ENTRY
{
	char RuleName[MAX_STR_LENGTH];
	SYMBOL Selects[64][64];
}RULE_ENTRY;

static const RULE_ENTRY rule_table[] =
{
	/* A -> abC | abcD | abcE */
	{ "A", 
			{
				{ { 1, "a" }, { 1, "b" }, { 1, "C" } },
				{ { 1, "a" }, { 1, "b" }, { 1, "c" }, { 1, "D" } },
				{ { 1, "a" }, { 1, "b" }, { 1, "c" }, { 1, "E" } }
			}	
	}
};

/*
���ܣ�
	��ʼ���ķ�����
	
����ֵ��
	�ķ���ͷָ�롣
*/
Rule* InitRules()
{
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr1, **pSymbolPtr2;
	// ���� rule_table ��ռ�ÿռ��һ�� rule ��ռ�ÿռ䣬���� rule ������
	int nRuleCount = sizeof(rule_table) / sizeof(rule_table[0]);
	int i, j, k;

	// pRulePtr �洢��ǰ��Ҫ������ rule ��ָ��ĵ�ַ
	Rule** pRulePtr = &pHead;
	// ����ÿһ�� rule
	for (i = 0; i < nRuleCount; i++)
	{
		// �������ƴ����µ� rule
		*pRulePtr = CreateRule(rule_table[i].RuleName);
		// ��ָ��ǰ rule ����һ�� rule ��ָ��ĵ�ַ�洢�� pRulePtr �У��Ա�������һ�� rule
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	// �������е� rule
	pRule = pHead;
	for (i = 0; i < nRuleCount; i++)
	{
		pSymbolPtr1 = &pRule->pFirstSymbol;
		// ������ǰ rule �ж���õ����� select
		for (j = 0; rule_table[i].Selects[j][0].Name[0] != '\0'; j++)
		{
			pSymbolPtr2 = pSymbolPtr1;
			// ������ǰ select �ж���õ����� rule
			for (k = 0; rule_table[i].Selects[j][k].Name[0] != '\0'; k++)
			{
				// �洢Ԥ�ȶ���� symbol ��Ϊ�������� symbol �ڵ�Ĳο�
				const SYMBOL* pSymbol = &rule_table[i].Selects[j][k];
				
				// �� pSymbolPtr2 ��ָ���ĵ�ַ�����µ� symbol �ڵ�
				*pSymbolPtr2 = CreateSymbol();
				// ���ݶ���� isToken ���ýڵ�� isToken
				(*pSymbolPtr2)->isToken = pSymbol->isToken;
				// ������ս������ֱ�ӽ����Ƹ��Ƶ� TokenName ��
				if (1 == pSymbol->isToken)
				{
					strcpy((*pSymbolPtr2)->TokenName, pSymbol->Name);
				}
				// ����Ƿ��ս���������ҵ���������Ӧ�Ľڵ��ٽ�������
				else
				{
					(*pSymbolPtr2)->pRule = FindRule(pHead, pSymbol->Name);
					if (NULL == (*pSymbolPtr2)->pRule)
					{
						printf("Init rules error, miss rule \"%s\"\n", pSymbol->Name);
						exit(1);
					}
				}
				// �� pSymbolPtr2 ��ֵ����Ϊ��һ�� symbol ָ��Ĵ洢��ַ
				pSymbolPtr2 = &(*pSymbolPtr2)->pNextSymbol;
			}
			// �� pSymbolPtr1 ��ֵ����Ϊ��һ�� select ָ��Ĵ洢��ַ
			pSymbolPtr1 = &(*pSymbolPtr1)->pOther;
		}
		
		pRule = pRule->pNextRule;
	}

	return pHead;
}

/*
���ܣ�
	��ʼ���ķ�����(��ִ����ˮ��ʱ����)��
	
����ֵ��
	�ķ���ͷָ�롣
*/
Rule* InitRules_CI()
{
	int nRuleCount = 0;
	for (int i = 0; i < 20; i++)
	{
		gets(rule_table_ci[i]);	
		int length = strlen(rule_table_ci[i]);
		if (length == 0)
		{
			break;
		}
		
		for (int j = 0; j < length; j++)
		{
			if (rule_table_ci[i][j] == ' ')
			{
				ruleNameArr[i][j] = '\0';
				break;
			}
			ruleNameArr[i][j]= rule_table_ci[i][j];
		}	  
		
		nRuleCount++;
	}
			
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr1, **pSymbolPtr2;
		
	int i, j, k;

	Rule** pRulePtr = &pHead;
	for (i=0; i<nRuleCount; i++)
	{
		*pRulePtr = CreateRule(ruleNameArr[i]);
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	pRule = pHead;
	for (i=0; i<nRuleCount; i++)
	{
		pSymbolPtr1 = &pRule->pFirstSymbol;
		
		int start = 0;
		for (int j=0; rule_table_ci[i][j] != '\0'; j++)
		{
			if (rule_table_ci[i][j] == ' '
			 && rule_table_ci[i][j + 1] == '-'
			&& rule_table_ci[i][j + 2] == '>' 
			&& rule_table_ci[i][j + 3] == ' ')
			{
				start = j + 4;
				break;
			}
		}
			
		for (k = start; rule_table_ci[i][k] != '\0'; k++)
		{
			if (rule_table_ci[i][k] == '|')
			{
				pSymbolPtr1 = &(*pSymbolPtr1)->pOther;
				pSymbolPtr2 = pSymbolPtr1;
				continue;
			}
			if (rule_table_ci[i][k] == ' ')
			{
				continue;
			}
			if (k == start)
			{
				pSymbolPtr2 = pSymbolPtr1;
			}

			*pSymbolPtr2 = CreateSymbol();
			
			char tokenName[MAX_STR_LENGTH] = {};
			tokenName[0] = rule_table_ci[i][k];
			tokenName[1] = '\0';
			(*pSymbolPtr2)->isToken = 1;
			for (int m = 0; m < nRuleCount; m++)
			{
				if (strcmp(tokenName, ruleNameArr[m]) == 0)
				{
					(*pSymbolPtr2)->isToken = 0;
					(*pSymbolPtr2)->pRule = FindRule(pHead, tokenName);
					if (NULL == (*pSymbolPtr2)->pRule)
					{
						printf("Init rules error, miss rule \"%s\"\n", tokenName);
						exit(1);
					}
				}
			}
			if ((*pSymbolPtr2)->isToken == 1)
			{
				strcpy((*pSymbolPtr2)->TokenName, tokenName);
			}
			
			pSymbolPtr2 = &(*pSymbolPtr2)->pNextSymbol;
			
		}
			
		pRule = pRule->pNextRule;
	}

	return pHead;
}

/*
���ܣ�
	����һ���µ� Rule��

������
	pRuleName -- �ķ������֡�
	
����ֵ��
	Rule ָ��	
*/
Rule* CreateRule(const char* pRuleName)
{
	Rule* pRule = (Rule*)malloc(sizeof(Rule));

	strcpy(pRule->RuleName, pRuleName);
	pRule->pFirstSymbol = NULL;
	pRule->pNextRule = NULL;

	return pRule;
}

/*
���ܣ�
	����һ���µ� Symbol��
	
����ֵ��
	RuleSymbol ָ��	
*/
RuleSymbol* CreateSymbol()
{
	RuleSymbol* pSymbol = (RuleSymbol*)malloc(sizeof(RuleSymbol));

	pSymbol->pNextSymbol = NULL;
	pSymbol->pOther = NULL;
	pSymbol->isToken = -1;
	pSymbol->TokenName[0] = '\0';
	pSymbol->pRule = NULL;

	return pSymbol;
}

/*
���ܣ�
	���� RuleName ���ķ������в���������ͬ���ķ���

������
	pHead -- �ķ���ͷָ�롣
	RuleName -- �ķ������֡�
	
����ֵ��
	Rule ָ��	
*/
Rule* FindRule(Rule* pHead, const char* RuleName)
{
	Rule* pRule;
	for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		if (0 == strcmp(pRule->RuleName, RuleName))
		{
			break;
		}
	}

	return pRule;
}

/*
���ܣ�
	����ķ���

������
	pHead -- �ķ���ͷָ�롣
*/
void PrintRule(Rule* pHead)
{
	// �������е� Rule
	while (pHead != NULL)
	{
		// ��� Rule ������
		printf("%s->", pHead->RuleName);
		RuleSymbol *pSelect = pHead->pFirstSymbol;
		// ������ǰ Rule �����е� Select
		while (pSelect != NULL)
		{
			RuleSymbol *pSymbol = pSelect;
			// ������ǰ Select �����е� Symbol
			while (pSymbol != NULL)
			{
				// ������ս������ֱ���������
				if (pSymbol->isToken == 1)
				{
					printf("%s", pSymbol->TokenName);
				}
				// ����Ƿ��ս�����������ָ�� Rule ������
				else
				{
					printf("%s", pSymbol->pRule->RuleName);
				}
				pSymbol = pSymbol->pNextSymbol;
			}
			pSelect = pSelect->pOther;
			if (pSelect != NULL)
			{
				printf("|");
			}
		}
		puts("");
		pHead = pHead->pNextRule;
	}
}
