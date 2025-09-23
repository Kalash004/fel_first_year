package cz.cvut.fel.pjv;

public class GetSubtraction implements IFunc<Void, Void> {
    private Controller controller;

    public GetSubtraction(Controller controller) {
        this.controller = controller;
    }

    @Override
    public Void call(Void input) {
        throw new UnsupportedOperationException("Not supported.");
    }

    @Override
    public Void call() {
        controller.startSubtraction();
        return null;
    }
}

