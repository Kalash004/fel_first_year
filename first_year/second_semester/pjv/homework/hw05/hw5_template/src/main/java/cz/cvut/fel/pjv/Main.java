package cz.cvut.fel.pjv;

import cz.cvut.fel.pjv.impl.TreeImpl;

public class Main {
    public static void main(String[] args) {
        TreeImpl tree = new TreeImpl();
        int[] values = {1,2,3,4,5,6,7,8,9,10};
        tree.setTree(values);
        System.out.println(tree.toString());
    }
}
