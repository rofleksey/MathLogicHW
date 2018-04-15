import expression.Expression;
import expression.Implication;
import utils.MySweetGluttony;
import utils.Pair;

import java.io.*;
import java.nio.charset.Charset;
import java.nio.file.Files;

import java.nio.file.Paths;
import java.util.ArrayList;
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
    static ArrayList<Expression> expressions = new ArrayList<>(10000);
    static Expression alpha, result, resultRight;

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
                if (!compareTrees(expr.args[i], to.args[i], vars)) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    static Pair<Integer, Integer> getMp(Expression expr) {
        Pair<Expression, Expression> a = mpProved.get(expr);
        if (a == null) {
            return null;
        }
        Integer b = mentioned.get(a.getValue());
        if (b == null) {
            return null;
        }
        return new Pair<>(mentioned.get(a.getKey()), b);
    }

    static void writeExpression(Expression expr, BufferedWriter out) throws IOException {
        out.write(expr.toString());
        out.newLine();
    }

    static void firstCase(Expression expr, BufferedWriter out) throws IOException {
        Implication ae = new Implication(alpha, expr);
        writeExpression(expr, out);
        writeExpression(new Implication(expr, ae), out);
        writeExpression(ae, out);
    }

    static void secondCase(BufferedWriter out) throws IOException {
        Implication aia = new Implication(alpha, alpha);
        writeExpression(new Implication(alpha, aia), out);
        writeExpression(new Implication(new Implication(alpha, aia), new Implication(new Implication(alpha, new Implication(aia, alpha)), aia)  ), out);
        writeExpression(new Implication(new Implication(alpha, new Implication(aia, alpha)), aia), out);
        writeExpression(new Implication(alpha, new Implication(aia, alpha)), out);
        writeExpression(aia, out);
    }

    static void thirdCase(Expression bi, Expression bj, BufferedWriter out) throws IOException {
        writeExpression(new Implication(new Implication(alpha, bj), new Implication(new Implication(alpha, new Implication(bj, bi)), new Implication(alpha, bi))), out);
        writeExpression(new Implication(new Implication(alpha, new Implication(bj, bi)), new Implication(alpha, bi)), out);
        writeExpression(new Implication(alpha, bi), out);
    }

    static void getHint(Expression expr, BufferedWriter out) throws IOException {
        if ("->".equals(expr.key)) {
            mpProved.put(expr.args[1], new Pair<>(expr, expr.args[0]));
        }
        Integer h = hyp.get(expr);
        if (h != null) {
            //out.write("Предп. " + (h + 1));
            firstCase(expr, out);
            return;
        }
        for (int i = 0; i < axioms.length; i++) {
            vars.clear();
            if (compareTrees(expr, axioms[i], vars)) {
                //out.write("Сх. акс. " + (i + 1));
                firstCase(expr, out);
                return;
            }
        }
        if (alpha.equals(expr)) {
            secondCase(out);
            return;
        }
        Pair<Integer, Integer> mpResult = getMp(expr);
        //out.write(mpResult == null ? "Не доказано" : "M.P. " + (mpResult.getKey() + 1) + ", " + (mpResult.getValue() + 1));
        thirdCase(expr, expressions.get(mpResult.getValue()), out);
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
            ArrayList<Expression> hypArr = new ArrayList<>(1000);
            if (!header[0].isEmpty()) {
                String[] dataStrings = header[0].split(",");
                for (int i = 0; i < dataStrings.length-1; i++) {
                    hyp.put(alpha = parser.parse(dataStrings[i]), i);
                    hypArr.add(alpha);
                }
                alpha = parser.parse(dataStrings[dataStrings.length-1]);
            }
            resultRight = parser.parse(header[1]);
            result = new Implication(alpha, resultRight);
            int counter = 1;
            String line;
            try (BufferedWriter out = Files.newBufferedWriter(Paths.get("output.txt"), Charset.forName("UTF-8"))) {
                for (int i = 0; i < hypArr.size(); i++) {
                    out.write(hypArr.get(i).toString() + (i == hypArr.size() - 1 ? "" : ","));
                }
                out.write("|-");
                out.write(result.toString());
                out.newLine();
                //out.write(alpha+" "+result+"!!!");
                //out.newLine();
                while ((line = r.readLine()) != null) {
                    line = sunshine(line);
                    if (line.isEmpty()) {
                        continue;
                    }
                    Expression cur = parser.parse(line);
                    getHint(cur, out);
                    mentioned.put(cur, counter - 1);
                    expressions.add(cur);
                    counter++;
                }
            }
        }
    }
}