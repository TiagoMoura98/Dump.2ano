public class Labyrinth {
    public void testCheck(){
        System.out.println(x:"check");
        int[][] actual = {
                {1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1},
                {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
                {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
                {1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
                {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };
        int y = 0;
        int x = 0;
        int[][] expResult = {
                {9, 9, 9, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1},
                {9, 0, 9, 9, 9, 0, 1, 1, 1, 1, 1, 0, 1},
                {9, 0, 0, 0, 9, 0, 1, 0, 1, 0, 1, 0, 1},
                {9, 0, 0, 0, 9, 9, 9, 0, 1, 0, 1, 1, 1},
                {9, 9, 9, 9, 9, 0, 0, 0, 0, 1, 0, 0, 0},
                {0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9}
        };
        int[][] result = check(actual, x, y);
        }
        public static int [][] check(int[][] actual, int x, int y){
            actual [x]  [y] = 9;
            if {(y == actual.length-1) && (x == actual[0].length-1)){
                return actual;
            }
            int [][] result;

            if (moveIsAllowed(actual, y-1, x)){   //North
                result = check(actual, y-1, x);
                if (result != null){
                    return result;
                }
            }
            if (moveIsAllowed(actual, x+1, y){    //East
                result = check(actual, x+1, y);
                if (result != null){
                    return result;
                }
            }
            if (moveIsAllowed(actual, y+1, x){    //South
                result = check(actual, y+1, x);
                if (result != null){
                    return result;
                }
            }
            if (moveIsAllowed(actual, x-1, y){     //West
                result = check(actual, x-1, y);
                if (result != null){
                    return result;
                }
            }

            actual [x] [y] = 2;
            return null;

            private static boolean moveIsAllowed(int[][] actual, int x, int y){
                return (x >= 0) && (x < actual.length) && (y >= 0) && (y < actual[0].length) && (actual[x][y] == 1);
            };
    }
}
