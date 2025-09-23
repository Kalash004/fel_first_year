package service_connection_api.reading;

import configs.ApiConfig;
import models.events.GetLocalPeerIDEvent;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api.peer_manager.PeerManager;
import service_event_handler.EventHandler;
import service_message_history.MessageServiceController;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.ArrayList;

public class MessageServer implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(MessageServer.class);

    private int port;
    private boolean runFlag;
    private static MessageServer instance;
    private ServerSocket serverSocket;
    private ArrayList<ClientServer> clientServers;
    private String peerId;
    private String userName;
    private Thread thread;
    private EventHandler eventHandler;

    public static MessageServer getInstance() {
        if (instance == null) {
            instance = new MessageServer();
        }
        return instance;
    }

    private MessageServer() {

    }

    public void testingSetPort(int port) {
        this.port = port;
    }

    @Override
    public void run() {
        logger.info("MessageServer started, listening for connections on {}:{}", this.serverSocket.getInetAddress().getHostAddress(), this.serverSocket.getLocalPort());
        while (runFlag) {
            Socket clientSocket;
            try {
                clientSocket = serverSocket.accept();
            } catch (SocketTimeoutException e) {
                continue; // normal, loop again
            } catch (IOException e) {
                logger.error("Error accepting client connection", e);
                throw new RuntimeException(e);
            }

            ClientServer clientServer = new ClientServer(this, clientSocket, this.peerId, this.userName, this.eventHandler);
            clientServer.start();
            logger.info("Accepted connection from {}", clientSocket.getRemoteSocketAddress());
            this.clientServers.add(clientServer);
        }
        logger.info("MessageServer stopping...");
        this.thread.interrupt();
    }

    public void start() {
        this.runFlag = true;
        this.eventHandler = EventHandler.getInstance();
        this.clientServers = new ArrayList<>();
        this.port = ApiConfig.getMessagingPort();
        GetLocalPeerIDEvent event = new GetLocalPeerIDEvent();
        eventHandler.handleEvent(event);
        this.peerId = event.getOutput();
        this.userName = ApiConfig.getUserName();
        this.thread = new Thread(this, "MessageServer");

        logger.info("Starting MessageServer thread...");
        try {
            this.serverSocket = new ServerSocket(this.port);
            serverSocket.setSoTimeout(500);
            logger.info("MessageServer initialized on port {}", this.port);
        } catch (IOException e) {
            logger.error("Failed to initialize MessageServer on port {}", this.port, e);
            throw new RuntimeException(e);
        }
        if (this.thread == null) {
            this.thread = new Thread(this, "MessageServer");
        }
        this.thread.start();
    }

    public void stop() {
        logger.info("Stopping MessageServer...");
        this.runFlag = false;
        for (ClientServer clientServer : this.clientServers) {
            clientServer.stop();
        }
        try {
            this.thread.join();
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        this.clientServers.clear();
        this.thread = null;
    }


    public void join() {
        if (this.thread == null) {
            return;
        }
        for (ClientServer clientServer : this.clientServers) {
            clientServer.join();
        }
        try {
            this.thread.join();
            logger.info("MessageServer thread joined.");
        } catch (InterruptedException e) {
            logger.error("Interrupted while joining MessageServer thread", e);
            throw new RuntimeException(e);
        }
    }

    public ClientServer[] getClientServer() {
        return this.clientServers.toArray(new ClientServer[0]);
    }

    public void removeClientServer(ClientServer clientServer) {
        this.clientServers.remove(clientServer);
        logger.info("Removed ClientServer");
    }
}
