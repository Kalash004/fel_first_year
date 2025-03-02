package cz.cvut.fel.pjv;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        homework();
    }

    private static void homework() {
        Calculator calc = new Calculator();
        Controller controller = new Controller(calc, new Scanner(System.in));
        controller.loop();
    }
}
