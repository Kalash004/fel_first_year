package models.exceptions;

public class MessageDoesntExistRuntimeException extends RuntimeException {
    public MessageDoesntExistRuntimeException() {
        super("Message doesn't exist");
    }

}
