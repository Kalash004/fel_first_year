import com.sun.jdi.VoidType;

import java.util.HashMap;
import java.util.Scanner;

public class Controller {
    private Calculator calc;
    private HashMap<Integer, IFunc<Void, Void>> funcs;

    public Controller(Calculator calc) {
        this.calc = calc;
        this.funcs = new HashMap<>();

        this.funcs.put(1,new getAddition(this));
    }


    public void loop() {
        int choice = 0;
        while (choice != -1) {
            choice = getChoice();
            if (choice == -1) {
                System.out.println("Bye. \n");
                return;
            }
            this.funcs.get(choice).call();
        }

    }

    public void startAddition() {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Zadej scitanec:\n");
        float x = scanner.nextFloat();
        System.out.println("Zadej scitanec:\n");
        float y = scanner.nextFloat();
        System.out.println("Zadej pocet desetinnych mist:\n");
        float count = scanner.nextFloat();
    }

    private int getChoice() {
        int choice = 0;
        Scanner scanner = new Scanner(System.in);
        while (!this.funcs.containsKey(choice)) {
                System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
                choice = scanner.nextInt();
        }
        return choice;
    }
}
