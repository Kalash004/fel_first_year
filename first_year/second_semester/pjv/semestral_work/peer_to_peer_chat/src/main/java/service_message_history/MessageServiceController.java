package service_message_history;

import models.events.Event;
import models.enums.event.EventType;
import models.events.GetConnectedPeerEvent;
import models.events.HandlePublicMessageEvent;
import models.interfaces.ICommand;
import models.interfaces.IListen;
import models.data_classes.message.Message;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api.peer_manager.Peer;
import service_connection_api.peer_manager.PeerManager;
import service_event_handler.EventHandler;
import service_message_history.commands.CommandAddMessage;
import service_message_history.commands.CommandAddPrivateMessage;
import service_message_history.commands.CommandGetMessageHistory;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class MessageServiceController implements IListen {
    private static final Logger logger = LoggerFactory.getLogger(MessageServiceController.class);
    private static MessageServiceController instance;
    private final HashMap<Peer, ArrayList<Message>> messageHistory;
    private final HashMap<Peer, ArrayList<Message>> privateMessageHistory;
    private final Map<EventType, ICommand> commandsRegistry;
    //private final PeerManager peerManager; //TODO: is it necessary?

    private MessageServiceController() {
        //peerManager = PeerManager.getInstance();
        messageHistory = new HashMap<Peer, ArrayList<Message>>();
        privateMessageHistory = new HashMap<Peer, ArrayList<Message>>();
        commandsRegistry = new HashMap<EventType, ICommand>(
                Map.of(
                        EventType.ADD_MESSAGE_TO_HISTORY, new CommandAddMessage(this),
                        EventType.ADD_PRIVATE_MESSAGE_TO_HISTORY, new CommandAddPrivateMessage(this),
                        EventType.GET_MESSAGE_HISTORY, new CommandGetMessageHistory(this)
//                        EventType.HANDLE_PUBLIC_MESSAGE, new ICommand() {
//                            @Override
//                            public void execute(Event<?, ?> input) {
//                                if (!(input instanceof HandlePublicMessageEvent)) {
//                                    return;
//                                }
//                                HandlePublicMessageEvent event = (HandlePublicMessageEvent) input;
//                                GetConnectedPeerEvent getPeer = new GetConnectedPeerEvent();
//                                getPeer.setInput(event.getInput().getPeerId());
//                                EventHandler.getInstance().handleEvent(getPeer);
//                                addMessage(getPeer.getOutput(), event.getInput());
//                            }
//                        }
                ));
    }

    public static MessageServiceController getInstance() {
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
            instance = new MessageServiceController();
        }
        return instance;
    }

    public void addMessage(Peer peer, Message message) {
        if (peer == null || message == null) {
            return; // TODO: log
        }
        logger.info("Adding new message from peer '{}'", peer.getPeerId());
        if (!messageHistory.containsKey(peer)) {
            messageHistory.put(peer, new ArrayList<Message>());
        }
        messageHistory.get(peer).add(message);
    }

    public void addPrivateMessage(Peer peer, Message message) {
        if (peer == null || message == null) {
            return; // TODO: log
        }
        logger.info("Adding new private message from peer '{}'", peer.getPeerId());
        if (!privateMessageHistory.containsKey(peer)) {
            privateMessageHistory.put(peer, new ArrayList<Message>());
        }
        privateMessageHistory.get(peer).add(message);
    }

    public HashMap<Peer, ArrayList<Message>> getMessageHistory() {
        return messageHistory;
    }

    public HashMap<Peer, ArrayList<Message>> getPrivateMessageHistory() {
        return privateMessageHistory;
    }

    @Override
    public void handleEvent(Event<?, ?> event) {
        EventType type = event.getType();
        if (this.commandsRegistry.containsKey(type)) {
            ICommand command = this.commandsRegistry.get(type);
            command.execute(event);
        }
    }


}
