#ifndef CPP_SOLUTION_PARSER_H
#define CPP_SOLUTION_PARSER_H

#include "util.h"
#include "expression.h"

class parser {
public:	
	enum ParserType {NONE, VAR, CON, DIS, NEG, IMPL, LBRACKET, RBRACKET};
	ParserType type;
	std::string varName, expr;
	swag ii;
	expression * parse(std::string s);
	char nextChar();
	void get();
	expression * high();
	expression * impl();
	expression * conj();
	expression * disj();
};

#endif //CPP_SOLUTION_PARSER_H
