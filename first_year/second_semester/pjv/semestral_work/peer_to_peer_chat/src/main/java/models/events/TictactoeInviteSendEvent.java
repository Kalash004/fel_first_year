package models.events;

import models.data_classes.message.Message;
import models.enums.event.EventReceiver;
import models.enums.event.EventType;

import java.util.ArrayList;
import java.util.Arrays;

public class TictactoeInviteSendEvent extends Event<Message, Void>{
    public TictactoeInviteSendEvent() {
        this.receivers = new ArrayList<>(Arrays.asList(
                EventReceiver.MESSAGE_SERVICE,
                EventReceiver.API
        ));
        this.type = EventType.PRIVATE_MESSAGE_SEND;
    }
}
