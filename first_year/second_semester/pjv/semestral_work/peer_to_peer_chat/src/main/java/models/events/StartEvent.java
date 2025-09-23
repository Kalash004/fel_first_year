package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventSender;
import models.enums.event.EventType;

import java.util.ArrayList;
import java.util.Arrays;

public class StartEvent extends Event<Void,Void> {
    public StartEvent() {
        this.type = EventType.PROGRAM_START;
        this.receivers = new ArrayList<EventReceiver>(Arrays.asList(
                EventReceiver.API,
                EventReceiver.COROUTINE
        ));
        this.sender = EventSender.PROGRAM;
    }
}
