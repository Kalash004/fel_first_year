package cz.cvut.fel.pjv;

import java.util.HashMap;
import java.util.Scanner;

public class Controller {
    private final Calculator calc;
    private final HashMap<Integer, IFunc<Void, Void>> commands;
    private final Scanner scanner;

    public Controller(Calculator calc, Scanner scanner) {
        this.calc = calc;
        this.commands = new HashMap<>();
        this.scanner = scanner;

        this.commands.put(1, new GetAddition(this));
        this.commands.put(2, new GetSubtraction(this));
        this.commands.put(3, new GetMultiplication(this));
        this.commands.put(4, new GetDivision(this));

    }

    public void loop() {
        int choice;
        choice = getChoice();
        if (choice == -1) {
            System.out.println("Bye. \n");
            return;
        }
        if (!commands.containsKey(choice)) {
            System.out.println("Chybna volba!\n");
            return;
        }
        this.commands.get(choice).call();
    }

    public void startAddition() {
        System.out.println("Zadej scitanec:");
        float x = scanner.nextFloat();
        System.out.println("Zadej scitanec:");
        float y = scanner.nextFloat();
        System.out.println("Zadej pocet desetinnych mist:");
        int count = scanner.nextInt();
        if (count < 0) {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            return;
        }
        float result = this.calc.sum(x, y);
        printAnswer(x, y, count, result, "+");
    }

    public void startSubtraction() {
        System.out.println("Zadej mensenec:");
        float x = scanner.nextFloat();
        System.out.println("Zadej mensitel:");
        float y = scanner.nextFloat();
        System.out.println("Zadej pocet desetinnych mist:");
        int count = scanner.nextInt();
        if (count < 0) {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            return;
        }
        float result = this.calc.sub(x, y);
        printAnswer(x, y, count, result, "-");
    }

    public void startMultiplication() {
        System.out.println("Zadej cinitel:");
        float x = scanner.nextFloat();
        System.out.println("Zadej cinitel:");
        float y = scanner.nextFloat();
        System.out.println("Zadej pocet desetinnych mist:");
        int count = scanner.nextInt();
        if (count < 0) {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            return;
        }
        float result = this.calc.mul(x, y);
        printAnswer(x, y, count, result, "*");
    }

    public void startDivision() {
        System.out.println("Zadej delenec:");
        float x = scanner.nextFloat();
        System.out.println("Zadej delitel:");
        float y = scanner.nextFloat();
        if (y == 0) {
            scanner.close();
            System.out.println("Pokus o deleni nulou!");
            return;
        }
        System.out.println("Zadej pocet desetinnych mist:");
        int count = scanner.nextInt();
        if (count < 0) {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            return;
        }
        float result = this.calc.div(x, y);
        printAnswer(x, y, count, result, "/");
    }

    private int getChoice() {
        int choice;
        System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
        choice = scanner.nextInt();
        return choice;
    }

    private void printAnswer(float x, float y, int decimal_count, float answer, String operation) {
        String decimal_format = "%." + decimal_count + "f";
        String answer_format = String.format("%s %s %s = %s", decimal_format, "%s", decimal_format, decimal_format);
        System.out.println(String.format(answer_format, x, operation, y, answer));
    }

    public void closeScanner() {
        scanner.close();
    }
}
