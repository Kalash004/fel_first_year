package service_view.commands;

import javafx.util.Pair;
import models.data_classes.message.Message;
import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.peer_manager.Peer;
import service_view.UiController;

public class CommandReceiveTTTInvite implements ICommand {
    private final UiController uiController;
    public CommandReceiveTTTInvite(UiController uiController) {
        this.uiController = uiController;
    }

    @Override
    public void execute(Event<?, ?> input) {

        uiController.onReceivedTTTInvite((Peer) input.getInput());
    }
}
