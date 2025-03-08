package cz.cvut.fel.pjv.impl;

import java.util.Scanner;

public class Lab02 {
    private final Scanner scanner = new Scanner(System.in);
    private Stats stats = new Stats();

    public void main(String[] args) {
        int loop_count = 0;
        while (scanner.hasNext()) {
            loop_count++;
            double number;

            try {
                number = Double.parseDouble(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.err.printf("A number has not been parsed from line %d\n", loop_count);
                --loop_count;
                continue;
            }

            stats.addNumber(number);
            if (loop_count % 10 == 0) {
                System.out.printf("%2d %s\n", loop_count, stats.getFormattedStatistics());
                loop_count = 0;
                stats = new Stats();
            }
        }
        System.err.printf("End of input detected!\n", loop_count);
        System.out.printf("%2d %s\n", loop_count, stats.getFormattedStatistics());
        System.out.println(stats.getList());
        scanner.close();
    }
}