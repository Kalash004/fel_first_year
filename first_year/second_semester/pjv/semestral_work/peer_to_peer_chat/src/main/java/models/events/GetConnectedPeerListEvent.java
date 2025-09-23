package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.List;

public class GetConnectedPeerListEvent extends  Event<Void, Peer[]> {
    public GetConnectedPeerListEvent() {
        this.receivers = new ArrayList<>(List.of(EventReceiver.PEER_MANAGER));
        //this.sender = EventSender.UI;
        this.type = EventType.GET_PEER_LIST;
    }
}
