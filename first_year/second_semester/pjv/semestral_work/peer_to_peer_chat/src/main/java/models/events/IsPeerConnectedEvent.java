package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.Arrays;

public class IsPeerConnectedEvent extends Event<Peer, Boolean> {

    public IsPeerConnectedEvent() {
        this.receivers = new ArrayList<>(Arrays.asList(
                EventReceiver.PEER_MANAGER
        ));
        this.type = EventType.IS_PEER_CONNECTED;
    }
}
