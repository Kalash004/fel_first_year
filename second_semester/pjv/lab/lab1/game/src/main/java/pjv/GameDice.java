package pjv;

import java.util.Random;
import java.util.random.RandomGenerator;

public class GameDice {
    private int sides = 0;

    public GameDice() {
        this.sides = 6;
    }

    public GameDice(int sides) {
        this.sides = sides;
    }

    public int roll() {
        Random rand = new Random();
        return rand.nextInt(this.sides) + 1;
    }
}
