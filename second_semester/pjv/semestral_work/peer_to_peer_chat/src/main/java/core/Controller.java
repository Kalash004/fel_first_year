package core;

public class Controller {
    private static Controller instance = new Controller();

    private Controller() {
    }

    public static Controller getInstance() {
        return instance;
    }

    public void launch() {
        // TODO:
    }
}