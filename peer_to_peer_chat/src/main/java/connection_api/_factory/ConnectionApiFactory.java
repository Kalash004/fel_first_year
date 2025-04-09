package connection_api._factory;

import connection_api.discovery.DiscoveryClient;
import connection_api.discovery.DiscoveryServer;
import data_classes.connection.ConnectionSettings;

/**
 * Gets connection options from settings and creates neeeded classes
 */
public class ConnectionApiFactory {
    private ConnectionApiFactory discoveryClient;

    public DiscoveryServer getDiscoveryServer() {
        return new DiscoveryServer();
    }

    public DiscoveryClient getDiscoveryClient() {
        ConnectionSettings connectionSettings = getDiscoveryClientSettings();
        return new DiscoveryClient(connectionSettings.ip,connectionSettings.port);
    }

    private ConnectionSettings getDiscoveryClientSettings() {
        return null;
    }
}
