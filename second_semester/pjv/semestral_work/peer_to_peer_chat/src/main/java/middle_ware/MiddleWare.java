package middle_ware;


import models.abstracts.event_handling.Event;
import models.interfaces.IListen;

/**
 * Middle ware endpoint,
 * possible use for minigames such as tick tack toe in chat
 * @author Anton
 * @version 0.1
 */
public class MiddleWare implements Runnable, IListen {

    private static MiddleWare instance = new MiddleWare();

    private MiddleWare() {
    }

    public static MiddleWare getInstance() {
        return instance;
    }

    public void launch() {
        // TODO:

    }

    @Override
    public void run() {

    }

    @Override
    public void handleEvent(Event event) {

    }
}