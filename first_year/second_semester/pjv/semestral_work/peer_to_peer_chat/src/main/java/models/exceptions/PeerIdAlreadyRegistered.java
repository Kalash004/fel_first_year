package models.exceptions;

public class PeerIdAlreadyRegistered extends RuntimeException {
    private static final String m = "Peer ID already registered";
    public PeerIdAlreadyRegistered(String message) {
        super(m + " " + message);
    }
}
