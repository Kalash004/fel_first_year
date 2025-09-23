package service_connection_api.sending;

import com.fasterxml.jackson.core.JsonProcessingException;
import models.data_classes.message.Message;
import models.enums.message_status.MessageStatus;
import models.events.Event;
import models.events.GetConnectedPeerListEvent;
import models.interfaces.IListen;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api.discovery.DiscoveryClient;
import service_connection_api.peer_manager.Peer;
import service_connection_api.peer_manager.PeerManager;
import service_event_handler.EventHandler;
import utils.json.JsonUtils;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketTimeoutException;

public class MessageClient {
    private static final Logger logger = LoggerFactory.getLogger(MessageClient.class);
    private final int TimeoutMs;

    public MessageClient(int TimeoutMs) {
        this.TimeoutMs = TimeoutMs;
    }



    public void sendMessageToAllPeers(Message message) {
        // TODO: Possibly too slow - if waiting for every timeout
        GetConnectedPeerListEvent getPeers = new GetConnectedPeerListEvent();
        EventHandler.getInstance().handleEvent(getPeers);
        Peer[] peers = getPeers.getOutput();
        for (Peer peer : peers) {
            this.sendMessage(message, peer);
        }
    }

    /**
     * Opens socket, jsonifies the message, sends bytes,
     *
     * @param message - Message to be sent
     * @param peer    - Peer to send message to
     */
    public boolean sendMessage(Message message, Peer peer) {
        String requestString = JsonUtils.toJson(message);
        // Socket setup
        Socket socket;
        ObjectOutputStream objectOutputStream;
        ObjectInputStream objectInputStream;
        try {
            socket = new Socket(peer.getIpAddress(), peer.getMessagePort());
            objectOutputStream = new ObjectOutputStream(socket.getOutputStream());
            objectOutputStream.flush();
            objectInputStream = new ObjectInputStream(socket.getInputStream());
            socket.setSoTimeout(this.TimeoutMs);
        } catch (IOException e) {
            logger.error("Error during creation of socket in MessageClient {}", e.getMessage());
            return false;
        }
        send(objectOutputStream, requestString);
        getResponse(objectInputStream);
        closeSocket(socket);
        return true;
    }

    private boolean send(ObjectOutputStream outputStream, String requestString) {
        try {
            outputStream.writeObject(requestString);
            outputStream.flush(); // send
        } catch (IOException e) {
            // TODO: log
            return false;
        }
        return true;
    }

    private boolean getResponse(ObjectInputStream inputStream) {
        String response = null;
        try {
            response = inputStream.readObject().toString();
        } catch (IOException | ClassNotFoundException e) {
            // TODO: log
            return false;
        }
        Message responseMessage = null;
        try {
            responseMessage = JsonUtils.fromJson(response, Message.class);
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }
        return (responseMessage.getStatus() == MessageStatus.OK);
    }

    private boolean closeSocket(Socket socket) {
        try {
            socket.close();
        } catch (IOException e) {
            // TODO: log
            return false;
        }
        return true;
    }
}
