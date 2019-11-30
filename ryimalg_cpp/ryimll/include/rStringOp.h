#ifndef _R_STRING_OPERATION_H_
#define _R_STRING_OPERATION_H_

#include "rUtility.h"
#include "rGraph.h"
#include <vector>
#include <stack>
#include <string>


namespace Ryim
{
	namespace SubstringSearch
	{
		//////////////////////////////////////////////////////////////////////////
		// brute force string search:
		// O(patLen * txtLen)
		//
		// Bibliography:
		// https://algs4.cs.princeton.edu/53substring/
		//////////////////////////////////////////////////////////////////////////
		int naive_string_matcher(const std::string &pat, const std::string &txt);


		//////////////////////////////////////////////////////////////////////////
		// Knuth-Morris-Pratt:
		// O(patLen + txtLen)
		//
		// Bibliography:
		// https://algs4.cs.princeton.edu/53substring/
		//////////////////////////////////////////////////////////////////////////
		class kmpMatcher
		{
		public:
			explicit kmpMatcher(std::string& p);

			~kmpMatcher();

			void setPattern(std::string& p);


			bool KMPsearch(std::string txt, std::vector<int>& results);

			void computePrefixSuffix();

		private:
			std::vector<int> m_LofPS;
			std::string m_pattern;
			int m_patLen;
		};
	}

	// Regular Expression
	namespace Regexp
	{
		/////////////////////////////////////////////////////////
		// RegularExp��:������ʽ������
		// clear:��������������ʽ������
		// NFA:����������ʽ����NFA(��ȷ������״̬�Զ���)
		// Recognize:�ж�Ŀ���ı��Ƿ�ƥ��������ʽ
		//////////////////////////////////////////////////////

		class RegularExp
		{
		public:
			// ���캯��  
			RegularExp();
			// ��������  
			~RegularExp();

			void clear();

			void NFA(std::string regexp);

			bool Recognize(std::string txt);

		private:
			int M; // ����������ʽ�ַ�����  
			std::string re; // ����������ʽ  
			DirectedGraph *G; // ����NFA����������ͼ  
		};
	}
}// END Namespace Ryim
#endif