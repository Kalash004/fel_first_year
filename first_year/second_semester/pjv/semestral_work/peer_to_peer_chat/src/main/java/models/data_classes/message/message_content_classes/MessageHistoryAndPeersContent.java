package models.data_classes.message.message_content_classes;

import models.data_classes.message.Message;
import service_connection_api.peer_manager.Peer;
import utils.json.JsonUtils;

import java.util.ArrayList;
import java.util.HashMap;

public class MessageHistoryAndPeersContent {
    public HashMap<Peer, ArrayList<Message>> messages;
    public Peer[] peers;

    public MessageHistoryAndPeersContent(HashMap<Peer, ArrayList<Message>> messages, Peer[] peers) {
        this.messages = messages;
        this.peers = peers;
    }

    public MessageHistoryAndPeersContent() {
    }

    public String toJson() {
        return JsonUtils.toJson(this);
    }
}
