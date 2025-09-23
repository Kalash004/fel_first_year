package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;

import java.util.ArrayList;
import java.util.List;

public class StopProgramEvent extends Event<Void, Void> {
    public StopProgramEvent() {
        this.type = EventType.PROGRAM_END;
        this.receivers = new ArrayList<>(List.of(
                EventReceiver.MESSAGE_SERVICE,
                EventReceiver.UI,
                EventReceiver.API,
                EventReceiver.PEER_MANAGER,
                EventReceiver.MIDDLE_WARE,
                EventReceiver.COROUTINE
        ));
    }
}
