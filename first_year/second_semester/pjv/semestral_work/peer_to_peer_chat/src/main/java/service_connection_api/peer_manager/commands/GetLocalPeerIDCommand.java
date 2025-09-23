package service_connection_api.peer_manager.commands;

import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.peer_manager.Peer;
import service_connection_api.peer_manager.PeerManager;

public class GetLocalPeerIDCommand implements ICommand {
    private final PeerManager peerManager;

    public GetLocalPeerIDCommand(PeerManager peerManager) {
        this.peerManager = peerManager;
    }

    @Override
    public void execute(Event<?, ?> input) {
        Event<Peer, String> event = (Event<Peer, String>) input;
        event.setOutput(peerManager.getLocalPeerId());
    }
}
