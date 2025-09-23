package models.events;

import models.data_classes.message.Message;
import models.enums.event.EventReceiver;
import models.enums.event.EventType;
import models.enums.message_history_type.MessageHistoryType;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

public class GetMessageHistoryEvent extends Event<MessageHistoryType, HashMap<Peer, ArrayList<Message>>> {
    public GetMessageHistoryEvent() {
        this.receivers = new ArrayList<>(List.of(EventReceiver.MESSAGE_SERVICE));
        this.type = EventType.GET_MESSAGE_HISTORY;
    }
}
