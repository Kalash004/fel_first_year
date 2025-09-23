package service_view.controllers;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import javafx.util.Pair;
import models.data_classes.message.Message;
import models.events.SendPublicMessageEvent;
import service_connection_api.peer_manager.Peer;
import service_event_handler.EventHandler;
import models.enums.ui_link_utils.Page;
import service_view.UiController;
import service_view.format_utils.DateUtils;

import java.util.ArrayList;

public class PublicChatController {
    @FXML private Label chatLabel;
    @FXML private AnchorPane rootPane;
    @FXML private TextArea messageArea;
    @FXML private Button sendButton;
    @FXML private ListView<String> userList;
    @FXML private ListView<String> messageHistory;
    public void initialize() {
        //messageHashMap = MessageServiceController.getInstance().
    }


    public void setNewUserList(Peer[] peerList) {
        Platform.runLater(() -> { // NOTE: used when another thread needs to update the JavaFX user interface
            if(peerList.length == 0){
                chatLabel.setText("Messages can't be sent until another user connects");
            } else {
                chatLabel.setText("Public chat");
            }
            userList.getItems().clear();
            for(Peer peer : peerList) {
                userList.getItems().add(peer.getUserName());
            }
        });
    }

    public void setNewMessageHistory(ArrayList<Pair<Peer, Message>> messageList) {
        Platform.runLater(() -> { // NOTE: used when another thread needs to update the JavaFX user interface
            messageHistory.getItems().clear();
            //HashMap<Peer, Message> sortedMessageMap = new HashMap<>();
            for(Pair<Peer, Message> pair : messageList) {
                addMessage(pair.getValue(), pair.getKey());
            }
        });
    }


    private void sendMessage(String content) {
        SendPublicMessageEvent messageSendEvent = new SendPublicMessageEvent();
        messageSendEvent.setInput(content);
        EventHandler.getInstance().handleEvent(messageSendEvent);
    }

    public void setDarkMode(boolean darkMode) {
        styleListView(userList, darkMode);
        styleListView(messageHistory, darkMode);
        rootPane.getStyleClass().removeAll("light-mode", "dark-mode");
        if(darkMode) {
            rootPane.getStyleClass().add("dark-mode");
        } else {
            rootPane.getStyleClass().add("light-mode");
        }
    }

    protected void styleListView(ListView<String> listView, boolean darkMode
    ) {
        listView.setCellFactory(lv -> new ListCell<>() {
            @Override
            protected void updateItem(String item, boolean empty) {
                super.updateItem(item, empty);
                setText(item);
                // Force dark mode colors manually (can be customized)
                if(darkMode) {
                    setStyle("-fx-control-inner-background: #252526; -fx-text-fill: white;");
                } else {
                    setStyle("-fx-control-inner-background: white; -fx-background-color: white; -fx-text-fill: black;");
                }

            }
        });
    }

    @FXML
    private void onUserListClick(MouseEvent event) {
        String selected = userList.getSelectionModel().getSelectedItem();
        if(selected == null) { // if an empty list element was selected
            return;
        }
        Stage stage = (Stage) sendButton.getScene().getWindow();
        UiController.getInstance().setLabel(selected);
        UiController.getInstance().openPage(Page.PRIVATE_CHAT, stage);
        //UiController.getInstance().setChatUser(selected);
        /*
        send to backend, switch FMXL to private chat, show the chat history with a selected active user
         */
    }

    @FXML
    private void onOptionsButtonClick(ActionEvent event) {
        Stage stage = (Stage) sendButton.getScene().getWindow();
        UiController.getInstance().openPage(Page.OPTIONS, stage);
    }

    @FXML
    private void onSendButtonClick(ActionEvent event) {
        String content = messageArea.getText();
        messageArea.clear();
        //messageHistory.getItems().add("You (" + new Date() + "): " + content);
        sendMessage(content);
    }

    public void addMessage(Message message, Peer peer) {
        Platform.runLater(() -> {
            if(peer == null) {
                return;
            }
            messageHistory.getItems().add(peer.getUserName() + " (" + DateUtils.timestampToString(message.getTimestamp()) + "): " + message.getContent());
        });
    }
}
