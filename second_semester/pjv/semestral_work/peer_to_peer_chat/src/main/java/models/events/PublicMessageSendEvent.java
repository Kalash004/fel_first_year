package models.events;

import models.abstracts.event_handling.Event;
import models.enums.event.EventReciever;
import models.enums.event.EventSender;
import models.enums.event.EventType;

public class PublicMessageSendEvent extends Event {

    private EventSender sender;
    private EventReciever reciever;
    private final EventType type = EventType.PUBLIC_MESSAGE_SEND;

    private PublicMessageSendEvent() {}

    @Override
    public EventReciever getReciever() {
        return reciever;
    }

    @Override
    public EventSender getSender() {
        return sender;
    }

    @Override
    public EventType getType() {
        return type;
    }
}
