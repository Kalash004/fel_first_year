package service_connection_api.discovery;

import configs.ApiConfig;
import models.data_classes.message.generic_messages.DiscoveryHelloMessage;
import models.interfaces.ICorutine;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_coroutine.CoroutineService;
import utils.json.JsonUtils;

import java.net.*;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Date;

/**
 * Sends discovery requests {@message_type <strong>hello</strong>}
 */
public class DiscoveryClient implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(DiscoveryClient.class);
    private final int messagingPort;

    private final Thread thread;

    private final int localPort;
    private final int discoveryPortTarget;
    private final int defaultPacketBufferSize;
    private final String peerId;
    private final int timeoutMs;
    private boolean runFlag;
    private final int socketSleepMs;
    private DatagramSocket testSocket = null;
    private final InetAddress broadcastAddress;
    private final String userName;
    private DatagramSocket clientSocket;


    private Socket discoverySocket;

    public DiscoveryClient(int localReaderPort, int defaultPacketBufferSize, String localPeerId, int timeoutMs, int socketSleepMs, String ipAddress, int localPort, int messagingPort, String userName) {
        try {
            this.broadcastAddress = InetAddress.getByName(ipAddress);
        } catch (UnknownHostException e) {
            // TODO: add log ?
            throw new RuntimeException("Invalid ip address", e);
        }
        this.discoveryPortTarget = localReaderPort;
        this.defaultPacketBufferSize = defaultPacketBufferSize;
        this.peerId = localPeerId;
        this.timeoutMs = timeoutMs;
        this.socketSleepMs = socketSleepMs;
        this.thread = new Thread(this, "DiscoveryClient");
        this.runFlag = true;
        this.localPort = localPort;
        this.messagingPort = messagingPort;
        this.userName = userName;
    }

    @Override
    public void run() {
        logger.info("running DiscoveryClient");

        try (DatagramSocket temp = new DatagramSocket(this.localPort)) {
            clientSocket = temp;
            clientSocket.setBroadcast(true); // Enable broadcast
            clientSocket.setSoTimeout(this.timeoutMs); // Set up timeout for recieve ...
            // ... wait for recieve for x amount of milisecs, then throw error - good for hangups

            logger.info("Started sending discovery packets on port: {}", this.localPort);

            while (runFlag) {
                String request;
                try {
                    request = JsonUtils.toJson(new DiscoveryHelloMessage(this.peerId, this.messagingPort, this.userName).getMessage());
                    logger.debug("Discovery request message: {}", request);
                } catch (Exception e) {
                    logger.error("Failed to create discovery message", e);
                    throw new RuntimeException(e);
                }

                byte[] sendBuffer = request.getBytes();
                if (sendBuffer.length > this.defaultPacketBufferSize) {
                    logger.warn("Buffer size exceeds the default packet buffer size: {} bytes", sendBuffer.length);
                    throw new RuntimeException("Buffer size exceeds the default packet buffer size");
                }

                DatagramPacket sendPacket = new DatagramPacket(sendBuffer, sendBuffer.length, this.broadcastAddress, this.discoveryPortTarget);
                clientSocket.send(sendPacket);

                logger.info("Sent {} bytes to {}:{}", sendPacket.getLength(), broadcastAddress.getHostAddress(), this.discoveryPortTarget);

                try {
                    Thread.sleep(socketSleepMs);
                } catch (InterruptedException e) {
                    logger.warn("Sleep interrupted, continuing with next iteration");
                    Thread.currentThread().interrupt(); // Restore interrupted status
                }
            }
            logger.info("Stopped discovery client");
        } catch (Exception e) {
            logger.error("Error in DiscoveryClient run", e);
        }
    }

    public void start() {
        logger.info("Starting DiscoveryClient thread...");
        thread.start();
    }

    public void stop() {
        logger.info("Stopping DiscoveryClient thread...");
        runFlag = false;
        CoroutineService.getInstance().addCoroutine(new ICorutine() {
            final Date curTime = new Date();
            final int endDeltaTimeSec = ApiConfig.getDiscoveryKillTime();

            // SetUp coroutine timer
            @Override
            public boolean resume() {
                if (new Date().getTime() >= curTime.getTime() + endDeltaTimeSec * 1000L) {
                    kill();
                    return false;
                }
                return true;
            }
        });
    }

    public void kill() {
        logger.info("Killing DiscoveryClient thread...");
        runFlag = false;
        if(clientSocket == null) {
            logger.error("No client socket available");
            return;
        }
        this.clientSocket.close();
        this.thread.interrupt();
    }

    public Thread getThread() {
        return thread;
    }

    public void setTestSocket(DatagramSocket socket) {
        this.testSocket = socket;
    }
}


