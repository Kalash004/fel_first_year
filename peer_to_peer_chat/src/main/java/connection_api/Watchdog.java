package connection_api;

import models.abstracts.event_handling.Event;
import models.interfaces.IListen;

/**
 * Watches over processes
 * if stuck terminates
 * maybe counts time ?
 */
public class Watchdog implements Runnable, IListen {
    @Override
    public void run() {

    }

    @Override
    public void handleEvent(Event event) {

    }
}
