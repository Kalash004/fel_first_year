package cz.cvut.fel.pjv;

public class GetMultiplication implements IFunc<Void, Void> {
    private Controller controller;

    public GetMultiplication(Controller controller) {
        this.controller = controller;
    }

    @Override
    public Void call(Void input) {
        throw new UnsupportedOperationException("Not supported.");
    }

    @Override
    public Void call() {
        controller.startMultiplication();
        return null;
    }
}
