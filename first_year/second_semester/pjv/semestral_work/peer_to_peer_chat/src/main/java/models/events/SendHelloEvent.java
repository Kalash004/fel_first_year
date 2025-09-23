package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventSender;
import models.enums.event.EventType;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SendHelloEvent extends Event<String, Void> {
    public SendHelloEvent() {
        this.type = EventType.SEND_HELLO;
        this.receivers = new ArrayList<EventReceiver>(List.of(
                EventReceiver.API
        ));
        this.sender = EventSender.UI;
    }
}
