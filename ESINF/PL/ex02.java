public class Labyrinth {
    private static final int PATH = 1;
    private static final int WALL = 0;
    private static final int VISITED = 2;
    private static final int SOLUTION = 9;

    public static void main(String[] args) {
        int[][] labyrinth = {
                {1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1},
                {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
                {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
                {1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
                {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };

        if (findPath(labyrinth, 0, 0, 6, 12)) {
            printLabyrinth(labyrinth);
        } else {
            System.out.println("No path found.");
        }
    }

    public static boolean findPath(int[][] labyrinth, int x, int y, int destX, int destY) {
        // Check if we are out of bounds or at a wall or already visited
        if (x < 0 || x >= labyrinth.length || y < 0 || y >= labyrinth[0].length || labyrinth[x][y] != PATH) {
            return false;
        }

        // Check if we have reached the destination
        if (x == destX && y == destY) {
            labyrinth[x][y] = SOLUTION;
            return true;
        }

        // Mark the current cell as part of the solution path
        labyrinth[x][y] = SOLUTION;

        // Move in the order: north, east, south, west
        if (findPath(labyrinth, x - 1, y, destX, destY) || // North
                findPath(labyrinth, x, y + 1, destX, destY) || // East
                findPath(labyrinth, x + 1, y, destX, destY) || // South
                findPath(labyrinth, x, y - 1, destX, destY)) { // West
            return true;
        }

        // Mark the cell as visited but not part of the solution path
        labyrinth[x][y] = VISITED;
        return false;
    }

    public static void printLabyrinth(int[][] labyrinth) {
        for (int[] row : labyrinth) {
            for (int cell : row) {
                System.out.print(cell + " ");
            }
            System.out.println();
        }
    }
}