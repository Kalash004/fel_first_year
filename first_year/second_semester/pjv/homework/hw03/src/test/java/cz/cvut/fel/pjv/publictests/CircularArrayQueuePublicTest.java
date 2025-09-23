package cz.cvut.fel.pjv.publictests;

import cz.cvut.fel.pjv.impl.CircularArrayQueue;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;


public class CircularArrayQueuePublicTest {

    /**
     * Test of size method, of class CircularArrayQueue.
     */
    @Test
    void testSize() {
        System.out.println("size");
        CircularArrayQueue instance = new CircularArrayQueue();
        int expResult = 0;
        int result = instance.size();
        Assertions.assertEquals(expResult, result, "Queue size function test after creating CircularArrayQueue with default capacity");

        instance.enqueue("Darth Vader");
        expResult = 1;
        result = instance.size();
        Assertions.assertEquals(expResult, result, "Queue size function test after adding one element.");

        instance.dequeue();
        expResult = 0;
        result = instance.size();
        Assertions.assertEquals(expResult, result, "Queue size function test after dequeue.");

        CircularArrayQueue instance2 = new CircularArrayQueue(20);
        int expResult2 = 0;
        int result2 = instance2.size();
        Assertions.assertEquals(expResult2, result2, "Queue size function test after creating CircularArrayQueue with capacity 20");
        instance2.dequeue();

        result2 = instance2.size();
        Assertions.assertEquals(expResult2, result2, "Queue size function test after dequeue on empty queue");
    }

    /**
     * Test of isEmpty method, of class CircularArrayQueue.
     */
    @Test
    void testIsEmpty() {
        System.out.println("isEmpty");
        CircularArrayQueue instance = new CircularArrayQueue(5);
        boolean result = instance.isEmpty();
        Assertions.assertTrue(result, "Queue isEmpty on empty queue should return true");
        instance.enqueue("Admiral Ackbar");
        result = instance.isEmpty();
        Assertions.assertFalse(result, "Queue isEmpty on non empty queue should return false");
        instance.dequeue();
        result = instance.isEmpty();
        Assertions.assertTrue(result, "Queue isEmpty on empty queue should return true");
    }

    /**
     * Test of isFull method, of class CircularArrayQueue.
     */
    @Test
    void testIsFull() {
        System.out.println("isFull");
        CircularArrayQueue instance = new CircularArrayQueue(5);
        Assertions.assertFalse(instance.isFull(), "Queue isFull on empty queue should return false");
        instance.enqueue("General Grievous");
        Assertions.assertFalse(instance.isFull(), "Queue isFull should return false when size < capacity.");
        instance.enqueue("Darth Revan");
        Assertions.assertFalse(instance.isFull(), "Queue isFull should return false when size < capacity.");
        instance.enqueue("Princess Leia");
        Assertions.assertFalse(instance.isFull(), "Queue isFull should return false when size < capacity.");
        instance.enqueue("Anakin Skywalker");
        Assertions.assertFalse(instance.isFull(), "Queue isFull should return false when size < capacity.");
        instance.enqueue("Lando Calrissian");
        Assertions.assertTrue(instance.isFull(), "Queue isFull on full queue should return true");
        instance.dequeue();
        Assertions.assertFalse(instance.isFull(), "Queue isFull should return false when size < capacity.");
    }

    @Test
    void testEnqueueOnFull() {
        CircularArrayQueue instance = new CircularArrayQueue();
        Assertions.assertTrue(instance.enqueue("StormTrooper 1"), "Enqueue on empty queue should return true");
        Assertions.assertTrue(instance.enqueue("StormTrooper 2"), "Enqueue should return true");
        Assertions.assertTrue(instance.enqueue("StormTrooper 3"), "Enqueue should return true");
        Assertions.assertTrue(instance.enqueue("StormTrooper 4"), "Enqueue should return true");
        Assertions.assertTrue(instance.enqueue("StormTrooper 5"), "Enqueue should return true");
        Assertions.assertFalse(instance.enqueue("StormTrooper 6"), "Enqueue on full queue should return false");
        instance.dequeue();
        Assertions.assertTrue(instance.enqueue("StormTrooper 7"), "Enqueue should return true");
    }

    @Test
    void testMultipleEnqueAndDeque() {
        CircularArrayQueue instance = new CircularArrayQueue(5);

        String[] items = {
                "StormTrooper 1",
                "StormTrooper 2",
                "StormTrooper 3",
                "StormTrooper 4",
                "StormTrooper 5"
        };

        for (int i = 0; i < 5; i++) {
            for (String item : items) {
                boolean actual = instance.enqueue(item);
                Assertions.assertTrue(actual, "Insert should be successful");
            }

            for (String expected : items) {
                final String actual = instance.dequeue();
                Assertions.assertEquals(expected, actual, "Queue should provide given values");
            }


            for (int j = 0; j < 4; j++) {
                boolean actual = instance.enqueue(items[j]);
                Assertions.assertTrue(actual,"Queue should provide given values");
            }

            for (int j = 0; j < 4; j++) {
                final String actual = instance.dequeue();
                Assertions.assertEquals(items[j], actual,"Queue should provide given values");
            }
        }
    }

    /**
     * Test of enqueue method, of class CircularArrayQueue.
     */
    @Test
    void testEnqueue() {
        System.out.println("enqueue");
        String obj = "Darth Maul";
        CircularArrayQueue instance = new CircularArrayQueue(5);
        instance.enqueue(obj);
        Assertions.assertEquals(obj, instance.dequeue(), "Queue enqueue test, expected value = true");

        obj = "Mace Windu";
        instance.enqueue(obj);
        Assertions.assertEquals(obj, instance.dequeue(), "Queue enqueue test, expected Mace Windu");

        // Test on null
        Assertions.assertFalse(instance.enqueue(null), "Queue enqueue null test, expected false");
        Assertions.assertEquals(0, instance.size(), "Queue size function test after adding null element.");
    }

    /**
     * Test of dequeue method, of class CircularArrayQueue.
     */
    @Test
    void testDequeue() {
        System.out.println("dequeue");
        CircularArrayQueue instance = new CircularArrayQueue(5);
        String result = instance.dequeue();
        Assertions.assertNull(result,"Test dequeue if CircularArrayQueue is empty");
    }

    @Test
    void testIndicesSwitched() {
        CircularArrayQueue queue = new CircularArrayQueue(10);
        for (int i = 0; i < 3; i++) {
            queue.enqueue("Admiral Ackbar");
            queue.enqueue("Lando Calrissian");
            queue.enqueue("Anakin Skywalker");
            queue.dequeue();
            queue.dequeue();
            queue.dequeue();
        }
        queue.enqueue("Darth Maul");
        queue.enqueue("Darth Vader");
        Assertions.assertEquals(2, queue.size(), "Test on size function when queue is starting again on beginning (indices are switched)");
    }

    @Test
    void testMultipleInstances() {
        String[] items = {
                "StormTrooper 1",
                "StormTrooper 2",
                "StormTrooper 3",
                "StormTrooper 4",
                "StormTrooper 5"
        };

        CircularArrayQueue instance = new CircularArrayQueue();
        for (int i = 0; i < 3; i++) {
            Assertions.assertTrue(instance.enqueue(items[i]), "Enqueue should return true");
        }

        CircularArrayQueue instance2 = new CircularArrayQueue();
        for (String item : items) {
            Assertions.assertTrue(instance2.enqueue(item), "Enqueue should return true");
        }

        Assertions.assertFalse(instance2.enqueue("StormTrooper 6"), "Enqueue on full queue should return false");

        for (String item : items) {
            Assertions.assertEquals(item, instance2.dequeue(), "Dequeue should return the same value as enqueued");
        }

        for (int i = 0; i < 3; i++) {
            Assertions.assertEquals(items[i], instance.dequeue(), "Dequeue should return the same value as enqueued");
        }
    }
}


