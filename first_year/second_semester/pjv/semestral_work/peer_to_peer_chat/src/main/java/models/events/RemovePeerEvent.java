package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;

import java.util.ArrayList;
import java.util.List;

public class RemovePeerEvent extends Event<String, Void> {
    public RemovePeerEvent() {
        this.receivers = new ArrayList<>(List.of(
                EventReceiver.PEER_MANAGER,
                EventReceiver.UI
        ));
        this.type = EventType.REMOVE_PEER;
    }
}
