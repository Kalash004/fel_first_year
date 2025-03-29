package cz.cvut.fel.pjv.impl;

import cz.cvut.fel.pjv.Node;
import cz.cvut.fel.pjv.Tree;

import java.util.*;

public class TreeImpl implements Tree {
    private NodeImpl root = null;

    public TreeImpl() {
    } // default

    @Override
    public void setTree(int[] values) {
        if (values == null || values.length == 0) {
            return;
        }
        setRoot(values);
        this.setSides(values, this.root, 0);
    }

    private void setRoot(int[] values) {
        this.root = new NodeImpl(values[(values.length / 2)]);
    }

    @Override
    public Node getRoot() {
        return this.root;
    }

    @Override
    public String toString() {
        if (this.root == null) {
            return "";
        }

        HashSet<NodeImpl> explored = new HashSet<>();
        Stack<NodeImpl> stack = new Stack<>();
        StringBuilder result = new StringBuilder();

        stack.push(root);
        while (!stack.isEmpty()) {
            NodeImpl node = stack.pop();
            char[] pad = new char[node.depth];
            Arrays.fill(pad, ' ');
            result.append(pad).append("- ").append(node.getValue()).append("\n");
            if (explored.contains(node)) {
                continue;
            }
            explored.add(node);
            if (node.getRight() != null) {
                stack.push(node.getRightNode());
            }
            if (node.getLeft() != null) {
                stack.push(node.getLeftNode());
            }
        }
        return result.toString();
    }

    private void setSides(int[] values, NodeImpl parent, int depth) {
        int[] leftArr;
        leftArr = Arrays.copyOfRange(values, 0, values.length / 2);
        int[] rightArr;
        rightArr = Arrays.copyOfRange(values, (values.length / 2) + 1, values.length);
        NodeImpl left = null;
        NodeImpl right = null;

        depth += 1;
        if (leftArr.length == 0 && rightArr.length == 0) {
            return;
        }
        if (leftArr.length > 0) {
            left = new NodeImpl(leftArr[(leftArr.length / 2)]);
            left.depth = depth;
            parent.setLeft(left);
            this.setSides(leftArr, left, depth);
        }

        if (rightArr.length > 0) {
            right = new NodeImpl(rightArr[(rightArr.length / 2)]);
            right.depth = depth;
            parent.setRight(right);
            this.setSides(rightArr, right, depth);
        }
    }
}
