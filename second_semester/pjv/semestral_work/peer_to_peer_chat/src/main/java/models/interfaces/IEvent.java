package models.interfaces;

import models.enums.event.EventReciever;
import models.enums.event.EventSender;
import models.enums.event.EventType;

public interface IEvent {
    public EventReciever getReciever();
    public EventSender getSender();
    public EventType getType();
}
