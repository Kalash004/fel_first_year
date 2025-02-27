public class Addition implements IFunc<Float, Params> {
    private Calculator calc;

    public Addition() {

    }

    @Override
    public Float call(Params input) {
        return 0f;
    }

    @Override
    public Float call() {
        throw new UnsupportedOperationException("Not supported yet.");
    }
}
