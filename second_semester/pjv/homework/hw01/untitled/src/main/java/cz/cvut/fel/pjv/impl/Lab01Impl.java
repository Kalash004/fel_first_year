package cz.cvut.fel.pjv.impl;
import cz.cvut.fel.pjv.Lab01;

import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.util.Scanner;

public class Lab01Impl implements Lab01 {

    public void homework() {
        Scanner scanner = new Scanner(System.in);
        int choice;
        choice = getChoice(scanner);
        if (choice > 4 || choice < 1) {
            System.out.println("Chybna volba!\n");
            return;
        }

        switch (choice) {
            case 1:
                startAddition(scanner);
                break;
            case 2:
                startSubtraction(scanner);
                break;
            case 3:
                startMultiplication(scanner);
                break;
            case 4:
                startDivision(scanner);
                break;
            default:
                System.out.println("Chybna volba!\n");
                break;
        }
        scanner.close();
        return;
    }

    private static int getChoice(Scanner scanner) {
        int choice;
        System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
        choice = scanner.nextInt();
        return choice;
    }

    private static void startAddition(Scanner scanner) {
        System.out.println("Zadej scitanec: ");
        double x = scanner.nextDouble();
        System.out.println("Zadej scitanec: ");
        double y = scanner.nextDouble();
        System.out.println("Zadej pocet desetinnych mist: ");
        int count = scanner.nextInt();
        if (count < 0) {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            return;
        }
        double result = x + y;
        printAnswer(x, y, count, result, "+");
    }

    private static void startSubtraction(Scanner scanner) {
        System.out.println("Zadej mensenec: ");
        double x = scanner.nextDouble();
        System.out.println("Zadej mensitel: ");
        double y = scanner.nextDouble();
        System.out.println("Zadej pocet desetinnych mist: ");
        int count = scanner.nextInt();
        if (count < 0) {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            return;
        }
        double result = x - y;
        printAnswer(x, y, count, result, "-");
    }

    private static void startMultiplication(Scanner scanner) {
        System.out.println("Zadej cinitel: ");
        double x = scanner.nextDouble();
        System.out.println("Zadej cinitel: ");
        double y = scanner.nextDouble();
        System.out.println("Zadej pocet desetinnych mist: ");
        int count = scanner.nextInt();
        if (count < 0) {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            return;
        }
        double result = x * y;
        printAnswer(x, y, count, result, "*");
    }

    private static void startDivision(Scanner scanner) {
        System.out.println("Zadej delenec: ");
        double x = scanner.nextDouble();
        System.out.println("Zadej delitel: ");
        double y = scanner.nextDouble();
        if (y == 0) {
            scanner.close();
            System.out.println("Pokus o deleni nulou!");
            return;
        }
        System.out.println("Zadej pocet desetinnych mist: ");
        int count = scanner.nextInt();
        if (count < 0) {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            return;
        }
        double result = x / y;
        printAnswer(x, y, count, result, "/");
    }

    private static void printAnswer(double x, double y, int decimal_count, double answer, String operation) {
//        String pattern = "#.";
//        for (int i = 0; i < decimal_count; i++) {
//            pattern = pattern.concat("#");
//        }
//        DecimalFormat df = new DecimalFormat(pattern);
//        df.setRoundingMode(RoundingMode.FLOOR);
//        System.out.println(df.format(x) + " " + operation + " " + df.format(y) + " = " + df.format(answer));
        String decimal_format = "%." + decimal_count + "f";
        String answer_format = "%s %s %s = %s";
        System.out.println(String.format(answer_format,
                String.format(decimal_format, x),
                operation,
                String.format(decimal_format, y),
                String.format(decimal_format, answer)
        ));
    }
}
