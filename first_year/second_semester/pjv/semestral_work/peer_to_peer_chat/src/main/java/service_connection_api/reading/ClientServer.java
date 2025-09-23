package service_connection_api.reading;

import com.fasterxml.jackson.core.JsonProcessingException;
import javafx.util.Pair;
import models.data_classes.message.Message;
import models.data_classes.message.generic_messages.ResponseMessage;
import models.enums.connection.MessageType;
import models.enums.message_status.MessageStatus;
import models.events.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api.peer_manager.Peer;
import service_connection_api.peer_manager.PeerManager;
import service_event_handler.EventHandler;
import utils.json.JsonUtils;
import utils.message.MessageUtils;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.Date;

public class ClientServer implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(ClientServer.class);

    private final String userName;
    private final Socket socket;
    private final Thread thread;
    private final String localPeerId;
    private final ObjectInputStream objectInputStream;
    private final ObjectOutputStream objectOutputStream;
    private final MessageServer messageServer;
    private final EventHandler eventHandler;

    public ClientServer(MessageServer messageServer, Socket clientSocket, String peerId, String userName, EventHandler eventHandler) {
        this.messageServer = messageServer;
        this.socket = clientSocket;
        this.localPeerId = peerId;
        this.userName = userName;
        this.eventHandler = eventHandler;
        try {
            this.objectInputStream = new ObjectInputStream(clientSocket.getInputStream());
            this.objectOutputStream = new ObjectOutputStream(clientSocket.getOutputStream());
            this.objectOutputStream.flush();
        } catch (IOException e) {
            logger.error("Failed to initialize streams for client", e);
            throw new RuntimeException(e);
        }
        thread = new Thread(this, "ClientServer");
    }

    @Override
    public void run() {
        logger.info("ClientServer started");
        String receivedJson = receiveMessage();

        Message receivedMessage;
        try {
            receivedMessage = JsonUtils.fromJson(receivedJson, Message.class);
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }

        boolean isGood = dealWithMessage(receivedMessage);
        MessageStatus status = (isGood) ? MessageStatus.OK : MessageStatus.FAIL;

        sendResponse(status);
        closeSocket(socket);

        this.messageServer.removeClientServer(this);
        logger.info("ClientServer finished");
    }

    private String receiveMessage() {
        try {
            return (String) this.objectInputStream.readObject();
        } catch (IOException | ClassNotFoundException e) {
            logger.error("Failed to receive message from peer", e);
            throw new RuntimeException(e);
        }
    }

    private void sendResponse(MessageStatus status) {
        Message resp = new ResponseMessage(localPeerId, userName, status).getMessage();
        String responseJson = JsonUtils.toJson(resp);
        try {
            this.objectOutputStream.writeObject(responseJson);
            this.objectOutputStream.flush();
            logger.info("Sent response to peer: {}", status);
        } catch (IOException e) {
            logger.error("Failed to send response to peer ", e);
            throw new RuntimeException(e);
        }
    }

    private void closeSocket(Socket socket) {
        try {
            socket.close();
        } catch (IOException e) {
            logger.error("Error while closing socket", e);
            throw new RuntimeException(e);
        }
    }

    private boolean dealWithMessage(Message message) {
        switch (message.getType()) {
            case MessageType.CONNECTION_REQUEST -> {
                return handleConnectionRequest(message);
            }
            case MessageType.MESSAGE_PUBLIC -> {
                GetConnectedPeerEvent getConnectedPeerEvent = new GetConnectedPeerEvent();
                getConnectedPeerEvent.setInput(message.getPeerId());
                eventHandler.handleEvent(getConnectedPeerEvent);
                logger.info("Received a message with type {} | TXT {} | FROM {}", message.getType(), message.getContent(), getConnectedPeerEvent.getOutput().getUserName());
                AddMessageToHistoryEvent event = new AddMessageToHistoryEvent();
                GetLocalPeerEvent localPeerEvent = new GetLocalPeerEvent();
                eventHandler.handleEvent(localPeerEvent);
                event.setInput(new Pair<>(getConnectedPeerEvent.getOutput(), message));
                eventHandler.handleEvent(event);
                return true;
            }
            case MessageType.BYE -> {
                RemovePeerEvent removePeerEvent = new RemovePeerEvent();
                removePeerEvent.setInput(message.getPeerId());
                eventHandler.handleEvent(removePeerEvent);
                return true;
            }
            default -> {
                logger.warn("Unknown message type '{}'", message.getType());
                return false;
            }
        }
    }

    private boolean handleConnectionRequest(Message message) {
        int port;
        try {
            port = MessageUtils.getPortFromJson(message);
        } catch (RuntimeException e) {
            // TODO: log
            return false;
        } catch (JsonProcessingException e) {
            // TODO: log
            throw new RuntimeException(e);
        }
        Peer tempPeer = new Peer(message.getPeerId(), message.getSenderName(), socket.getInetAddress(), port);
        IsPeerConnectedEvent peerConnectedEvent = new IsPeerConnectedEvent();
        peerConnectedEvent.setInput(tempPeer);
        eventHandler.handleEvent(peerConnectedEvent);
        if (peerConnectedEvent.getOutput()) {
            GetConnectedPeerEvent getPeer = new GetConnectedPeerEvent();
            getPeer.setInput(message.getPeerId());
            eventHandler.handleEvent(getPeer);
            tempPeer = getPeer.getOutput();
            tempPeer.setLastCheckTime(new Date());
        }

        ConnectionRequestEvent event = new ConnectionRequestEvent();
        event.setInput(tempPeer);
        EventHandler.getInstance().handleEvent(event);
        boolean result = false;
        result = (event.getOutput() != null) ? event.getOutput() : false;
        return result;
    }

    public void start() {
        thread.start();
    }

    public void stop() {
        this.thread.interrupt();
    }

    public void join() {
        try {
            this.thread.join();
        } catch (InterruptedException e) {
            logger.error("Join interrupted for ClientServer thread of peer", e);
            throw new RuntimeException(e);
        }
    }
}
