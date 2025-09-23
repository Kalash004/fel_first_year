package service_connection_api.commands;

import models.enums.event.EventType;
import models.events.ConnectionRequestEvent;
import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.ApiController;

public class CommandSendConAck implements ICommand {
    private final ApiController apiController;
    public CommandSendConAck(ApiController apiController) {
        this.apiController = apiController;
    }

    @Override
    public void execute(Event<?, ?> input) {
        if (!(input instanceof ConnectionRequestEvent connectionRequestEvent)) {
            return;
        }
        Boolean res = apiController.sendConAck(connectionRequestEvent.getInput());
        connectionRequestEvent.setOutput(res);
    }
}
