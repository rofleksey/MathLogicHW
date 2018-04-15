package expression;

import java.util.Objects;

public class Negation extends Expression {

    private Expression negated;

    public Negation(Expression negated) {
        this.negated = negated;
        key = "!";
        args = new Expression[]{negated};
    }

    public Expression getNegated() {
        return negated;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Negation negation = (Negation) o;
        return Objects.equals(negated, negation.negated);
    }

    @Override
    public int hashCode() {
        return hash != null ? hash : (hash = Objects.hash(negated));
    }

    @Override
    public void toStr(StringBuilder b) {
        b.append("!(");
        negated.toStr(b);
        b.append(")");
    }
}
