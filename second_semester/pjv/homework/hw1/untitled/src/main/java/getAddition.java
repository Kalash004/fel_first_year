public class getAddition implements IFunc<Void, Void>{
    private Controller controller;
    public getAddition(Controller controller) {
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
