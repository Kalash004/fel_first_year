package models.abstracts.event_handling;

import models.enums.event.EventReciever;
import models.enums.event.EventType;

import java.util.ArrayList;

public abstract class Filter {
    protected EventType type;
    protected ArrayList<EventReciever> recievers;

    public Filter() {}

    public ArrayList<EventReciever> getRecievers() {
        return recievers;
    }

    public  EventType getType() {
        return type;
    }
}
