package cz.cvut.fel.pjv;

import cz.cvut.fel.pjv.impl.CircularArrayQueue;

public class Start {

    public static void main(String[] args) {
        Queue queue = new CircularArrayQueue(6);

        queue.enqueue("Starkiller 1");
        queue.enqueue("Starkiller 2");
        queue.enqueue("Starkiller 3");
        queue.enqueue("Starkiller 4");
        queue.enqueue("Starkiller 5");
        queue.enqueue("Starkiller 6");
        queue.enqueue("Starkiller 7");

        System.out.println("printing all elements: ");
        System.out.println("size: " + queue.size());
        System.out.println(queue.getElements());
        queue.dequeue();
        queue.dequeue();
        System.out.println(queue.getElements());
        queue.enqueue("Starkiller 7");
        System.out.println(queue.getElements());

    }
}
