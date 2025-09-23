package models.data_classes.message.generic_messages;

import models.data_classes.message.Message;
import models.data_classes.message.MessageWrapper;
import models.data_classes.message.message_content_classes.PortHolderContent;
import models.enums.connection.MessageType;
import models.enums.message_status.MessageStatus;
import utils.json.JsonUtils;

import java.util.Date;

public class DiscoveryHelloMessage extends MessageWrapper {
    public DiscoveryHelloMessage(String peerId, int messagePort, String userName) {
        this.message = new Message();
        message.setPeerId(peerId);
        message.setType(MessageType.HELLO);
        message.setTimestamp(new Date());
        message.setContent(JsonUtils.toJson(new PortHolderContent(messagePort)));
        message.setStatus(MessageStatus.OK);
        message.setSenderName(userName);
    }
}
