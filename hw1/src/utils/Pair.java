package utils;

public class Pair<A, B> {
    private A a;
    private B b;
    public Pair(A a, B b) {
        this.a = a;
        this.b = b;
    }

    public A getKey() {
        return a;
    }

    public B getValue() {
        return b;
    }

    @Override
    public int hashCode() {
        return a.hashCode() * 13 + (b == null ? 0 : b.hashCode());
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o instanceof Pair) {
            Pair pair = (Pair) o;
            if (a != null ? !a.equals(pair.a) : pair.a != null) return false;
            if (b != null ? !b.equals(pair.b) : pair.b != null) return false;
            return true;
        }
        return false;
    }
}
