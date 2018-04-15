package expression;

import java.util.Objects;

public class Disjunction extends Expression {

    private Expression left; //Disjunction
    private Expression right; //Conjunction

    public Disjunction(Expression left, Expression right) {
        this.left = left;
        this.right = right;
        key = "|";
        args = new Expression[]{left, right};
    }

    public Expression getLeft() {
        return left;
    }

    public Expression getRight() {
        return right;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Disjunction that = (Disjunction) o;
        return Objects.equals(left, that.left) &&
                Objects.equals(right, that.right);
    }

    @Override
    public int hashCode() {
        return hash != null ? hash : (hash = Objects.hash(left, right));
    }

    @Override
    public void toStr(StringBuilder b) {
        b.append("(");
        getLeft().toStr(b);
        b.append("|");
        getRight().toStr(b);
        b.append(")");
    }
}
