package service_connection_api.commands;

import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.ApiController;

public class CommandProgramEnd implements ICommand {
    private final ApiController apiController;

    public CommandProgramEnd(ApiController apiController) {
        this.apiController = apiController;
    }

    @Override
    public void execute(Event<?, ?> input) {
        apiController.sendBye();
        apiController.stop();
    }
}
