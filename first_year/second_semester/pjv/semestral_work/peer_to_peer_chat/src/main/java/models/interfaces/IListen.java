package models.interfaces;

import models.events.Event;

public interface IListen {
    void handleEvent(Event<?, ?> event);
}
