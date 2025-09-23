package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventSender;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.Arrays;

public class AddPeerEvent extends Event<Peer, Void> {
    public AddPeerEvent() {
        this.type = EventType.ADD_PEER;
        this.receivers = new ArrayList<>(Arrays.asList(

                EventReceiver.PEER_MANAGER,
                EventReceiver.UI
        ));
        this.sender = EventSender.API;
    }
}
