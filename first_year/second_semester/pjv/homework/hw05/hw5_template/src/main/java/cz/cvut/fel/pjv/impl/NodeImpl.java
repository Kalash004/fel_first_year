package cz.cvut.fel.pjv.impl;

import cz.cvut.fel.pjv.Node;

public class NodeImpl implements Node {

    // TODO: implement this class
    private final int value;
    private NodeImpl left = null;
    private NodeImpl right = null;
    public int depth;

    public NodeImpl(int value) {
        this.value = value;
    }

    @Override
    public Node getLeft() {
        return left;
    }

    @Override
    public Node getRight() {
        return right;
    }

    @Override
    public int getValue() {
        return value;
    }

    public NodeImpl getLeftNode() {
        return left;
    }

    public NodeImpl getRightNode() {
        return right;
    }

    public void setLeft(NodeImpl left) {
        this.left = left;
    }

    public void setRight(NodeImpl right) {
        this.right = right;
    }

    @Override
    public String toString() {
        return value + "";
    }
}
