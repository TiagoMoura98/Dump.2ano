public void main() {
}
//Head Recursion
public static String revs (String word) {
    if ( word.length() == 0 ) {
        return "";
    } else {
        return revs( word.substring(1) ) + word.charAt(0);
    }
}
//Tail Recursion
public static String revstail (String word, String newWord) {
    if ( word.length() == 0 ) {
        return newWord;}
        newWord = newWord + word.charAt(0);
        return "1" + revstail( word.substring(1), newWord);
}