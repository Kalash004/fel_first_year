package models.data_classes.message.generic_messages;

import models.data_classes.message.Message;
import models.data_classes.message.MessageWrapper;
import models.data_classes.message.message_content_classes.MessageHistoryAndPeersContent;
import models.enums.connection.MessageType;
import models.enums.message_status.MessageStatus;
import service_connection_api.peer_manager.Peer;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;

public class ConAckMessage extends MessageWrapper {
    public ConAckMessage(String peerId, HashMap<Peer, ArrayList<Message>> messageHistory, Peer[] connectedPeers) {
        message = new Message();
        message.setPeerId(peerId);
        message.setType(MessageType.CONNECTION_ACKNOWLEDGEMENT);
        message.setTimestamp(new Date());
        message.setStatus(MessageStatus.OK);
        message.setContent(new MessageHistoryAndPeersContent(messageHistory, connectedPeers).toJson());
    }
}
