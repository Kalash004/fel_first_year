package models.enums.connection;

public enum MessageType {
    HELLO,
    CONNECTION_ACKNOWLEDGEMENT,
    CONNECTION_REQUEST,
    SYNCHRONIZATION,
    MESSAGE_PUBLIC,
    RESPONSE, BYE, MESSAGE_PRIVATE
}
