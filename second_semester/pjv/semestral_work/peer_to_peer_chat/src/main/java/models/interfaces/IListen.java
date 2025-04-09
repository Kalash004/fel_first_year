package models.interfaces;

import models.abstracts.event_handling.Event;

public interface IListen {
    public void handleEvent(Event event);
}
