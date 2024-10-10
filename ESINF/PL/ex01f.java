/*Helper On Recursive methods

public class ex01f {
    public static int sum_a (int []a){
        return sum_aHelper(a, a.length-1);
    }
    private static int sum_aHelper(int []a, int index){
        if (index < 0){
            return 0;
        }
        return a[index] + sum_aHelper(a, index-1);
    }
}*/

public class ex01f {

    public static int sum2DArray(int[][] array) {
        return sum2DArrayHelper(array, array.length - 1);
    }

    private static int sum2DArrayHelper(int[][] array, int row) {
        if (row < 0) {
            return 0;
        }
        return sumRow(array[row], array[row].length - 1) + sum2DArrayHelper(array, row - 1);
    }

    private static int sumRow(int[] row, int index) {
        if (index < 0) {
            return 0;
        }
        return row[index] + sumRow(row, index - 1);
    }

    public static void main(String[] args) {
        int[][] array = {
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}
        };
        System.out.println(STR."Sum of all elements: \{sum2DArray(array)}");
    }
}