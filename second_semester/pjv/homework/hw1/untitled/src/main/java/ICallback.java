public interface ICallback<Input, Return> {
    Return call(Input input);
}
