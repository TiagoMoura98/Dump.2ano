public class ex01d {
    public static int stringToInt(String str) {
        if (str.isEmpty()) {
            return 0;
        }
        int digit = str.charAt(0) - '0';
        return digit * (int) Math.pow(10, str.length() - 1) + stringToInt(str.substring(1));
    }

    public static void main(String[] args) {
        String str = "13531";
        System.out.println(STR."The integer representation of \"\{str}\" is: \{stringToInt(str)}");
    }
}
