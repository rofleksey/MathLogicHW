package expression;

import java.util.Objects;

public class Variable extends Expression {

    private String name;

    public Variable(String name) {
        this.name = name;
        key = "var";
        args = new Expression[0];
    }

    @Override
    public String toTree() {
        return name;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Variable variable = (Variable) o;
        return Objects.equals(name, variable.name);
    }

    @Override
    public int hashCode() {
        return hash != null ? hash : (hash = Objects.hash(name));
    }

    @Override
    public void toStr(StringBuilder b) {
        b.append(name);
    }

    @Override
    public String toString() {
        return name;
    }
}
