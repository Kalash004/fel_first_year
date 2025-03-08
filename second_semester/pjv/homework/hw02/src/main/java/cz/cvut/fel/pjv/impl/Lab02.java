package cz.cvut.fel.pjv.impl;

import java.util.Scanner;

public class Lab02 {
    private final Scanner scanner = new Scanner(System.in);
    private Stats stats = new Stats();

    public void main(String[] args) {
        int input_count = 0;
        int line_count = 0;
        while (scanner.hasNext()) {
            input_count++;
            line_count++;
            double number;

            try {
                number = Double.parseDouble(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.err.printf("A number has not been parsed from line %d\n", line_count);
                input_count--;
                continue;
            }

            stats.addNumber(number);
            if (input_count % 10 == 0) {
                System.out.printf("%s\n", stats.getFormattedStatistics());
            }
        }
        System.err.printf("End of input detected!\n");
        System.out.printf("%s\n", stats.getFormattedStatistics());
        scanner.close();
    }
}