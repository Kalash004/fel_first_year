package models.data_classes.message.generic_messages;

import models.data_classes.message.Message;
import models.data_classes.message.MessageWrapper;
import models.data_classes.message.message_content_classes.PortHolderContent;
import models.enums.connection.MessageType;
import models.enums.message_status.MessageStatus;
import utils.json.JsonUtils;

import java.util.Date;

public class ConReqMessage extends MessageWrapper {
    public ConReqMessage(String peerId, int messagePort, String userName) {
        message = new Message();
        message.setPeerId(peerId);
        message.setType(MessageType.CONNECTION_REQUEST);
        message.setTimestamp(new Date());
        message.setContent(JsonUtils.toJson(new PortHolderContent(messagePort)));
        message.setStatus(MessageStatus.OK);
        message.setSenderName(userName);
    }
}
