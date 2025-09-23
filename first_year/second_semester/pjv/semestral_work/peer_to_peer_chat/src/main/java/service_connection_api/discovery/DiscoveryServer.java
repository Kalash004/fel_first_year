package service_connection_api.discovery;

import com.fasterxml.jackson.core.JsonProcessingException;
import configs.ApiConfig;
import models.data_classes.message.Message;
import models.events.AddPeerEvent;
import models.events.IsPeerConnectedEvent;
import models.events.SendConReqEvent;
import models.interfaces.ICorutine;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api.peer_manager.Peer;
import service_connection_api.peer_manager.PeerManager;
import service_connection_api.sending.MessageClient;
import service_event_handler.EventHandler;
import service_coroutine.CoroutineService;
import utils.ip.IpUtils;
import utils.json.JsonUtils;
import utils.message.MessageUtils;

import java.io.IOException;
import java.net.*;
import java.util.Date;
import java.util.Enumeration;

/**
 * Accepts discovery requests and requests {@link MessageClient} to send
 * {@message_type <strong>conack</strong}
 */
public class DiscoveryServer implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(DiscoveryServer.class);

    private final Thread thread;
    private final int discoveryServerSourcePort;
    private final int defaultPacketSize;
    private boolean runFlag = false;
    private final String localIp;
    private final int discoveryTimeout;
    private DatagramSocket serverSocket;
    private EventHandler eventHandler;

    public DiscoveryServer(int discoveryServerPort, int defaultPacketSize, int discoveryTimeout, int discoveryServerTargetPort) {
        this.discoveryServerSourcePort = discoveryServerPort;
        this.defaultPacketSize = defaultPacketSize;
        this.discoveryTimeout = discoveryTimeout;
        thread = new Thread(this, "DiscoveryServer");
        this.localIp = IpUtils.getLocalIp();
    }

    // TODO: crazy stuff, might break someday

    /**
     * Gets local primary ip address
     *
     * @return String - ip
     */

    @Override
    public void run() {
        try {
            serverSocket = new DatagramSocket(this.discoveryServerSourcePort);
        } catch (SocketException e) {
            logger.error("Error when creating discovery server socket :{}",e.getMessage());
            throw new RuntimeException(e);
        }

        try {
            serverSocket.setSoTimeout(this.discoveryTimeout);
        } catch (SocketException e) {
            throw new RuntimeException(e);
        }

        logger.info("Discovery server started on port {}", this.discoveryServerSourcePort);

        while (this.runFlag) {
            byte[] buffer = new byte[this.defaultPacketSize];
            DatagramPacket request = new DatagramPacket(buffer, buffer.length);
            try {
                serverSocket.receive(request);
            } catch (SocketTimeoutException e) {
                continue;
            } catch (IOException e) {
                logger.error("Error during discovery server request receive " + e.getMessage());
                continue;
            }
            if (request.getAddress().getHostAddress().equals(this.localIp)) {
                continue;
            }
            String message = new String(request.getData());
            message = message.replace("\0", "");
            InetAddress address = request.getAddress();
            String addressString = address.getHostAddress();
            int port = request.getPort();

            Message messageObj;
            try {
                messageObj = JsonUtils.fromJson(message, Message.class);
            } catch (JsonProcessingException e) {
                // TODO: log
                throw new RuntimeException(e);
            }

            int peerMessagePort;

            try {
                peerMessagePort = MessageUtils.getPortFromJson(messageObj);
            } catch (JsonProcessingException e) {
                throw new RuntimeException(e);
            }

            Peer newPeer = new Peer(messageObj.getPeerId(), messageObj.getSenderName(), address, peerMessagePort);

            IsPeerConnectedEvent event = new IsPeerConnectedEvent();
            event.setInput(newPeer);
            eventHandler.handleEvent(event);
            if (event.getOutput()) {
                continue;
            }

            logger.info("Received new broadcast request {} from {}:{}", messageObj.getType(), addressString, port);

            AddPeerEvent addPeerEvent = new AddPeerEvent();
            addPeerEvent.setInput(newPeer);
            EventHandler.getInstance().handleEvent(addPeerEvent);


            SendConReqEvent sendConReqEvent = new SendConReqEvent();
            sendConReqEvent.setInput(newPeer);
            EventHandler.getInstance().handleEvent(sendConReqEvent); // TODO: Send conack message
        }
        serverSocket.close();
        logger.info("Stopped discovery server");
    }

    public void start() {
        this.runFlag = true;
        this.eventHandler = EventHandler.getInstance();
        this.thread.start();
    }

    public void stop() {
        logger.info("Stopping DiscoveryServer thread...");
        this.runFlag = false;
        // setup coroutine to kill the process just in case
        CoroutineService.getInstance().addCoroutine(new ICorutine() {
            final Date curTime = new Date();
            final int endDeltaTimeSec = ApiConfig.getDiscoveryKillTime();

            // SetUp coroutine timer
            @Override
            public boolean resume() {
                if (new Date().getTime() >= curTime.getTime() + endDeltaTimeSec * 1000L) {
                    logger.info("Killing discovery server");
                    kill();
                    return false;
                }
                return true;
            }
        });
    }

    public void kill() {
        logger.info("Killing DiscoveryServer thread...");
        this.runFlag = false;
        this.serverSocket.close();
        this.thread.interrupt();
    }
}