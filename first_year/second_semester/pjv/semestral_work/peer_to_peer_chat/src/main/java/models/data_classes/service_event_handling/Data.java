package models.data_classes.service_event_handling;

import models.data_classes.message.Message;
import models.exceptions.MessageDoesntExistRuntimeException;

public class Data {
    private final Message message = null;

    public Message getMessage() throws MessageDoesntExistRuntimeException {
        if (message == null) {
            throw new MessageDoesntExistRuntimeException();
        }
        return message;
    }
}
