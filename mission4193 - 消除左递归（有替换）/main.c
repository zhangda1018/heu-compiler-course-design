#include "RemoveLeftRecursion.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

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
	// ���������ݹ�֮ǰ���ķ�
	//
	printf("Before Remove Left Recursion:\n");
	PrintRule(pHead);

	//
	// ���� RemoveLeftRecursion ���������ķ��е���ݹ�
	//
	RemoveLeftRecursion(pHead);
	
	//
	// ���������ݹ�֮����ķ�
	//
	printf("\nAfter Remove Left Recursion:\n");
	PrintRule(pHead);
	
	return 0;
}

/*
���ܣ�
	�жϵ�ǰ Rule �е�һ�� Symbol �Ƿ���Ҫ���滻��
	��� Symbol ��һ�����ս������ Symbol ��Ӧ��
	Rule �ڵ�ǰ Rule ֮ǰ������Ҫ���滻��

������
	pCurRule -- ��ǰ Rule ��ָ�롣
	pSymbol -- Symbol ָ�롣
	pHead -- Rule ��ͷָ�롣
	  
����ֵ��
	��Ҫ�滻���� 1��
	����Ҫ�滻���� 0��
*/
int SymbolNeedReplace(Rule* pHead, const Rule* pCurRule, RuleSymbol* pSymbol)
{
	// �����ǰ Symbol ���ս���Ļ�����һ������Ҫ�滻
	if (1 == pSymbol->isToken)
	{
		return 0;
	}
	Rule* pRule = pHead;
	// �����ڵ�ǰ Rule ֮ǰ�Ѿ�������� Rule
	while (pRule != pCurRule)
	{
		// �����ǰ Rule �������뵱ǰ Symbol ��ָ��� Rule ������ͬ���������Ҫ�滻
		if (pSymbol->pRule->RuleName == pRule->RuleName)
		{
			return 1;
		}
		pRule = pRule->pNextRule;
	}
	return 0;	
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
	�滻һ�� Select �ĵ�һ�� Symbol��

������
	pSelectTemplate -- ��Ҫ���滻�� Select ָ�롣
	  
����ֵ��
	�滻���õ��� Select ��ָ�롣
	ע�⣬�滻����ܻ���һ���µ� Select��
	Ҳ���ܻ��ж�� Select ������һ��
*/
RuleSymbol* ReplaceSelect(RuleSymbol* pSelectTemplate)
{
	// result Ϊ�µ� Select ��ͷ�ڵ�ָ��
	RuleSymbol *result = NULL;
	// pNextSelect ָ��ָ���µ� Select ָ��洢��λ��
	RuleSymbol **pNextSelect;
	// ���Ϊ���ս����������滻
	if (pSelectTemplate->isToken == 0)
	{
		RuleSymbol *pSelect = pSelectTemplate->pRule->pFirstSymbol;
		// ������ָ��� Rule ������ Select
		while (pSelect != NULL)
		{
			// �����ڱ����� Select ����һ��
			RuleSymbol *pNewSelect = CopySelect(pSelect);
			// �����滻���ֵĺ�� Symbol ������һ��
			RuleSymbol *pNewSelectTail = CopySelect(pSelectTemplate->pNextSymbol);
			// �����ƺõ� Select �� pNewSelectTail ��������
			AddSymbolToSelect(pNewSelect, pNewSelectTail);
			// ���ɵ� Select ��Ȼ���µ� Select �����ĩβ
			pNewSelect->pOther = NULL;
			// ��� result Ϊ�գ���ֱ�ӽ� result �滻Ϊ��ǰ�ڵ�
			if (result == NULL)
			{
				result = pNewSelect;
				pNextSelect = &(pNewSelect->pOther);
			}
			// ��� result �ǿգ�������ӵ�����ĩβ
			else
			{
				*pNextSelect = pNewSelect;
				pNextSelect = &(pNewSelect->pOther);
			}
			pSelect = pSelect->pOther;
		}
		
	}
	// ���Ϊ�ս�����������滻��ֱ�Ӹ���һ��֮�󷵻ؼ���
	else
	{
		result = CopySymbol(pSelectTemplate);
	}
	return result; 
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
	�ж�һ�� Rule �Ƿ������ݹ顣

������
	prRule -- Rule ָ�롣
	  
����ֵ��
	���ڷ��� 1��
	�����ڷ��� 0��
*/
int RuleHasLeftRecursion(Rule* pRule)
{
	RuleSymbol *pSelect = pRule->pFirstSymbol;
	// �������е� Select
	while (pSelect != NULL)
	{
		// �����ǰ Select ָ��� Rule ��������� Rule������������ݹ�
		if (pSelect->isToken == 0 && pSelect->pRule->RuleName == pRule->RuleName)
		{
			return 1;
		}
		pSelect = pSelect->pOther;
	}
	return 0;
}

/*
���ܣ�
	��һ�� Symbol ��ӵ� Select ��ĩβ��

������
	pSelect -- Select ָ�롣
	pNewSymbol -- Symbol ָ�롣
*/
void AddSymbolToSelect(RuleSymbol* pSelect, RuleSymbol* pNewSymbol)
{
	RuleSymbol *pSymbol = pSelect;
	// ����������һ�� Symbol Ϊ�յ� Symbol ָ�룬����ǰ Select ��ĩβ Symbol
	while (pSymbol != NULL && pSymbol->pNextSymbol != NULL)
	{
		pSymbol = pSymbol->pNextSymbol;
	}
	// ���µ� Symbol ���뵽ĩβ Symbol �ĺ���
	pSymbol->pNextSymbol = pNewSymbol;
	pNewSymbol->pNextSymbol = NULL;
	//assert(pSymbol->pNextSymbol != 0x1);
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
	}
	else
	{
		// ���µ� Select ���뵽��ǰ�� Rule ��
		pRule->pFirstSymbol = pNewSelect;
	}
}

/*
���ܣ�
	������ݹ顣

������
	pHead -- �ķ������ͷָ�롣
*/
void RemoveLeftRecursion(Rule* pHead)
{
	Rule* pRule;				// Rule �α�
	RuleSymbol* pSelect; 		// Select �α�
	Rule* pNewRule;			  	// Rule ָ��
	int isChange;				// Rule �Ƿ��滻�ı��
	RuleSymbol **pSelectPrePtr; // Symbol ָ���ָ��
	
	for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		//
		// �滻
		//
		do
		{
			isChange = 0;

			// �� Rule ������ Select �в����Ƿ���Ҫ�滻
			for (pSelect = pRule->pFirstSymbol, pSelectPrePtr = &pRule->pFirstSymbol;
				pSelect != NULL;
				pSelectPrePtr = &pSelect->pOther, pSelect = pSelect->pOther)
			{
				if (SymbolNeedReplace(pHead, pRule, pSelect)) // �ж� Select �ĵ�һ�� Symbol �Ƿ���Ҫ�滻
				{
					isChange = 1;

					// ���� ReplaceSelect �������滻 Select �ĵ�һ�� Symbol ��õ��µ� Selects
					RuleSymbol* pNewSelects = ReplaceSelect(pSelect);
					// ʹ���µ� Selects �滻ԭ�е� Select�������� FreeSelect �����ͷ�ԭ�е� Select �ڴ�
					RuleSymbol* pNewSelectsTail = pNewSelects;
					while (pNewSelectsTail->pOther)
					{
						pNewSelectsTail = pNewSelectsTail->pOther;
					}
					// ���µ� Selects ��ĩβ���ӵ�ǰ Select ����һ�� Select
					pNewSelectsTail->pOther = pSelect->pOther;
					// ����һ�� Select ��ĩβ���ӵ�ǰ Select
					*pSelectPrePtr = pNewSelects;
					// �ݴ浱ǰ Select ��ָ��
					RuleSymbol *pTemp = pSelect;
					// ����ǰ Select ����Ϊ�µ� Select
					pSelect = pNewSelects;
					// �ͷű��滻�� Select ��ռ�õ��ڴ�
					FreeSelect(pTemp);
					break;
				}

				if (isChange)
					break;
			}
		} while (isChange);

		// ����û����ݹ�� Rule;
		if (!RuleHasLeftRecursion(pRule))
			continue;

		//
		// ������ݹ�
		//
		pNewRule = CreateRule(pRule->RuleName); // ������ Rule
		strcat(pNewRule->RuleName, Postfix);

		pSelect = pRule->pFirstSymbol; // ��ʼ�� Select �α�
		pSelectPrePtr = &pRule->pFirstSymbol;
		while (pSelect != NULL) // ѭ���������е� Select
		{
			if (0 == pSelect->isToken && pSelect->pRule == pRule) // Select ������ݹ�
			{
				// �Ƴ�������ݹ�� Select������ת��Ϊ�ҵݹ����ӵ��� Rule ��ĩβ�����ƶ��α�
				// ����ǰ Select �ĵڶ��� Symbol �����̸���һ�ݣ���Ϊ�µ� Select �Ŀ�ʼ
				RuleSymbol *pNewSelect = CopySelect(pSelect->pNextSymbol);
				// ����ǰ Select ��ͷ�ڵ㸴��һ�ݣ���Ϊ�µ� Select ��ĩβ
				RuleSymbol *pNewSelectTail = CopySymbol(pSelect);
				
				// �����µ� Select ��ĩβ�Ĳ���
				pNewSelectTail->pRule = pNewRule;
				pNewSelectTail->pNextSymbol = NULL;
				pNewSelectTail->pOther = NULL;
				
				// ���µ� Select ��ĩβ������
				AddSymbolToSelect(pNewSelect, pNewSelectTail);
				// �����µ� Select �Ĳ���
				pNewSelect->pOther = NULL;
				// ���µ� Select ���뵽�µ� Rule ��
				AddSelectToRule(pNewRule, pNewSelect);
				
				// �ݴ�һ��ԭʼ Select ��ָ�뱸��
				RuleSymbol *pTemp = pSelect;
				// ���´洢ָ����һ���ڵ����һ���ڵ��ָ��
				*pSelectPrePtr = pSelect->pOther;
				// ����ǰ�ڵ����Ϊ��ǰ�ڵ����һ���ڵ�
				pSelect = pSelect->pOther;
				
				// ����ݴ�� Select ��ռ���ڴ�
				FreeSelect(pTemp);
			}
			else // Select ��������ݹ�
			{
				// ��û����ݹ�� Select ĩβ���ָ���� Rule �ķ��ս�������ƶ��α�
				// ����һ���µķ��ս�� Symbol�������ò���
				RuleSymbol *pNewSymbol = CreateSymbol();
				pNewSymbol->pNextSymbol = NULL;
				pNewSymbol->pOther = NULL;
				pNewSymbol->isToken = 0;
				pNewSymbol->pRule = pNewRule;
				
				// ���µķ��ս�����뵽��ǰ Select ��ĩβ
				AddSymbolToSelect(pSelect, pNewSymbol);
				
				// ����ָ��洢ָ����һ���ڵ����һ���ڵ��ָ���ָ��
				pSelectPrePtr = &(pSelect->pOther);
				// ���µ�ǰ Select
				pSelect = pSelect->pOther;
			}
		}

		// ���� Rule ���������(�� '$' ����)
		// ���� Rule �����ķ�����
		// ����һ���µ��ս���ţ������ò���
		RuleSymbol *pNewSymbol = CreateSymbol();
		pNewSymbol->pNextSymbol = NULL;
		pNewSymbol->pOther = NULL;
		pNewSymbol->isToken = 1;
		strcpy(pNewSymbol->TokenName, VoidSymbol);
		
		// ���� Rule ���������
		AddSelectToRule(pNewRule, pNewSymbol);
		
		pNewRule->pNextRule = pRule->pNextRule;
		pRule->pNextRule = pNewRule;
		pRule = pNewRule;
	}
}

/*
���ܣ�
	ʹ�ø��������ݳ�ʼ���ķ�����

����ֵ��
	Rule ָ��
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
	/* A -> Ba | Aa | c */
	{ "A", 
			{
				{ { 0, "B" }, { 1, "a"} },
				{ { 0, "A" }, { 1, "a"} },
				{ { 1, "c" } }
			}	
	},

	/* B -> Bb | Ab | d */
	{ "B", 
			{
				{ { 0, "B" }, { 1, "b"} },
				{ { 0, "A" }, { 1, "b"} },
				{ { 1, "d" } }
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
