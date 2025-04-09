package models.abstracts.event_handling;

import models.enums.event.EventReciever;
import models.enums.event.EventSender;
import models.enums.event.EventType;
import models.interfaces.IEvent;

public abstract class Event implements IEvent {
    protected EventType type;
    protected EventSender sender;

}
