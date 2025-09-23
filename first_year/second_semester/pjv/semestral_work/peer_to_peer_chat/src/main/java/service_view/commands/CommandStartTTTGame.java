package service_view.commands;

import models.events.Event;
import models.interfaces.ICommand;
import service_view.UiController;

public class CommandStartTTTGame implements ICommand {
    private final UiController uiController;
    public CommandStartTTTGame(UiController uiController) {
        this.uiController = uiController;
    }

    @Override
    public void execute(Event<?, ?> input) {
        uiController.onStartTTTGame(true);
    }
}
