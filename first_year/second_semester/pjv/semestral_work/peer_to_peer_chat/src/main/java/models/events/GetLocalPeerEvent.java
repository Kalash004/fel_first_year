package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.Arrays;

public class GetLocalPeerEvent extends Event<Void, Peer> {
    public GetLocalPeerEvent() {
        this.receivers = new ArrayList<>(Arrays.asList(
                EventReceiver.PEER_MANAGER
        ));
        this.type = EventType.GET_LOCAL_PEER;
    }
}
