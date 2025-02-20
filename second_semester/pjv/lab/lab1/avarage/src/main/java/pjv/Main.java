package pjv;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        double scannedValue = 0;
        double sum = 0;
        int counter = 0;
        while (true) {
            scannedValue = scanner.nextDouble();
            if (scannedValue == -1) {
                break;
            }
            sum += scannedValue;
            counter++;
        }
        System.out.printf("Sum: %f\n Avg: %f", sum, sum/counter);
    }
}