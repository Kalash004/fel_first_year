package service_message_history.commands;

import models.enums.message_history_type.MessageHistoryType;
import models.events.Event;
import models.data_classes.message.Message;
import models.enums.event.EventType;
import models.events.GetMessageHistoryEvent;
import models.interfaces.ICommand;
import service_connection_api.peer_manager.Peer;
import service_message_history.MessageServiceController;

import java.util.ArrayList;
import java.util.HashMap;

public class CommandGetMessageHistory implements ICommand {
    private final MessageServiceController messageServiceController;
    public CommandGetMessageHistory(MessageServiceController controller) {
        this.messageServiceController = controller;
    }

    @Override
    public void execute(Event<?, ?> input) {

        HashMap<Peer, ArrayList<Message>> messages;
        if (!(input instanceof GetMessageHistoryEvent)) {
           return;
        }
        Event<MessageHistoryType, HashMap<Peer, ArrayList<Message>>> temp = (Event<MessageHistoryType, HashMap<Peer, ArrayList<Message>>>)input;
        if(temp.getInput().equals(MessageHistoryType.PRIVATE)) {
            messages = messageServiceController.getPrivateMessageHistory();
        } else if (temp.getInput().equals(MessageHistoryType.PUBLIC)) {
            messages = messageServiceController.getMessageHistory();
        } else {
            return; // TODO: error
        }
        temp.setOutput(messages);
    }
}
