package models.events;

import models.data_classes.message.Message;
import models.enums.event.EventReceiver;
import models.enums.event.EventType;

import java.util.ArrayList;
import java.util.Arrays;

public class HandlePublicMessageEvent extends Event<Message, Void> {
    public HandlePublicMessageEvent() {
        this.type = EventType.HANDLE_PUBLIC_MESSAGE;
        this.receivers = new ArrayList<>(Arrays.asList(
                EventReceiver.MESSAGE_SERVICE,
                EventReceiver.UI
        ));
    }
}
