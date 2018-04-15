package utils;

import expression.*;

public class MySweetGluttony {
    private Type type;
    private String varName;
    private String expr;
    private int ii;
    private enum Type {VAR, CON, DIS, NEG, IMPL, LBRACKET, RBRACKET}

    public Expression parse(String str) {
        type = null;
        varName = null;
        expr = str;
        ii = 0;
        return implication();
    }

    private char nextChar() {
        if (ii < expr.length()) {
            char c = expr.charAt(ii);
            ii++;
            return c;
        } else {
            return '$';
        }
    }

    private void get() {
        char c = nextChar();
        switch (c) {
            case '-':
                ii++;
                type = Type.IMPL;
                break;
            case '&':
                type = Type.CON;
                break;
            case '|':
                type = Type.DIS;
                break;
            case '!':
                type = Type.NEG;
                break;
            case '(':
                type = Type.LBRACKET;
                break;
            case ')':
                type = Type.RBRACKET;
                break;
            default:
                type = Type.VAR;
                int lastII = ii;
                while (ii < expr.length() && (Character.isLetter(expr.charAt(ii)) || Character.isDigit(expr.charAt(ii)))) {
                    ii++;
                }
                varName = c+expr.substring(lastII, ii);
        }
    }

    private Expression highPriority() {
        get();
        Expression t;
        switch (type) {
            case VAR:
                t = new Variable(varName);
                get();
                break;
            case NEG:
                t = new Negation(highPriority());
                break;
            case LBRACKET:
                t = implication();
                get();
                break;
            default:
                t = null;
        }
        return t;
    }

    private Expression implication() {
        Expression left = disjunction();
        while (true) {
            switch (type) {
                case IMPL:
                    left = new Implication(left, implication());
                    break;
                default:
                    return left;
            }
        }
    }

    private Expression conjunction() {
        Expression left = highPriority();
        while (true) {
            switch (type) {
                case CON:
                    left = new Conjunction(left, highPriority());
                    break;
                default:
                    return left;
            }
        }
    }

    private Expression disjunction() {
        Expression left = conjunction();
        while (true) {
            switch (type) {
                case DIS:
                    left = new Disjunction(left, conjunction());
                    break;
                default:
                    return left;
            }
        }
    }
}
