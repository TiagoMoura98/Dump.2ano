public class ex01c {

    public static int gcd(int m, int n) {
        if (n == 0) {
            return m;
        }
        return gcd(n, m % n);
    }

    public static void main(String[] args) {
        int m = 48;
        int n = 30;
        System.out.println(STR."GCD of \{m} and \{n} is: \{gcd(m, n)}");
    }
}