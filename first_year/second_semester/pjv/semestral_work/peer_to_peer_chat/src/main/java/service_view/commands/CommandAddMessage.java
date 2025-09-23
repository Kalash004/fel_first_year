package service_view.commands;

import javafx.util.Pair;
import models.data_classes.message.Message;
import models.enums.event.EventType;
import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.peer_manager.Peer;
import service_view.UiController;

public class CommandAddMessage  implements ICommand {
    private final UiController uiController;
    public CommandAddMessage(UiController uiController) {
        this.uiController = uiController;
    }

    @Override
    public void execute(Event<?, ?> input) {
        uiController.addMessage((Pair<Peer, Message>) input.getInput());
    }
}
