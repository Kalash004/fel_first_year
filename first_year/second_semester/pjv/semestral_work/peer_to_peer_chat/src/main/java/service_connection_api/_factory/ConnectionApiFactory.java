package service_connection_api._factory;

import configs.ApiConfig;
import models.events.GetLocalPeerIDEvent;
import service_connection_api.discovery.DiscoveryClient;
import service_connection_api.discovery.DiscoveryServer;
import service_connection_api.peer_manager.PeerManager;
import service_connection_api.sending.MessageClient;
import service_event_handler.EventHandler;

/**
 * Gets connection options from settings and creates neeeded classes
 */
public class ConnectionApiFactory {

    public static DiscoveryServer getDiscoveryServer() {
        return new DiscoveryServer(ApiConfig.getDiscoveryServerSourcePort(),
                ApiConfig.getDefualtPacketSize(), ApiConfig.getDiscoveryTimeoutMillis(), ApiConfig.getDiscoveryServerTargetPort());
    }

    public static DiscoveryClient getDiscoveryClient() {
        GetLocalPeerIDEvent getLocalPeerIDEvent = new GetLocalPeerIDEvent();
        EventHandler.getInstance().handleEvent(getLocalPeerIDEvent);
        String localPeer = getLocalPeerIDEvent.getOutput();
        return new DiscoveryClient(ApiConfig.getDiscoveryServerTargetPort(), ApiConfig.getDefualtPacketSize(),
                localPeer,
                ApiConfig.getDiscoveryTimeoutMillis(), ApiConfig.getDiscoverySocketSleep(),
                ApiConfig.getDiscoveryBroadcastIpAddress(), ApiConfig.getDiscoveryClientSourcePort(),
                ApiConfig.getMessagingPort(), ApiConfig.getUserName());
    }

    public static MessageClient getMessagingClient() {
        return new MessageClient(ApiConfig.getMessagingTimeoutMillis());
    }
}
