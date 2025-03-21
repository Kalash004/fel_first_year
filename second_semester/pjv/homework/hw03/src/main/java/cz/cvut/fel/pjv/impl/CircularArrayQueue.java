package cz.cvut.fel.pjv.impl;

import cz.cvut.fel.pjv.Queue;

import java.util.*;

/**
 * Implementation of the {@link Queue} backed by fixed size array.
 */
public class CircularArrayQueue implements Queue {

    // TODO: Implement the homework here
    private final String[] queue;
    private int capacity = 5;
    private int size = 0;
    private int head = 0;
    private int tail = 0;
    /**
     * Creates the queue with capacity set to the value of 5.
     */
    public CircularArrayQueue() {
        queue = new String[capacity];
    }

    /**
     * Creates the queue with given {@code capacity}. The capacity represents maximal number of elements that the
     * queue is able to store.
     *
     * @param capacity of the queue
     */
    public CircularArrayQueue(int capacity) {
        this.capacity = capacity;
        queue = new String[capacity];
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean isEmpty() {
        return (size == 0);
    }

    @Override
    public boolean isFull() {
        return (capacity - size) == 0;
    }

    @Override
    public boolean enqueue(String obj) {
        if (obj == null) {
            return false;
        }
        if (isFull()) {
            return false;
        }
        int temp = head;
        head++;
        size++;
        if (head == capacity) {
            head = 0;
        }
        queue[temp] = obj;
        return true;
    }

    @Override
    public String dequeue() {
        if (isEmpty()) return null;
        int temp = tail;
        size--;
        tail++;
        if (tail == capacity) {
            tail = 0;
        }
        String tmp_obj = queue[temp];
        if (tmp_obj == null) {
            size++;
            tail--;
        }
        queue[temp] = null;
        return tmp_obj;
    }

    @Override
    public Collection<String> getElements() {
        ArrayList<String> list = new ArrayList<>();
        for (int i = head; i < capacity; i++) {
            if (queue[i] == null) {
                continue;
            }
            list.addLast(queue[i]);
        }
        for (int i = 0; i < head; i++) {
            if (queue[i] == null) {
                continue;
            }
            list.addLast(queue[i]);
        }
        return list;
    }

    @Override
    public void printAllElements() {
        for (String s : queue) {
            if (s == null) {
                continue;
            }
            System.out.println(s);
        }
    }
}

