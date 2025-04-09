package connection_api;

import models.abstracts.event_handling.Event;
import models.interfaces.IListen;

public class ApiController implements IListen, Runnable {
    private static ApiController instance = new ApiController();

    private ApiController() {
    }

    public static ApiController getInstance() {
        return instance;
    }

    public void launch() {
        //TODO:
    }

    @Override
    public void run() {

    }

    @Override
    public void handleEvent(Event event) {

    }
}