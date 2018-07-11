#ifndef CPP_SOLUTION_DEDUCTION_H
#define CPP_SOLUTION_DEDUCTION_H
#include "parser.h"

class deduction {
public:
	std::vector<std::string> axiomsImpl = {
		"A->(B->A)",
        "(A->B)->(A->B->C)->(A->C)",
        "A->B->A&B",
        "(A&B)->A",
        "(A&B)->B",
        "A->(A|B)",
        "A->(B|A)",
        "(A->C)->(B->C)->((A|B)->C)",
        "(A->B)->(A->(!B))->(!A)",
        "(!!A)->A"
	};
	parser p;
	std::string lastHyp;
	std::vector<std::string> rebuild;
	std::vector<expression *> axioms;
	expression * lastHypExpr;
	std::unordered_map<expression *, swag, std::hash<expression*>, expression_equal> hypMap, used;
	std::unordered_map<expression *, std::vector<std::vector<expression *>>, std::hash<expression*>, expression_equal> mp;
	
	void printAx(std::vector<std::string> & result, std::string input);
	void printHyp(std::vector<std::string> & result, std::string input);
	void printMp(std::vector<std::string> & result, std::string input, std::string found);
	void prove(std::string exprStr, swag num, std::vector<std::string> & result);
	std::vector<std::string> apply(std::vector<std::string> & proof);
};


#endif //CPP_SOLUTION_DEDUCTION_H
