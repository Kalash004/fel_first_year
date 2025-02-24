public class Calculator {
    private static Calculator instance = null;
    public static Calculator getInstance() {
        if (instance == null) {
            instance = new Calculator();
        }
        return instance;
    }

    public float add(float x, float y) {
        return x + y;
    }

    public float sub(float x, float y) {
        return x - y;
    }

    public float mul(float x, float y) {
        return x * y;
    }

    public float div(float x, float y) {
        return x / y;
    }
}
