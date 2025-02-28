public class GetDivision implements IFunc<Void, Void> {
    private Controller controller;

    public GetDivision(Controller controller) {
        this.controller = controller;
    }

    @Override
    public Void call(Void input) {
        throw new UnsupportedOperationException("Not supported.");
    }

    @Override
    public Void call() {
        controller.startDivision();
        return null;
    }
}

