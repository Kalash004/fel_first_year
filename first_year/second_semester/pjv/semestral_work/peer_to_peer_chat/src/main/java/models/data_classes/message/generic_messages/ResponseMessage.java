package models.data_classes.message.generic_messages;

import models.data_classes.message.Message;
import models.data_classes.message.MessageWrapper;
import models.enums.connection.MessageType;
import models.enums.message_status.MessageStatus;

public class ResponseMessage extends MessageWrapper {

    public ResponseMessage(String peerId, String userName, MessageStatus messageStatus) {
        this.message = new Message();
        this.message.setPeerId(peerId);
        this.message.setType(MessageType.RESPONSE);
        this.message.setStatus(MessageStatus.OK);
        this.message.setSenderName(userName);
    }
}
