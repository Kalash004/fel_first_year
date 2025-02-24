import java.util.function.Function;

public class MultiplicationCommand implements ICommand{
    private Function<Float, Void> callback;
    private float x;
    private float y;

    public MultiplicationCommand(Function<Float, Void> callback, float x, float y) {
        this.callback = callback;
        this.x = x;
        this.y = y;
    }

    @Override
    public void execute() {
        Calculator calc = Calculator.getInstance();
        float res = calc.mul(this.x, this.y);
        this.callback.apply(res);
    }
}
