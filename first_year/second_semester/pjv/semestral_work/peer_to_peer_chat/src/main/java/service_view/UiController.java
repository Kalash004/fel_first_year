package service_view;


import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.util.Pair;
import models.data_classes.message.Message;
import models.enums.event.EventType;
import models.enums.message_history_type.MessageHistoryType;
import models.enums.tictactoe.TictactoeGameStatus;
import models.enums.ui_link_utils.Page;
import models.events.GetMessageHistoryEvent;
import models.events.GetConnectedPeerListEvent;
import models.events.StopProgramEvent;
import models.interfaces.ICommand;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api.peer_manager.Peer;
import service_event_handler.EventHandler;
import models.events.Event;
import models.interfaces.IListen;
import service_tictactoe.TictactoeGame;
import service_tictactoe.TictactoeStatusReports;
import service_view.commands.CommandAddMessage;
import service_view.commands.CommandAddPeer;
import service_view.commands.CommandAddPrivateMessage;
import service_view.commands.CommandRemovePeer;
import service_view.controllers.*;
import service_view.ui_link_utils.CSSLinker;
import service_view.ui_link_utils.FXMLLinker;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

/**
 * Middle ware endpoint,
 * possible use for minigames such as tic tac toe in chat
 *
 * @author Anton
 * @version 0.1
 */
public class UiController extends Application implements IListen {
    private static final Logger logger = LoggerFactory.getLogger(UiController.class);
    private final HashMap<EventType, ICommand> commandsRegistry;
    private final FXMLLoader fxmlLoader;
    private static UiController instance;
    private static final EventHandler eventHandler = EventHandler.getInstance();
    private MainMenuController mainMenuController;
    private TictactoeController tictactoeController;
    private PublicChatController publicChatController;
    private PrivateChatController privateChatController;
    private MainOptionsController mainOptionsController;
    private String privateChatLabel;
    private Page currentPage;
    private Page previousPage;
    private boolean darkMode = true;

    private Peer[] peerList;
    private HashMap<Peer, ArrayList<Message>> messageHistory;
    private HashMap<Peer, ArrayList<Message>> privateMessageHistory;
    private TictactoeGame game;
    private int isPlayingStatus;

    public UiController() {
        this.commandsRegistry = new HashMap<EventType, ICommand>(Map.of(
                EventType.ADD_PEER, new CommandAddPeer(this),
                EventType.ADD_MESSAGE_TO_HISTORY, new CommandAddMessage(this),
                EventType.HANDLE_PUBLIC_MESSAGE, new CommandAddMessage(this),
                EventType.ADD_PRIVATE_MESSAGE_TO_HISTORY, new CommandAddPrivateMessage(this),
                EventType.REMOVE_PEER, new CommandRemovePeer(this)
        ));
        this.peerList = new Peer[]{};
        this.privateChatLabel = "";
        this.fxmlLoader = new FXMLLoader(UiController.class.getResource(FXMLLinker.PAGE_LINKS.get(Page.MAIN_MENU)));
    }

    @Override
    public void start(Stage stage) throws IOException {
        openPage(Page.MAIN_MENU, stage);
        stage.setOnCloseRequest(event -> {
            StopProgramEvent stopProgramEvent = new StopProgramEvent();
            EventHandler.getInstance().handleEvent(stopProgramEvent);
        });
        /*Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Hello!");
        scene.getStylesheets().add(getClass().getResource(CSSLinker.PAGE_LINKS.get(Page.MAIN_MENU)).toExternalForm());
        stage.setScene(scene);
        stage.show();*/
    }

    public void launchUi() {
        Application.launch();
    }

    public static UiController getInstance() {
        // Block non main calls
//        String allowedCaller = "_main.Application"; // fully-qualified class name
//
//        StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
//        if (stackTrace.length > 2) {
//            String callerClassName = stackTrace[2].getClassName();
//            if (!callerClassName.equals(allowedCaller)) {
//                throw new SecurityException("Unauthorized caller: " + callerClassName + " PLEASE USE EVENTS OR START FROM " + allowedCaller);
//            }
//        }
        // end

        if (instance == null) {
            instance = new UiController();
        }
        //instance.peerList = PeerManager.getInstance().getConnectedPeers(); // TODO: use events
        return instance;
    }

    protected void setController(Page page, FXMLLoader fxmlLoader) {
        switch (page) {
            case MAIN_MENU: {
                mainMenuController = fxmlLoader.getController();
                break;
            }
            case TICTACTOE: {
                tictactoeController = fxmlLoader.getController();
                break;
            }
            case PUBLIC_CHAT: {
                publicChatController = fxmlLoader.getController();
                break;
            }
            case PRIVATE_CHAT: {
                privateChatController = fxmlLoader.getController();
                break;
            }
            case OPTIONS: {
                mainOptionsController = fxmlLoader.getController();
                break;
            }
        }
    }

    private Peer[] getPeerList() {
        GetConnectedPeerListEvent getPeerListEvent = new GetConnectedPeerListEvent();
        EventHandler.getInstance().handleEvent(getPeerListEvent);
        Peer[] peerList = getPeerListEvent.getOutput();
        logger.info(peerList.length + " peers found when loading peer list to UI");
        return peerList;
    }

    private HashMap<Peer, ArrayList<Message>> getMessageHistory(boolean isPrivate) {
        GetMessageHistoryEvent getMessageHistoryEvent = new GetMessageHistoryEvent();
        MessageHistoryType messageHistoryType = MessageHistoryType.PUBLIC;
        if (isPrivate) {
            messageHistoryType = MessageHistoryType.PRIVATE;
        }
        getMessageHistoryEvent.setInput(messageHistoryType);
        EventHandler.getInstance().handleEvent(getMessageHistoryEvent);
        HashMap<Peer, ArrayList<Message>> messageHistory = getMessageHistoryEvent.getOutput();
        logger.info(messageHistory.size() + " messages found when loading message history to UI");
        if (messageHistory.size() != 0) {
            for (Peer peer : messageHistory.keySet()) {
                ArrayList<Message> messages = messageHistory.get(peer);
                logger.info(messages.size() + " found when loading message history to UI");
            }
        }
        return messageHistory;
    }

    private ArrayList<Pair<Peer, Message>> getSortedMessageHistory() {
        ArrayList<Pair<Peer, Message>> sortedMessageHistory = new ArrayList<>();
        for (HashMap.Entry<Peer, ArrayList<Message>> peerMessageEntry : messageHistory.entrySet()) {
            for (Message message : peerMessageEntry.getValue()) {
                sortedMessageHistory.add(new Pair(peerMessageEntry.getKey(), message));
            }
        }
        sortedMessageHistory.sort(Comparator.comparing(pair -> pair.getValue().getTimestamp()));
        return sortedMessageHistory;
    }


    private void setControllerSettings(Page page) {
        switch (page) {
            case MAIN_MENU: {
                mainMenuController.setDarkMode(darkMode);
                break;
            }
            case TICTACTOE: {
                tictactoeController.setDarkMode(darkMode);
                break;
            }
            case PUBLIC_CHAT: {
                publicChatController.setDarkMode(darkMode);
                this.peerList = getPeerList();
                publicChatController.setNewUserList(peerList);
                this.messageHistory = getMessageHistory(false);
                publicChatController.setNewMessageHistory(getSortedMessageHistory());
                break;
            }
            case PRIVATE_CHAT: {
                privateChatController.setDarkMode(darkMode);
                this.peerList = getPeerList();
                privateChatController.setNewUserList(peerList);
                privateChatController.setLabel(privateChatLabel);
                this.messageHistory = getMessageHistory(true);
                publicChatController.setNewMessageHistory(getSortedMessageHistory());
                break;
            }
            case OPTIONS: {
                mainOptionsController.setDarkMode(darkMode);
                break;
            }
        }
    }

    public void onStartTTTGame(boolean isFirst) {
        Stage stage = new Stage();
        openPage(Page.TICTACTOE, stage);
    }

    public void setGameStatus(TictactoeGameStatus gameStatus) {
        tictactoeController.sendBoardStatus(TictactoeStatusReports.STATUS_REPORTS.get(gameStatus));
    }

    public void setReceiveBoard(String buttonName, String sign) {
        tictactoeController.onReceiveBoard(buttonName, sign);
    }

    public int getTTTStatus() {
        return isPlayingStatus;
    }

    public void openPage(Page page, Stage stage) {
        //stage.close();
        if (page == null) {
            page = previousPage;
        }
        previousPage = currentPage;
        currentPage = page;
        FXMLLoader fxmlLoader = new FXMLLoader(MainMenuController.class.getResource(FXMLLinker.PAGE_LINKS.get(page)));
        Scene scene = null;

        try {
            scene = new Scene(fxmlLoader.load());
        } catch (IOException e) {
//            logger.log();
            throw new RuntimeException(e);
        }
        setController(page, fxmlLoader);
        scene.getStylesheets().add(getClass().getResource(CSSLinker.PAGE_LINKS.get(page)).toExternalForm());
        setControllerSettings(page);
        stage.setScene(scene);
        stage.show();

    }

    public void setDarkMode() {
        darkMode = !darkMode;
        System.out.println(darkMode);
        mainOptionsController.setDarkMode(darkMode);
    }


    @Override
    public void handleEvent(Event<?, ?> event) {
        EventType type = event.getType();
        if (this.commandsRegistry.containsKey(type)) {
            ICommand command = this.commandsRegistry.get(type);
            command.execute(event);
        }
    }

    public void addPeer(Peer peer) {
        logger.info("New peer added to UI controller");
        this.peerList = getPeerList();
        if (publicChatController != null) {
            publicChatController.setNewUserList(peerList);
        } else if (privateChatController != null) {
            privateChatController.setNewUserList(peerList);
        }
    }

    public void removePeer() {
        logger.info("New peer removed from UI controller");
        this.peerList = getPeerList();
        if (publicChatController != null) {
            publicChatController.setNewUserList(peerList);
        } else if (privateChatController != null) {
            privateChatController.setNewUserList(peerList);
        }
    }

    public void setLabel(String selectedUser) {
        this.privateChatLabel = selectedUser;
    }

    public void addMessage(Pair<Peer, Message> peerMessagePair) {
        logger.info("New message added to public chat UI");
        if (publicChatController != null) {
            publicChatController.addMessage(peerMessagePair.getValue(), peerMessagePair.getKey());
        }
    }

    public void addPrivateMessage(Pair<Peer, Message> peerMessagePair) {
        logger.info("New message added to private chat UI");
        if (privateChatController != null) {
            privateChatController.addMessage(peerMessagePair.getValue(), peerMessagePair.getKey());
        }
    }

    public void onReceivedTTTInvite(Peer peer) {
        if(isPlayingStatus == -1) {
            isPlayingStatus = 0;
        } else {
            // sendUnavailableTTT
        }
    }

}