package service_connection_api;

import configs.ApiConfig;
import javafx.util.Pair;
import models.data_classes.message.Message;
import models.data_classes.message.generic_messages.ByeMessage;
import models.data_classes.message.generic_messages.ConAckMessage;
import models.data_classes.message.generic_messages.ConReqMessage;
import models.data_classes.message.user_messages.PublicMessage;
import models.enums.event.EventType;
import models.enums.message_history_type.MessageHistoryType;
import models.events.*;
import models.interfaces.ICommand;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api._factory.ConnectionApiFactory;
import service_connection_api.commands.*;
import service_connection_api.discovery.DiscoveryController;
import service_connection_api.peer_manager.Peer;
import service_connection_api.reading.MessageServer;
import service_connection_api.sending.MessageClient;
import service_event_handler.EventHandler;
import models.interfaces.IListen;
import service_message_history.MessageServiceController;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class ApiController implements IListen {
    private static final Logger logger = LoggerFactory.getLogger(ApiController.class);

    private static ApiController instance = null;

    private DiscoveryController discoveryController;
    private MessageClient messageClient;
    private MessageServer messageServer;

    private EventHandler eventHandler;
    private final HashMap<EventType, ICommand> commandsRegistry;
    private String localPeerId;
    private int messagingPort;
    private String userName;
    private MessageServiceController messageServiceController;

    public static ApiController getInstance() {
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
            instance = new ApiController();
        }
        return instance;
    }

    private ApiController() {
        // TODO: maybe return

        this.commandsRegistry = new HashMap<EventType, ICommand>(Map.of(
                EventType.PROGRAM_START, new CommandStartApi(this),
                EventType.SEND_CONACK, new CommandSendConAck(this),
                EventType.SEND_CONREQ, new CommandSendConReq(this),
                EventType.SEND_HELLO, new CommandSendHello(this),
                EventType.SEND_PUBLIC_MESSAGE, new CommandSendPublicMessage(this),
                EventType.PROGRAM_END, new CommandProgramEnd(this),
                EventType.CONNECTION_REQUESTED, new ICommand() {
                    @Override
                    public void execute(Event<?, ?> input) {
                        if (!(input instanceof ConnectionRequestEvent connectionRequestEvent)) {
                            return;
                        }
                        Boolean res = sendConAck(connectionRequestEvent.getInput());
                        connectionRequestEvent.setOutput(res);
                    }
                }
        ));
    }

    public void launch() {
        //TODO:
        this.eventHandler = EventHandler.getInstance();
        this.messageClient = ConnectionApiFactory.getMessagingClient();
        this.userName = ApiConfig.getUserName();
        this.messagingPort = ApiConfig.getMessagingPort();
        this.discoveryController = DiscoveryController.getInstance();
        this.messageServer = MessageServer.getInstance();
        GetLocalPeerIDEvent getLocalPeerIDEvent = new GetLocalPeerIDEvent();
        eventHandler.handleEvent(getLocalPeerIDEvent);
        this.localPeerId = getLocalPeerIDEvent.getOutput();
        this.start();
    }

    @Override
    public void handleEvent(Event<?, ?> event) {
        EventType type = event.getType();
        if (this.commandsRegistry.containsKey(type)) {
            ICommand command = this.commandsRegistry.get(type);
            command.execute(event);
        }
    }

    public void sendBye() {
        logger.info("Sending bye");
        String peerId = this.localPeerId;
        ByeMessage byeMessage = new ByeMessage(peerId);
        this.messageClient.sendMessageToAllPeers(byeMessage.getMessage());
    }

    public void sendHello(String userName) {
        ApiConfig.setUserName(userName);
        logger.info("Username set to {}", userName);
        this.discoveryController = DiscoveryController.getInstance();
    }

    public boolean sendConAck(Peer peer) {
        GetMessageHistoryEvent getMessageHistoryEvent = new GetMessageHistoryEvent();
        getMessageHistoryEvent.setInput(MessageHistoryType.PUBLIC);
        eventHandler.handleEvent(getMessageHistoryEvent);
        HashMap<Peer, ArrayList<Message>> message_history = getMessageHistoryEvent.getOutput();
        Peer[] connected_peers;
        GetConnectedPeerListEvent event = new GetConnectedPeerListEvent();
        eventHandler.handleEvent(event);
        ConAckMessage message = new ConAckMessage(this.localPeerId, message_history, event.getOutput());
        logger.info("Sending connection acknowledgement to peer: {} | TXT: {}", peer, message.getMessage().toString());
        return this.messageClient.sendMessage(message.getMessage(), peer);
    }

    public void sendPublicMessage(String content) {
        PublicMessage message = new PublicMessage(this.localPeerId, content);
        this.messageClient.sendMessageToAllPeers(message.getMessage());
        logger.info("Message sent to all peers");

        GetLocalPeerEvent getLocalPeerEvent = new GetLocalPeerEvent();
        eventHandler.handleEvent(getLocalPeerEvent);
        Peer thisPeer = getLocalPeerEvent.getOutput();
        AddMessageToHistoryEvent addMessageToHistoryEvent = new AddMessageToHistoryEvent();
        addMessageToHistoryEvent.setInput(new Pair<>(thisPeer, message.getMessage()));
        eventHandler.handleEvent(addMessageToHistoryEvent);
    }

    public void sendConReq(Peer peer) {
        ConReqMessage message = new ConReqMessage(this.localPeerId, this.messagingPort, this.userName);
        this.messageClient.sendMessage(message.getMessage(), peer);
    }

    public void sendMessage(Message message, Peer peer) {
        this.messageClient.sendMessage(message, peer);
    }

    public void stop() {
        endDiscovery();
        endMessaging();
    }

    private void endDiscovery() {
        if (discoveryController == null) {
            return;
        }
        this.discoveryController.stop();
    }

    private void endMessaging() {
        if (messageServer == null) {
            logger.warn("[endMessaging()] No message server available to stop");
            return;
        }
        messageServer.stop();
    }

    public void start() {
        this.discoveryController.startDiscovery();
        this.messageServer.start();
    }
}
