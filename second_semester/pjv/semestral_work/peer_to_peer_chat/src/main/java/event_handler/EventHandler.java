package event_handler;

import models.abstracts.event_handling.Event;
import models.abstracts.event_handling.Filter;
import models.enums.event.EventReciever;
import models.enums.event.EventType;
import models.interfaces.IListen;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;

public class EventHandler implements Runnable {
    private boolean runFlag = true;
    private static EventHandler instance;
    private Queue<Event> events;
    private HashMap<EventReciever, IListen> listeners;
    private HashMap<EventType, Filter> filter;

    private EventHandler() {}

    public static EventHandler getInstance(HashMap<EventType, Filter> filters) {
        if (instance == null) {
            instance = new EventHandler();
            instance.events = new LinkedList<Event>();
            instance.listeners = new HashMap<EventReciever, IListen>();
            instance.filter = filters;
        }
        return instance;
    }

    public void run() {
        // TODO: run
        while (runFlag) {
            processEvent();
        }
    }

    public void addEvent(Event event) {
        events.add(event);
    }

    private void processEvent() {
        Event event = events.poll();
        if (event == null) {
            // TODO: handle exceptions
            // TODO: logger add log of exception
            return;
        }
        if (!filter.containsKey(event.getType())) {
            // TODO: handle exceptions
            // TODO: logger add log of exception
            return;
        }
        ArrayList<EventReciever> tempRecievers = filter.get(event.getType()).getRecievers();
        for (EventReciever reciever : tempRecievers) {
            if (!listeners.containsKey(reciever)) {
                // TODO: exceptions
                // TODO: logger add log of exception
                return;
            }
            // TODO: possible middle ware preprocessing add here
            IListen listener = listeners.get(reciever);
            listener.handleEvent(event);
        }
    }

    public void addListener(EventReciever receiver, IListen listener)
    {
        this.listeners.put(receiver, listener);
    }

}
