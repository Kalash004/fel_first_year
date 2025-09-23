package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SendConReqEvent extends Event<Peer,Void> {
    public SendConReqEvent() {
        this.receivers = new ArrayList<>(List.of(EventReceiver.API));
        this.type = EventType.SEND_CONREQ;
    }
}
