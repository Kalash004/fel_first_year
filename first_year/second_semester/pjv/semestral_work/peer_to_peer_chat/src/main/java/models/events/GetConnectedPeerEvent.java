package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.List;

public class GetConnectedPeerEvent extends Event<String, Peer> {
    public GetConnectedPeerEvent() {
        this.receivers = new ArrayList<>(List.of(
                EventReceiver.PEER_MANAGER
        ));
        this.type = EventType.GET_PEER;
    }
}
