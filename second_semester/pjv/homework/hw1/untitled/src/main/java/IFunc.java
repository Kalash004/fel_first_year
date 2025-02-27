public interface IFunc<Retrn, Inpt> {
    Retrn call(Inpt input);

    Retrn call();
}
