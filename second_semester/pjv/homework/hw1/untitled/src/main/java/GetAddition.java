public class GetAddition implements IFunc<Void, Void>{
    private Controller controller;
    public GetAddition(Controller controller) {
        this.controller = controller;
    }

    @Override
    public Void call(Void input) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public Void call() {
        controller.startAddition();
        return null;
    }
}
