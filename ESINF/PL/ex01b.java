public class ex01b {

    public static int multiply(int m, int n) {
        if (n == 0) {
            return 0;
        }
        return m + multiply(m, n - 1);
    }

    public static void main(String[] args) {
        int m = 5;
        int n = 3;
        System.out.println(STR."Product of \{m} and \{n} is: \{multiply(m, n)}");
    }
}