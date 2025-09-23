package service_message_history.commands;

import javafx.util.Pair;
import models.data_classes.message.Message;
import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.peer_manager.Peer;
import service_message_history.MessageServiceController;

public class CommandAddPrivateMessage implements ICommand {
    private final MessageServiceController messageServiceController;
    public CommandAddPrivateMessage(MessageServiceController controller) {
        this.messageServiceController = controller;
    }

    @Override
    public void execute(Event<?, ?> input) {
        Event<Pair<Peer, Message>, ?> temp = (Event<Pair<Peer, Message>, ?>)input;
        messageServiceController.addPrivateMessage(temp.getInput().getKey(), temp.getInput().getValue());
    }
}
