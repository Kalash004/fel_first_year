package service_connection_api.commands;

import models.data_classes.message.Message;
import models.enums.event.EventType;
import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.ApiController;
import service_connection_api.peer_manager.Peer;
import service_message_history.MessageServiceController;

public class CommandGetPeerList implements ICommand {
    private final ApiController apiController;
    public CommandGetPeerList(ApiController apiController) {
        this.apiController = apiController;
    }

    @Override
    public void execute(Event<?, ?> input) {
    }
}
