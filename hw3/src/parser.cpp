#include "parser.h"

expression * parser::parse(std::string s) {
	type = NONE;
	varName = "";
	expr = s;
	ii = 0;
	return impl();
}

char parser::nextChar() {
	if(ii < expr.length()) {
		char c = expr[ii];
		ii++;
		return c;
	} else {
		return '$';
	}
}

void parser::get() {
	char c = nextChar();
	switch(c) {
		case '-':
			ii++;
			type = IMPL;
			break;
        case '&':
            type = CON;
            break;
        case '|':
            type = DIS;
            break;
        case '!':
            type = NEG;
            break;
        case '(':
            type = LBRACKET;
            break;
        case ')':
            type = RBRACKET;
            break;
        default:
        	type = VAR;
        	int lastII = ii;
        	while(ii < expr.length() && (isalpha(expr[ii]) || isdigit(expr[ii]))) {
        		ii++;
        	}
        	varName = c+expr.substr(lastII, ii-lastII);
        	break;
	}
}

expression * parser::high() {
	get();
	expression * t;
	switch(type) {
		case VAR:
			t = new variable(varName);
			get();
			break;
		case NEG:
			t = new negation(high());
			break;
		case LBRACKET:
			t = impl();
			get();
			break;
		default:
			t = nullptr;
			break;
	}
	return t;
}

expression * parser::impl() {
	expression * left = disj();
	while(true) {
		switch(type) {
			case IMPL:
				left = new implication(left, impl());
				break;
			default:
				return left;
				break;
		}
	}
}

expression * parser::conj() {
	expression * left = high();
	while(true) {
		switch(type) {
			case CON:
				left = new conjunction(left, high());
				break;
			default:
				return left;
				break;
		}
	}
}

expression * parser::disj() {
	expression * left = conj();
	while(true) {
		switch(type) {
			case DIS:
				left = new disjunction(left, conj());
				break;
			default:
				return left;
				break;
		}
	}
}
