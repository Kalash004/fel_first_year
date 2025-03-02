package cz.cvut.fel.pjv;

public interface IFunc<Retrn, Inpt> {
    Retrn call(Inpt input);

    Retrn call();
}
