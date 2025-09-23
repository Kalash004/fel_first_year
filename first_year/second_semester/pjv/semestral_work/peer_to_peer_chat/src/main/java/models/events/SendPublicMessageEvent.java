package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventSender;
import models.enums.event.EventType;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SendPublicMessageEvent extends Event<String, Void> {

    public SendPublicMessageEvent() {
        this.receivers = new ArrayList<>(List.of(
                EventReceiver.API
        ));
        this.sender = EventSender.UI;
        this.type = EventType.SEND_PUBLIC_MESSAGE;
    }

}
