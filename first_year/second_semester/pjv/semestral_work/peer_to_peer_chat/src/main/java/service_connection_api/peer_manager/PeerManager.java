package service_connection_api.peer_manager;

import configs.ApiConfig;
import configs.PeerManagerConfig;
import models.enums.event.EventType;
import models.enums.peer.SynchronizationStatus;
import models.events.*;
import models.exceptions.PeerDoesntExist;
import models.interfaces.ICommand;
import models.interfaces.IListen;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api.ApiController;
import service_connection_api.peer_manager.commands.AddPeerCommand;
import service_connection_api.peer_manager.commands.GetLocalPeerIDCommand;
import service_event_handler.EventHandler;
import utils.ip.IpUtils;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

public class PeerManager implements IListen {
    private static final Logger logger = LoggerFactory.getLogger(PeerManager.class);

    private static PeerManager instance;
    private final int peerLiveMs;
    private final HashMap<String, Peer> connectedPeers;
    private String localPeerId = null;
    private final EventHandler eventHandler;
    private final HashMap<EventType, ICommand> commands;
    private String userName;

    public static synchronized PeerManager getInstance() {
        // Block non main calls
        String allowedCaller = "_main.Application"; // fully-qualified class name

        StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
        if (stackTrace.length > 2) {
            String callerClassName = stackTrace[2].getClassName();
            if (!callerClassName.equals(allowedCaller)) {
                throw new SecurityException("Unauthorized caller: " + callerClassName + " PLEASE USE EVENTS OR START FROM " + allowedCaller);
            }
        }
        // end

        if (instance == null) {
            instance = new PeerManager();
        }
        return instance;
    }

    private PeerManager() {
        this.connectedPeers = new HashMap<>();
        this.peerLiveMs = PeerManagerConfig.getPeerExpirationTimeMs();
        this.eventHandler = EventHandler.getInstance();
        this.commands = new HashMap<>(Map.of(
                EventType.CONNECTION_REQUESTED, new AddPeerCommand(this),
                EventType.ADD_PEER, new AddPeerCommand(this),
                EventType.GET_LOCAL_PEER_ID, new GetLocalPeerIDCommand(this),
                EventType.GET_PEER_LIST, new ICommand() {
                    @Override
                    public void execute(Event<?, ?> input) {
                        if (!(input instanceof GetConnectedPeerListEvent)) return;
                        GetConnectedPeerListEvent temp = (GetConnectedPeerListEvent) input;
                        temp.setOutput(getConnectedPeers());
                    }
                },
                EventType.GET_LOCAL_PEER, new ICommand() {
                    @Override
                    public void execute(Event<?, ?> input) {
                        if (!(input instanceof GetLocalPeerEvent)) return;
                        GetLocalPeerEvent temp = (GetLocalPeerEvent) input;
                        temp.setOutput(getLocalPeer());
                    }
                },
                EventType.IS_PEER_CONNECTED, new ICommand() {

                    @Override
                    public void execute(Event<?, ?> input) {
                        if (!(input instanceof IsPeerConnectedEvent)) return;
                        IsPeerConnectedEvent temp = (IsPeerConnectedEvent) input;
                        temp.setOutput(containsPeer(temp.getInput()));
                    }
                },
                EventType.GET_PEER, new ICommand() {

                    @Override
                    public void execute(Event<?, ?> input) {
                        if (!(input instanceof GetConnectedPeerEvent)) return;
                        GetConnectedPeerEvent temp = (GetConnectedPeerEvent) input;
                        temp.setOutput(getPeer(temp.getInput()));
                    }
                },
                EventType.REMOVE_PEER, new ICommand() {

                    @Override
                    public void execute(Event<?, ?> input) {
                        if (!(input instanceof RemovePeerEvent)) return;
                        RemovePeerEvent temp = (RemovePeerEvent) input;
                        Peer peer = getPeer(temp.getInput());
                        removePeer(peer);
                    }
                }
        ));
    }

    public Boolean addPeer(Peer peer) throws NullPointerException {
        if (peer == null) {
            // TODO: maybe log ?
            throw new NullPointerException("Null peer");
        }
        if (peer.getPeerId() == null) {
            throw new NullPointerException("Null peerId for peer : " + peer);
        }
        if (this.connectedPeers.containsKey(peer.getPeerId())) {
            logger.info("Peer " + peer.getPeerId() + " is already connected");
            return true;
        }
        this.connectedPeers.put(peer.getPeerId(), peer);
        logger.info("Added peer {} to peer", peer.getPeerId());

        this.sendConAck(peer);

        return true;
    }

    public void removePeer(Peer peer) throws PeerDoesntExist, NullPointerException {
        if (peer == null) {
            throw new NullPointerException("Null peer");
        }
        if (!this.connectedPeers.containsKey(peer.getPeerId())) {
            throw new PeerDoesntExist("Peer : " + peer);
        }
        this.connectedPeers.remove(peer.getPeerId());
    }

    public void removeOldPeers() {
        for (Peer peer : this.connectedPeers.values()) {
            long duration = new Date().getTime() - peer.getLastCheckTime().getTime();
            long peerLifeTimeMs = TimeUnit.MILLISECONDS.convert(duration, TimeUnit.MILLISECONDS);
            if (peerLifeTimeMs > this.peerLiveMs) {
                this.connectedPeers.remove(peer.getPeerId());
            }
        }
    }

    public static String generatePeerId() {
        UUID uuid = UUID.randomUUID();
        return uuid.toString();
    }

    public String getLocalPeerId() {
        if (localPeerId == null) {
            localPeerId = generatePeerId();
        }
        return this.localPeerId;
    }

    public Peer getLocalPeer() {
        try {
            return new Peer(this.localPeerId, ApiConfig.getUserName(), InetAddress.getByName(IpUtils.getLocalIp()), ApiConfig.getMessagingPort());
        } catch (UnknownHostException e) {
            throw new RuntimeException(e);
        }
    }

    public Peer getPeer(String peerId) {
        return this.connectedPeers.get(peerId);
    }

    public boolean containsPeer(Peer peer) {
        return containsPeerId(peer.getPeerId());
    }

    public boolean containsPeerId(String peerId) {
        return this.connectedPeers.containsKey(peerId);
    }

    private void sendConAck(Peer peer) {

    }

    public Peer[] getConnectedPeers() {
        return this.connectedPeers.values().toArray(new Peer[0]);
    }

    @Override
    public void handleEvent(Event<?, ?> event) {
        if (!this.commands.containsKey(event.getType())) {
            // TODO: Log
            return;
        }
        this.commands.get(event.getType()).execute(event);
    }

    public boolean AddPeer(Peer peer) {
        if (peer == null) {
            throw new RuntimeException("Null peer");
        }
        peer.setLastCheckTime(new Date());
        peer.setStatus(SynchronizationStatus.PEER_REQUESTED_CONNECTION);
        this.connectedPeers.put(peer.getPeerId(), peer);
        return true;
    }
}
