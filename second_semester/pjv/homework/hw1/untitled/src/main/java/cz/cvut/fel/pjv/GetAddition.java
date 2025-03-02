package cz.cvut.fel.pjv;

public class GetAddition implements IFunc<Void, Void>{
    private Controller controller;
    public GetAddition(Controller controller) {
        this.controller = controller;
    }

    @Override
    public Void call(Void input) {
        throw new UnsupportedOperationException("Not supported.");
    }

    @Override
    public Void call() {
        controller.startAddition();
        return null;
    }
}
