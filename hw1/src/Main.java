import expression.Expression;
import utils.MySweetGluttony;
import utils.Pair;

import java.io.*;
import java.nio.charset.Charset;
import java.nio.file.Files;

import java.nio.file.Paths;
import java.util.HashMap;

public class Main {
    static final String[] axiomsStr = new String[]{
            "A->B->A",                     // 1
            "(A->B)->(A->B->C)->(A->C)",   // 2
            "A->B->A&B",                   // 3
            "A&B->A",                      // 4
            "A&B->B",                      // 5
            "A->A|B",                      // 6
            "B->A|B",                      // 7
            "(A->C)->(B->C)->(A|B->C)",    // 8
            "(A->B)->(A->!B)->!A",         // 9
            "!!A->A"                       // 10
    };
    static Expression[] axioms;
    static HashMap<Expression, Integer> hyp = new HashMap<>(52001, 1);
    static HashMap<Expression, Integer> mentioned = new HashMap<>(52001, 1);
    static HashMap<Expression, Pair<Expression, Expression>> mpProved = new HashMap<>(52001, 1);
    static HashMap<Expression, Expression> vars = new HashMap<>(52001, 1);

    static boolean compareTrees(Expression expr, Expression to, HashMap<Expression, Expression> vars) {
        if (to.args.length == 0) {
            if (vars.containsKey(to)) {
                return vars.get(to).equals(expr);
            } else {
                vars.put(to, expr);
                return true;
            }
        } else if (to.key.equals(expr.key)) {//TODO: CHANGED <=
            for (int i = 0; i < to.args.length; i++) {
                if(!compareTrees(expr.args[i], to.args[i], vars)) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    static Pair<Integer, Integer> getMp(Expression expr) {
        Pair<Expression, Expression> a = mpProved.get(expr);
        if(a == null) {
            return null;
        }
        Integer b = mentioned.get(a.getValue());
        if(b == null) {
            return null;
        }
        return new Pair<>(mentioned.get(a.getKey()), b);
    }

    static void getHint(Expression expr, BufferedWriter out, int index) throws IOException {
        if("->".equals(expr.key)) {
            mpProved.put(expr.args[1], new Pair<>(expr, expr.args[0]));
        }
        Integer h = hyp.get(expr);
        if(h != null) {
            out.write("Предп. " + (h + 1));
            return;
        }
        for (int i = 0; i < axioms.length; i++) {
            vars.clear();
            if (compareTrees(expr, axioms[i], vars)) {
                out.write("Сх. акс. " + (i + 1));
                return;
            }
        }
        Pair<Integer, Integer> mpResult = getMp(expr);
        out.write(mpResult == null ? "Не доказано" : "M.P. " + (mpResult.getKey()+1) + ", " + (mpResult.getValue()+1));
    }

    static String sunshine(String it) {
        StringBuilder b = new StringBuilder(it.length());
        for (int i = 0; i < it.length(); i++) {
            if (!Character.isWhitespace(it.charAt(i))) {
                b.append(it.charAt(i));
            }
        }
        return b.toString();
    }

    public static void main(String[] args) throws Exception {
        MySweetGluttony parser = new MySweetGluttony();
        axioms = new Expression[axiomsStr.length];
        for (int i = 0; i < axiomsStr.length; i++) {
            axioms[i] = parser.parse(axiomsStr[i]);
        }
        try (BufferedReader r = Files.newBufferedReader(Paths.get("input.txt"), Charset.forName("UTF-8"))) {
            String[] header = sunshine(r.readLine()).split("\\|-");
            if (!header[0].isEmpty()) {
                String[] dataStrings = header[0].split(",");
                for (int i = 0; i < dataStrings.length; i++) {
                    hyp.put(parser.parse(dataStrings[i]), i);
                }
            }
            int counter = 1;
            String line;
            try (BufferedWriter out = Files.newBufferedWriter(Paths.get("output.txt"), Charset.forName("UTF-8"))) {
                while ((line = r.readLine()) != null) {
                    line = sunshine(line);
                    if(line.isEmpty()) {
                        continue;
                    }
                    Expression cur = parser.parse(line);
                    out.write("(" + counter + ") " + line + " (");
                    getHint(cur, out, counter - 1);
                    out.write(")");
                    out.newLine();
                    mentioned.put(cur, counter - 1);
                    counter++;
                }
            }
        }
        //System.out.println("TIME: "+(System.currentTimeMillis()-time));
    }
}
