package cz.cvut.k36.omo.hw.hw01;
public class Homework1 {
    private int h_call_count = 0;
    private static int i_call_count = 0;

    public boolean f() {
        return true;
    }

    public static boolean g() {
        return false;
    }

    public int h() {
        h_call_count++;
        return h_call_count;
    }

    public static int i() {
        i_call_count++;
        return i_call_count;
    }
}
