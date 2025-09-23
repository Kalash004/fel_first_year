package service_view.commands;

import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.peer_manager.Peer;
import service_view.UiController;

public class CommandRemovePeer implements ICommand {
    private final UiController uiController;
    public CommandRemovePeer(UiController uiController) {
        this.uiController = uiController;
    }

    @Override
    public void execute(Event<?, ?> input) {
        uiController.removePeer();
    }
}
