#ifndef CPP_SOLUTION_EXPRESSION_H
#define CPP_SOLUTION_EXPRESSION_H

#include "util.h"

class expression {
public:
	enum ExprType {VAR, NEG, IMPL, CONJ, DISJ};
	size_t hashField = 0;
	ExprType _type;
	std::string str = "";

    virtual std::string toString() = 0;
    
    virtual std::string getType() = 0;
    
    
    virtual expression * getLeft() = 0;
    virtual expression * getRight() = 0;
    
    virtual bool doCompare(expression * other) = 0;
    
    virtual size_t calcHash() = 0;
    
    virtual bool calcValue(std::unordered_map<std::string, bool> & v) = 0;
    
    virtual bool compareTree(expression * expr, std::unordered_map<std::string, expression *> & vv) = 0;
    
    inline size_t myHashCode(std::string s, expression * a = nullptr, expression * b = nullptr) {
		swag result = 1;
		std::hash<std::string> hasher;
		result = 31*result + hasher(s);
		if(a != nullptr) result = 31*result + a->calcHash();
		if(b != nullptr) result = 31*result + b->calcHash();
		return result;
	}
    
    inline bool operator == (expression& other) {
        return doCompare(&other);
    }

    virtual ~expression() {};
};

struct expression_equal {
    bool operator()(expression * a, expression * b) const {
        return a->doCompare(b);
    }
};

class variable : public expression {
private:
    std::string _name;

public:
    variable(std::string &name) : _name(name) {
    	_type = VAR;
    }
    
    virtual size_t calcHash() {
    	if(hashField == 0) {
    		return hashField = myHashCode(_name);
    	}
    	return hashField;
    }
    
    virtual bool doCompare (expression * other) {
    	if(other->_type != _type) {
    		return false;
    	}
        variable * p = dynamic_cast<variable *>(other);
        if(p != nullptr) {
        	return _name == p->_name;
        }
        return false;
    }
    
    virtual bool calcValue(std::unordered_map<std::string, bool> & v) {
    	return v[_name];
    }
    
    virtual bool compareTree(expression * expr, std::unordered_map<std::string, expression *> & vv) {
    	if(vv.find(_name) == vv.end()) {
    		vv[_name] = expr;
    		return true;
    	} else if(*vv[_name] == *expr) {
    		return true;
    	}
    	return false;
    }
    
    virtual expression * getLeft() {
    	return nullptr;
    }
    virtual expression * getRight() {
    	return nullptr;
    }
    
    virtual std::string getType() {
    	return "%var";	
    }

    virtual std::string toString() {
        return _name;
    }
};

class implication : public expression {
private:
    expression* _left;
    expression* _right;

public:
    implication(expression* left, expression* right) : _left(left), _right(right) {
            _type = IMPL;
    }
    
    virtual size_t calcHash() {
    	if(hashField == 0) {
    		return hashField = myHashCode(getType(), _left, _right);
    	}
    	return hashField;
    }
    
    virtual bool doCompare (expression * other) {
    	if(other->_type != _type) {
    		return false;
    	}
        implication * p = dynamic_cast<implication *>(other);
        if(p != nullptr) {
        	return _left->doCompare(p->_left) && _right->doCompare(p->_right);
        }
        return false;
    }
    
    virtual bool calcValue(std::unordered_map<std::string, bool> & v) {
    	return !(_left->calcValue(v)) || _right->calcValue(v);
    }
    
    virtual bool compareTree(expression * expr, std::unordered_map<std::string, expression *> & vv) {
    	
    	if(expr->_type != _type) {
    		return false;
    	}
    	return _left->compareTree(expr->getLeft(), vv) && _right->compareTree(expr->getRight(), vv);
    }
    
    virtual expression * getLeft() {
    	return _left;
    }
    virtual expression * getRight() {
    	return _right;
    }
    
    virtual std::string getType() {
    	return "->";	
    }

    virtual std::string toString() {
        return str=="" ? (str = "(" + _left->toString() + "->" + _right->toString() + ")") : str;
    }
};

class disjunction : public expression {
private:
    expression* _left;
    expression* _right;

public:
    disjunction(expression* left, expression* right) : _left(left), _right(right) {
    	_type = DISJ;
    }
    
    virtual size_t calcHash() {
    	if(hashField == 0) {
    		return hashField = myHashCode(getType(), _left, _right);
    	}
    	return hashField;
    }
    
    virtual bool doCompare (expression * other) {
    	if(other->_type != _type) {
    		return false;
    	}
        disjunction * p = dynamic_cast<disjunction *>(other);
        if(p != nullptr) {
        	return _left->doCompare(p->_left) && _right->doCompare(p->_right);
        }
        return false;
    }
    
    virtual bool calcValue(std::unordered_map<std::string, bool> & v) {
    	return (_left->calcValue(v)) || (_right->calcValue(v));
    }
    
    virtual bool compareTree(expression * expr, std::unordered_map<std::string, expression *> & vv) {
    	
    	if(expr->_type != _type) {
    		return false;
    	}
    	return _left->compareTree(expr->getLeft(), vv) && _right->compareTree(expr->getRight(), vv);
    }
    
    virtual expression * getLeft() {
    	return _left;
    }
    virtual expression * getRight() {
    	return _right;
    }
    
    virtual std::string getType() {
    	return "|";	
    }

    virtual std::string toString() {
        return str=="" ? (str = "(" + _left->toString() + "|" + _right->toString() + ")") : str;
    }
};

class conjunction : public expression {
private:
    expression* _left;
    expression* _right;

public:
    conjunction(expression* left, expression* right) : _left(left), _right(right) {
    	_type = CONJ;
    }
    
    virtual size_t calcHash() {
    	if(hashField == 0) {
    		return hashField = myHashCode(getType(), _left, _right);
    	}
    	return hashField;
    }
    
    virtual bool doCompare (expression * other) {
    	if(other->_type != _type) {
    		return false;
    	}
        conjunction * p = dynamic_cast<conjunction *>(other);
        if(p != nullptr) {
        	return _left->doCompare(p->_left) && _right->doCompare(p->_right);
        }
        return false;
    }
    
    virtual bool calcValue(std::unordered_map<std::string, bool> & v) {
    	return (_left->calcValue(v)) && (_right->calcValue(v));
    }
    
    virtual bool compareTree(expression * expr, std::unordered_map<std::string, expression *> & vv) {
    	
    	if(expr->_type != _type) {
    		return false;
    	}
    	return _left->compareTree(expr->getLeft(), vv) && _right->compareTree(expr->getRight(), vv);
    }
    
    virtual expression * getLeft() {
    	return _left;
    }
    virtual expression * getRight() {
    	return _right;
    }
    
    virtual std::string getType() {
    	return "&";	
    }

    virtual std::string toString() {
        return str=="" ? (str = "(" + _left->toString() + "&" + _right->toString() + ")") : str;
    }
};

class negation : public expression {
private:
    expression* _expr;

public:
    negation(expression* expr) : _expr(expr) {
    	_type = NEG;
    }
    
    virtual size_t calcHash() {
    	if(hashField == 0) {
    		return hashField = myHashCode(getType(), _expr);
    	}
    	return hashField;
    }
    
    virtual bool doCompare (expression * other) {
    	if(other->_type != _type) {
    		return false;
    	}
        negation * p = dynamic_cast<negation *>(other);
        if(p != nullptr) {
        	return _expr->doCompare(p->_expr);
        }
        return false;
    }
    
    virtual bool calcValue(std::unordered_map<std::string, bool> & v) {
    	return !(_expr->calcValue(v));
    }
    
    virtual bool compareTree(expression * expr, std::unordered_map<std::string, expression *> & vv) {
    	
    	if(expr->_type != _type) {
    		return false;
    	}
    	return _expr->compareTree(expr->getLeft(), vv);
    }
    
    virtual expression * getLeft() {
    	return _expr;
    }
    virtual expression * getRight() {
    	return _expr;
    }
    
    virtual std::string getType() {
    	return "!";	
    }

    virtual std::string toString() {
        return str=="" ? (str = "!" + _expr->toString()) : str;
    }
};

namespace std
{
    template <> struct hash<expression>
    {
        size_t operator()(expression & o){
            return o.calcHash();
        }
    };
    template <> struct hash<expression*> {
        size_t operator()(expression * o) const { 
            return hash<expression>()(*o); 
        }
    };
}

#endif //CPP_SOLUTION_EXPRESSION_H
