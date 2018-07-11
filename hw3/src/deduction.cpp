#include "deduction.h"

std::vector<std::string> deduction::apply(std::vector<std::string> & proof) {
	for(std::string& s : axiomsImpl) {
		axioms.push_back(p.parse(s));
	}
	std::vector<std::string> result;
	std::string header = proof[0];
	std::vector<std::string> headerParts;
	split(header, "|-", headerParts);
	std::vector<std::string> hypArr;
	split(headerParts[0], ",", hypArr);
	lastHyp = hypArr[hypArr.size()-1];
	lastHypExpr = p.parse(lastHyp);
	std::string headerRes = "";
	for(std::string& s : hypArr) {
		if(s != "") {
			expression * e = p.parse(s);
			if(!(*e == *lastHypExpr)) {
				hypMap[e] = hypMap.size();
			}
		}
	}
	for(swag i = 0; i < hypArr.size() - 1; i++) {
		headerRes += (i==hypArr.size()-2) ? hypArr[i] : (hypArr[i]+",");
	}
	headerRes += "|-((" + lastHypExpr->toString()+")->("+p.parse(headerParts[1])->toString()+"))";
	result.push_back(headerRes);
	lastHyp = lastHypExpr->toString();
	for(swag i = 1; i < proof.size(); i++) {
		prove(proof[i], i, result);
	}
	return result;
}

void deduction::prove(std::string exprStr, swag num, std::vector<std::string> & result) {
	expression * expr = p.parse(exprStr);
	rebuild.push_back(expr->toString());
	if(expr->_type == expression::ExprType::IMPL) {
		if(mp.find(expr->getRight()) != mp.end()) {
			std::vector<std::vector<expression *>> & temp = mp[expr->getRight()];///!!!!!
			std::vector<expression *> t;
			t.push_back(expr);
			t.push_back(expr->getLeft());
			temp.push_back(t);
			mp[expr->getRight()] = temp;
		} else {
			std::vector<std::vector<expression *>> temp;
			std::vector<expression *> t;
			t.push_back(expr);
			t.push_back(expr->getLeft());
			temp.push_back(t);
			//mp.erase(expr->getRight());
			mp[expr->getRight()] = temp;
		}
	}
	if(hypMap.find(expr) != hypMap.end()) {
		printAx(result, expr->toString());
		used[expr] = num;
		return;
	}
	if(*lastHypExpr == *expr) {
		used[expr] = num;
		printHyp(result, expr->toString());
		return;
	}
	for(swag i = 0; i < 10; i++) {
		std::unordered_map<std::string, expression *> vv;
		if(axioms[i]->compareTree(expr, vv)) {
			used[expr] = num;
			printAx(result, expr->toString());
			return;
		}
	}
	if(mp.find(expr) != mp.end()) {
		std::vector<std::vector<expression *>> & temp = mp[expr];
		for(std::vector<expression *> & v : temp) {
			if(used.find(v[1]) != used.end()) {
				used[expr] = num;
				swag ii = used[v[1]];
				printMp(result, expr->toString(), rebuild[ii-1]);
				return;
			}
		}
	}
	result.push_back("[НЕ ДОКАЗУЕМО]"+expr->toString());
}

void deduction::printAx(std::vector<std::string> & result, std::string input)  {
    result.push_back("(" + input + ")");
    result.push_back("((" + input + ")" + "->(" + "(" + lastHyp + ")" + "->" + "(" + input + ")" + "))");
    result.push_back("((" + lastHyp + ")" + "->" + "(" + input + ")" + ")");
}

void deduction::printHyp(std::vector<std::string> & result, std::string input) {
    result.push_back("((" + lastHyp + ")" + "->(" + "(" + lastHyp + ")" + "->" + "(" + lastHyp + ")" + "))");
    result.push_back("((" + lastHyp + ")" + "->((" + "(" + lastHyp + ")" + "->" + "(" + lastHyp + ")" + ")->" + "(" + lastHyp + ")" + "))");
    result.push_back("(((" + lastHyp + ")" + "->(" + "(" + lastHyp + ")" + "->" + "(" + lastHyp + ")" + "))->((" + "(" + lastHyp + ")" + "->((" + "(" + lastHyp + ")" + "->" + "(" + lastHyp + ")" + ")->" + "(" + lastHyp + ")" + "))" + "->(" + "(" + lastHyp + ")" + "->" + "(" + lastHyp + ")" + ")))");
    result.push_back("(((" + lastHyp + ")" + "->((" + "(" + lastHyp + ")" + "->" + "(" + lastHyp + ")" + ")->" + "(" + lastHyp + ")" + "))->(" + "(" + lastHyp + ")" + "->" + "(" + lastHyp + ")" + "))");
    result.push_back("((" + input + ")" + "->" + "(" + lastHyp + ")" + ")");
}

void deduction::printMp(std::vector<std::string> & result, std::string input, std::string found) {
    result.push_back("((" + lastHyp + ")" + "->" + "(" + found + ")" + ")->((" + "(" + lastHyp + ")" + "->(" + "(" + found + ")" + "->" + "(" + input + ")" + "))->(" + "(" + lastHyp + ")" + "->" + "(" + input + ")" + "))");
    result.push_back("(((" + lastHyp + ")" + "->(" + "(" + found + ")" + "->" + "(" + input + ")" + "))->(" + "(" + lastHyp + ")" + "->" + "(" + input + ")" + "))");
    result.push_back("((" + lastHyp + ")" + "->" + "(" + input + ")" + ")");
}



