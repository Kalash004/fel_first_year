package service_coroutine.commands;

import models.events.Event;
import models.interfaces.ICommand;
import service_coroutine.CoroutineService;

public class CommandStartWatchdog implements ICommand {
    private final CoroutineService watchdog;
    public CommandStartWatchdog(CoroutineService watchdog) {
        this.watchdog = watchdog;
    }

    @Override
    public void execute(Event<?, ?> event) {
        watchdog.start();
    }
}
