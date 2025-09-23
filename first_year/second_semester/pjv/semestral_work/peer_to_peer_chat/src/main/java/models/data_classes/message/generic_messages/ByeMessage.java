package models.data_classes.message.generic_messages;

import models.data_classes.message.Message;
import models.data_classes.message.MessageWrapper;
import models.data_classes.message.message_content_classes.MessageHistoryAndPeersContent;
import models.enums.connection.MessageType;
import models.enums.message_status.MessageStatus;

import java.util.Date;
import java.util.HashMap;


public class ByeMessage extends MessageWrapper {
    public ByeMessage(String peerId) {
        message = new Message();
        message.setPeerId(peerId);
        message.setType(MessageType.BYE);
        message.setTimestamp(new Date());
        message.setStatus(MessageStatus.OK);
    }
}
