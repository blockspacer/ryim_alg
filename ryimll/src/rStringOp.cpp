#include "rStringOp.h"

using namespace Ryim;


//////////////////////////////////////////////////////////////////////////
// brute force string search:
// O(patLen * txtLen)
//
// Bibliography:
// https://algs4.cs.princeton.edu/53substring/
//////////////////////////////////////////////////////////////////////////
int SubstringSearch::naive_string_matcher(const std::string &pat, const std::string &txt)
{
	int patLen = pat.length();
	int txtLen = txt.length();
	if (txtLen < patLen) return txtLen;
	int j;
	for (int i = 0; i < txtLen - patLen; i++)
	{
		for (j = 0; j < patLen; j++)
		{
			if (txt.at(i + j) != pat.at(j))
				break;
		}
		if (j == patLen)
			return i; // found
	}
	return txtLen;  // not found
}

//////////////////////////////////////////////////////////////////////////
// Knuth-Morris-Pratt:
// O(patLen + txtLen)
//
// Bibliography:
// https://algs4.cs.princeton.edu/53substring/
//////////////////////////////////////////////////////////////////////////

SubstringSearch::kmpMatcher::kmpMatcher(std::string& p) :m_pattern(p)
{
	m_patLen = m_pattern.size();
	m_LofPS.resize(m_patLen);
	computePrefixSuffix();
}

SubstringSearch::kmpMatcher::~kmpMatcher() {}

void SubstringSearch::kmpMatcher::setPattern(std::string& p)
{
	m_pattern.clear();
	m_pattern.assign(p.begin(), p.end());
	m_patLen = m_pattern.size();
	computePrefixSuffix();
}

bool SubstringSearch::kmpMatcher::KMPsearch(std::string txt, std::vector<int>& results)
{
	int txtLen = txt.length();
	if (txtLen < m_patLen)
	{
		return false;
	}

	int q = -1;

	for (int i = 0; i < txtLen; ++i)
	{
		while (q >= 0 && m_pattern[q + 1] != txt[i])
		{
			q = m_LofPS[q];
		}

		if (m_pattern[q + 1] = txt[i])
			q++;

		if (q == m_patLen)
		{
			results.push_back(i - m_patLen);
			q = m_LofPS[q];
		}
	}
	return true;
}

void SubstringSearch::kmpMatcher::computePrefixSuffix()
{
	m_LofPS[0] = 0;
	int k = 0;

	for (int i = 0; i < m_patLen; ++i)
	{
		while (k > 0 && m_pattern[k + 1] != m_pattern[i])
		{
			k = m_LofPS[k];
		}
		if (m_pattern[k + 1] == m_pattern[i])
		{
			k++;
		}
		m_LofPS[i] = k;
	}
}


/////////////////////////////////////////////////////////
// RegularExp��:������ʽ������
// clear:��������������ʽ������
// NFA:����������ʽ����NFA(��ȷ������״̬�Զ���)
// Recognize:�ж�Ŀ���ı��Ƿ�ƥ��������ʽ
//////////////////////////////////////////////////////

	// ���캯��  
Regexp::RegularExp::RegularExp() :G(nullptr) {}
// ��������  
Regexp::RegularExp::~RegularExp() { clear(); }

// �ӿں���  
void Regexp::RegularExp::clear()
{
	if (G != nullptr)
	{
		delete G;
		G = nullptr;
	}
}
void Regexp::RegularExp::NFA(std::string regexp)
{
	std::stack <int> ops;
	// ����������ʽ����ȡ�䳤��
	re = regexp;
	M = regexp.length();
	// �����µ�����ͼ
	G = new DirectedGraph;
	G->initial(M + 1);

	int orOper = 0;
	int lp = 0;
	// ��������������ʽ
	for (int i = 0; i < M; i++) {
		lp = i;     // ������ʼλ��
					// ����'('��'|'�򽫸�λ��ѹ��ջ�У��������������
		if (re[i] == '(' || re[i] == '|')
		{
			ops.push(i);
		}
		else if (re[i] == ')') // �������
		{
			orOper = ops.top();
			ops.pop();
			// ������������'|'���������
			if (re[orOper] == '|') {
				lp = ops.top();
				ops.pop();
				// ���ӻ����
				G->addEdge(lp, orOper + 1);
				G->addEdge(orOper, i);
			}
			else
				lp = orOper;
		}

		// ������һλΪ�հ�����ʱ
		if (i < M - 1 && re[i + 1] == '*') {
			// ����˫������
			G->addEdge(lp, i + 1);
			G->addEdge(i + 1, lp);
		}

		// �ض�����ֱ��������һλ
		if (re[i] == '(' || re[i] == '*' || re[i] == ')')
			G->addEdge(i, i + 1);
	}
}

bool Regexp::RegularExp::Recognize(std::string txt)
{
	// ����Bag
	std::vector<int> pc;
	// ����DFS������
	DirectedDFS *dfs = new DirectedDFS();
	// ��������ͼG��ʼ��DFS������
	dfs->init(G->V());
	// �ӵ�һ�����㿪ʼ����DFS����
	dfs->dfs(G, 0);

	// ���ν��ӵ�һ������ɴﶥ�����Bag
	for (int v = 0; v < G->V(); v++)
		if (dfs->marked(v))
			pc.push_back(v);

	// ���������ı�
	std::vector<int> match;
	for (int i = 0; i < txt.length(); i++)
	{
		match.clear();
		// ����ƥ��Bag
		// �������пɴﶥ��
		for (int v = 0; v < pc.size(); v++)
			if (pc[v] < M)
				if (re[pc[v]] == txt[i] || re[pc[v]] == '.') // ��ȡ���п��ܵ�ƥ�䲢����match
					match.push_back(pc[v] + 1);

		// ����Bag�Լ�DFS������
		pc.clear();
		dfs->clearState(G->V());
		// ͨ������ͼG��ʼ��DFS������
		//dfs->init(G->V());
		// ������ƥ���ı���Ӧλ�õĽڵ����DFS����
		for (int i = 0; i < match.size(); i++)
			dfs->dfs(G, match[i]);

		// ���ν����пɴ�ڵ����Bag
		for (int v = 0; v < G->V(); v++)
			if (dfs->marked(v))
				pc.push_back(v);
	}

	// �������пɴ�ڵ�
	for (int i = 0; i < pc.size(); i++)
		if (pc[i] == M) // �ж��Ƿ�ƥ��������ʽ
			return true;

	return false;
}