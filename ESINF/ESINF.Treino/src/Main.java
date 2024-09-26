import java.util.*;

import static java.lang.Math.pow;

public class Main {
    public static void main(String[] args) {

        System.out.println(inverte("abcdefg"));

        System.out.println(inverte_tail("qwerty", ""));

        System.out.println(prod(10,4,0));

        System.out.println(converter("12345", 0));

    }

    public static String inverte(String str) {

        if (str.isEmpty()) {
            return "";
        }
        return "" + inverte(str.substring(1)) + str.charAt(0);
    }

    public static String inverte_tail(String str, String res) {

        if (str.isEmpty()) {
            return res;
        }

        res = str.charAt(0) + res;
        return inverte_tail(str.substring(1), res);
    }

    public static int prod (int m, int n, int res){

        if (n == 0){
            return res;
        }

        res += m;
        n --;
        return (prod(m,n,res));
    }

    public static int sum_a (int [] a){
        return sum_a_helper (a, a.length - 1);
    }

    public static int sum_a_helper (int [] a, int index){
        if(index < 0){
            return 0;
        }

        return a[index] + sum_a_helper(a,index - 1);
    }

    public static int converter (String str, int num){
        if (str.isEmpty()) {
            return num;
        }
        num = num + (int) ((str.charAt(0) - '0')* Math.pow(10, str.length() - 1));

        return converter (str.substring(1), num);
    }

    public Map <String, List<Integer>> footWorldCup (List<Map.Entry<Integer,String>> lf){

        if(lf == null){ return null;}

        Map <String,List<Integer>> mapWc = new HashMap<>();

        for (Map.Entry<Integer,String> pair : lf ){

            List<Integer> lst = mapWc.get(pair.getValue());

            if (lst == null){
                lst = new LinkedList<>();
            }

            lst.add(pair.getKey());
            mapWc.put(pair.getValue(),lst);
        }

        for(List<Integer> lst : mapWc.values()){
            Collections.sort(lst,Collections.reverseOrder());
        }
       return mapWc;
    }

    public LinkedList<Integer> CatCMMS (LinkedList<Integer> serie, Integer period){

        if (serie == null){ return null;}

        if(period == 0 || period > serie.size()){return null;}

        LinkedList<Integer> mms = new LinkedList<>();
        LinkedList<Integer> buffer = new LinkedList<>();
        Integer sum = 0;

        for(Integer num : serie){
            buffer.add();
        }

        return mms;
    }


}