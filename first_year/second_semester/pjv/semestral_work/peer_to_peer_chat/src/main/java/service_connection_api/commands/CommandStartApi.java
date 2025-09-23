package service_connection_api.commands;

import models.enums.event.EventType;
import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.ApiController;

public class CommandStartApi implements ICommand {
    private final ApiController apiController;
    public CommandStartApi(ApiController apiController) {
        this.apiController = apiController;
    }

    @Override
    public void execute(Event<?, ?> input) {
        apiController.launch();
    }
}
