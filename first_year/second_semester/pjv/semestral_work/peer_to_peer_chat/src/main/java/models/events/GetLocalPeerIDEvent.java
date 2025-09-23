package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.List;

public class GetLocalPeerIDEvent extends Event<Void, String> {
    public GetLocalPeerIDEvent() {
        this.receivers = new ArrayList<>(List.of(EventReceiver.PEER_MANAGER));
        //this.sender = EventSender.UI;
        this.type = EventType.GET_LOCAL_PEER_ID;
    }

}
