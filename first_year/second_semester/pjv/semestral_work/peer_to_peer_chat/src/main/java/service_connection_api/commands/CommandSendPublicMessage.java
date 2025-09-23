package service_connection_api.commands;

import models.data_classes.message.Message;
import models.enums.event.EventType;
import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.ApiController;

public class CommandSendPublicMessage implements ICommand {
    private final ApiController apiController;
    public CommandSendPublicMessage(ApiController apiController) {
        this.apiController = apiController;
    }

    @Override
    public void execute(Event<?, ?> input) {
        apiController.sendPublicMessage((String) input.getInput());
    }
}
