package models.exceptions;

public class PeerDoesntExist extends RuntimeException {
    public PeerDoesntExist(String message) {
      super("Peer is not registered in PeerManager " + message);
    }
}
