package expression;

public abstract class Expression extends Object {
    public Expression[] args;
    public String key;
    Integer hash;
    String strImpl;//TODO: replace with stringbuilder
    public abstract void toStr(StringBuilder b);
    @Override
    public String toString() {
        if(strImpl == null) {
            StringBuilder b = new StringBuilder();
            toStr(b);
            strImpl = b.toString();
        }
        return strImpl;
    }
    @Override
    public boolean equals(Object obj) {
        return obj != null && hashCode() == obj.hashCode() && toString().equals(toString());
    }
}
