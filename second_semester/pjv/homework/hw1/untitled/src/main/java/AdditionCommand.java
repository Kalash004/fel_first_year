import java.util.function.Function;

public class AdditionCommand implements ICommand{
    private ICallback<Float, Void> callback;
    private float x;
    private float y;

    public AdditionCommand(ICallback<Float, Void> callback, float x, float y) {
        this.callback = callback;
        this.x = x;
        this.y = y;
    }

    @Override
    public void execute() {
        Calculator calc = Calculator.getInstance();
        float res = calc.add(this.x, this.y);
        this.callback.call(res);
    }
}
