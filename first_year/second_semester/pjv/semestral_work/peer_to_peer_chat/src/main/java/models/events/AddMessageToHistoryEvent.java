package models.events;

import javafx.util.Pair;
import models.data_classes.message.Message;
import models.enums.event.EventReceiver;
import models.enums.event.EventSender;
import models.enums.event.EventType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.Arrays;

public class AddMessageToHistoryEvent extends Event<Pair<Peer, Message>, Void> {
    public AddMessageToHistoryEvent() {
        this.receivers = new ArrayList<>(Arrays.asList(
                EventReceiver.MESSAGE_SERVICE,
                EventReceiver.UI
        ));
        this.sender = EventSender.API;
        this.type = EventType.ADD_MESSAGE_TO_HISTORY;
    }
}
