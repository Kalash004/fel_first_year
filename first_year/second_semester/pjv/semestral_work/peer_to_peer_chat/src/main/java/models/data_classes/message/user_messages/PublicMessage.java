package models.data_classes.message.user_messages;

import configs.ApiConfig;
import models.data_classes.message.Message;
import models.data_classes.message.MessageWrapper;
import models.enums.connection.MessageType;

import java.util.Date;

public class PublicMessage extends MessageWrapper {
    public PublicMessage(String peerId, String content) {
        this.message = new Message();
        message.setPeerId(peerId);
        message.setType(MessageType.MESSAGE_PUBLIC);
        message.setTimestamp(new Date());
        message.setContent(content);
    }

}
