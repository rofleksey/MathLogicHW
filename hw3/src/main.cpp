#include "util.h"
#include "expression.h"
#include "parser.h"
#include "deduction.h"

using namespace std;



string fHyp, header;
unordered_map<string, bool> variables;
vector<vector<string>> pList;
unordered_map<string, vector<string>> detectMap;
vector<string> varList, hypList;

void loadSimple() {
	ifstream file ("proves/simple");
	string line;
	getline(file, line);
	while(!file.eof()) {
		string h = line;
		vector<string> prooves;
		getline(file, line);
		while(line != "---") {
			prooves.push_back(line);
			getline(file, line);
		}
		detectMap[h] = prooves;
		getline(file, line);
	}
}

void genProofImpl(expression * expr, vector<string> & sp) {
	if(expr->_type == expression::ExprType::NEG) {
		genProofImpl(expr->getRight(), sp);
		string key = (expr->getRight()->calcValue(variables) ? "1" : "0") + expr->getType();
		vector<string> & t = detectMap[key];
		for(swag i = 0; i < t.size(); i++) {
			string res = "";
			for(swag j = 0; j < t[i].length(); j++) {
				if(t[i][j] == 'A') {
					res += "(" + expr->getRight()->toString() + ")";
				} else {
					res += t[i][j];
				}
			}
			sp.push_back(res);
		}
	} else if(expr->_type == expression::ExprType::VAR) {
		if(variables[expr->toString()]) {
			sp.push_back(expr->toString());
		} else {
			sp.push_back("!"+expr->toString());
		}
	} else {
		genProofImpl(expr->getLeft(), sp);
		genProofImpl(expr->getRight(), sp);
		string key = (expr->getLeft()->calcValue(variables) ? "1" : "0" ) + expr->getType() + (expr->getRight()->calcValue(variables) ? "1" : "0");
		vector<string> & t = detectMap[key];
		for(swag i = 0; i < t.size(); i++) {
			string res = "";
			for(swag j = 0; j < t[i].length(); j++) {
				if(t[i][j] == 'A') {
					res += expr->getLeft()->toString();
				} else if(t[i][j] == 'B') {
					res += expr->getRight()->toString();
				} else {
					res += t[i][j];
				}
			}
			sp.push_back(res);
		}
	}
}

void genProof(expression * expr) {
	cout << header << endl;
	loadSimple();
	swag lim = 1 << varList.size();
	for(swag i = 0; i < lim; i++) {
		vector<string> sp;
		for(swag j = 0; j < varList.size(); j++) {
			variables[varList[j]] = (i & (1 << j)) != 0;
		}
		string h = "";
		for(swag j = 0; j < varList.size(); j++) {
			h += (variables[varList[j]] ? "" : "!") + varList[j];
			if(j != varList.size()-1) {
				h += ",";
			}
		}
		h += "|-" + expr->toString();
		sp.push_back(h);
		genProofImpl(expr, sp);
		pList.push_back(sp);
	}
	for(swag i = 0; i < pList.size(); i++) {
		for(swag j = 0; j < varList.size(); j++) {
			deduction d;
			pList[i] = d.apply(pList[i]);
		}
		for(swag j = 1; j < pList[i].size(); j++) {
			cout << pList[i][j] << endl;
		}
	}
	for(swag i = 0; i < varList.size(); i++) {
		string sv = varList[i];
		string mama_ya_sdal_matlog = "special";
		vector<string> & tt = detectMap[mama_ya_sdal_matlog];
		swag lim = 1 << (varList.size() - i - 1);
		for(swag j = 0; j < lim; j++) {
			for(swag k = 0; k < tt.size(); k++) {
				string ss = "";
				for(swag lul = 0; lul < tt[k].length(); lul++) {
					if(tt[k][lul] == 'A') {
						ss += "(" + sv + ")";
					} else {
						ss += tt[k][lul];
					}
				}
				cout << ss << endl;
			}
			string ff = "(";
			for(swag k = i + 1; k < varList.size(); k++) {
				bool b = (j & (1 << (k - i - 1))) != 0;
				ff += (b ? "" : "!") + varList[k] + "->";
			}
			ff += "(" + fHyp + "))";
			cout << "(" << sv << "->" << ff << ")->(!" << sv << "->" << ff << ")->((" << sv << "|!" << sv << ")->" << ff << ")" << endl;
			cout << "(!" << sv << "->" << ff << ")->((" << sv << "|!" << sv << ")->" << ff << ")" << endl;
			cout << "((" << sv << "|!" << sv << ")->" << ff << ")"  << endl;
			cout << ff << endl;
		}
	}
	for(string & s : hypList) {
		cout << s << endl;
	}
	parser p;
	expression * e = p.parse(fHyp);
	for(swag i = 0; i < hypList.size(); i++) {
		e = e->getRight();
		cout << e->toString() << endl;
	}
}

void calc(expression * expr) {
	size_t lim = 1 << varList.size();
	for(swag i = 0; i < lim; i++) {
		for(swag j = 0; j < varList.size(); j++) {
			variables[varList[j]] = (i & (1 << j)) != 0;
		}
		if(!expr->calcValue(variables)) {
			cout <<  "Высказывание ложно при ";
			for(swag j = 0; j < varList.size(); j++) {
				cout << varList[j] << "=" << ((((i >> j) & 1) == 1) ? "И" : "Л");
				if(j != varList.size() - 1) {
					cout << ", ";
				}
			}
			cout << endl;
			exit(0);
		}
	}
	genProof(expr);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    getline(cin, header);
    vector<string> headerParts;
    split(header, "|=", headerParts);
    if(headerParts[0].length()>0) {
    	split(headerParts[0], ",", hypList);
    }
    size_t signPos = header.find_first_of("|=");
    header.replace(signPos, 2, "|-");
    fHyp = "";
    if(!hypList.empty()) {
    	for(swag i = hypList.size()-1; i >= 0; i --) {
    		fHyp += "(" + hypList[i] + ")->";
    	}
    }
    fHyp += "(" + headerParts[1] + ")";
    parser p;
    expression * expr = p.parse(fHyp);
    for(swag i = 0; i < fHyp.length(); i++) {
    	if(fHyp[i] >= 'A' && fHyp[i] <= 'Z') {
    		string v = string(1, fHyp[i]);
    		if(variables.find(v) == variables.end()) {
    			variables[v] = true;
    			varList.push_back(v);
    		}
    	}
    }
    //cout << expr->toString() << endl;
    calc(expr);
    return 0;
}
