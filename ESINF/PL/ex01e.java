public class ex01e {
    public static class PalindromeChecker {

        public static boolean isPalindrome(int number) {
            String str = Integer.toString(number);
            return isPalindromeHelper(str, 0, str.length() - 1);
        }

        private static boolean isPalindromeHelper(String str, int left, int right) {
            if (left >= right) {
                return true;
            }
            if (str.charAt(left) != str.charAt(right)) {
                return false;
            }
            return isPalindromeHelper(str, left + 1, right - 1);
        }

        public static void main(String[] args) {
            int number = 1221;
            System.out.println(STR."Is \{number} a palindrome? \{isPalindrome(number)}");
        }
    }

}
