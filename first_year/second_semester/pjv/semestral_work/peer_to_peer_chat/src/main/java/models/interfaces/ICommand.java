package models.interfaces;

import models.events.Event;
import models.enums.event.EventType;

public interface ICommand {
    void execute(Event<?, ?> input);
}
