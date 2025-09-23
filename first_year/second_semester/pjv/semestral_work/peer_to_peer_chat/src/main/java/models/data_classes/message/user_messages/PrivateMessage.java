package models.data_classes.message.user_messages;

import configs.ApiConfig;
import models.data_classes.message.Message;
import models.data_classes.message.MessageWrapper;
import models.enums.connection.MessageType;

import java.util.Date;

public class PrivateMessage extends MessageWrapper {

    public PrivateMessage(String content) {
        this.message = new Message();
        message.setPeerId(ApiConfig.getPeerId());
        message.setType(MessageType.MESSAGE_PRIVATE);
        message.setTimestamp(new Date());
        message.setContent(content);
    }

}
