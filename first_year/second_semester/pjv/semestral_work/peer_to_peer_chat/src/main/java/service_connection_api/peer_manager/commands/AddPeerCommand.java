package service_connection_api.peer_manager.commands;

import models.enums.event.EventType;
import models.events.Event;
import models.interfaces.ICommand;
import service_connection_api.peer_manager.Peer;
import service_connection_api.peer_manager.PeerManager;

public class AddPeerCommand implements ICommand {
    private final PeerManager peerManager;

    public AddPeerCommand(PeerManager peerManager) {
        this.peerManager = peerManager;
    }

    @Override
    public void execute(Event<?, ?> input) {
        Event<Peer, Boolean> event = (Event<Peer, Boolean>) input;
        Peer peer = event.getInput();
        event.setOutput(peerManager.addPeer(peer));
    }
}
