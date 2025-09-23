package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.Arrays;

public class ConnectionRequestEvent extends Event<Peer, Boolean> {
    public ConnectionRequestEvent() {
        this.type = EventType.CONNECTION_REQUESTED;
        this.receivers = new ArrayList<>(Arrays.asList(
                EventReceiver.PEER_MANAGER,
                EventReceiver.API
        ));
    }

}
