public class GetMultiplication implements IFunc<Void, Void> {
    private Controller controller;

    public GetMultiplication(Controller controller) {
        this.controller = controller;
    }

    @Override
    public Void call(Void input) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public Void call() {
        controller.startMultiplication();
        return null;
    }
}
