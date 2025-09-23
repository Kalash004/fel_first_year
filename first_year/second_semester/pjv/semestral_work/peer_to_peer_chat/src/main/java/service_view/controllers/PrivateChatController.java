package service_view.controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import models.data_classes.message.Message;
import models.data_classes.message.user_messages.PrivateMessage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_connection_api.discovery.DiscoveryClient;
import models.enums.ui_link_utils.Page;
import service_view.UiController;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

public class PrivateChatController extends PublicChatController {
    @FXML private AnchorPane rootPane;
    @FXML private Label chatLabel;
    @FXML private Button ticButton;
    @FXML private TextArea messageArea;
    @FXML private Button sendButton;
    @FXML private ListView<String> userList;
    @FXML private ListView<String> messageHistory;
    private static final Logger logger = LoggerFactory.getLogger(DiscoveryClient.class);
    private String currentUser;
    private final HashMap<String, ArrayList<String>> userMap = new HashMap<>();
    public void initialize() {
        chatLabel.setText("Chat with " + currentUser);
    }

    private void sendMessage(String content) {
        // TODO: messageSendEvent = new SendPublicMessageEvent();
        //messageSendEvent.setInput(content);
        //EventHandler.getInstance().handleEvent(messageSendEvent);
    }

    private void receiveMessage(Message message) {
        /* receive message event */
        messageHistory.getItems().add(message.getPeerId() + " (" + message.getTimestamp() + "): " + message.getContent());
    }


    @FXML
    private void onUserListClick(MouseEvent event) throws IOException {
        String selected = userList.getSelectionModel().getSelectedItem();
        if(selected == null) { // if an empty list element was selected
            return;
        }

        setLabel(selected);
        //messageHistory.setItems(FXCollections.observableArrayList(userMap.get(selected)));
        /*
        send to backend, show the chat history with a selected active user
         */
    }

    @FXML
    private void onSendButtonClick(ActionEvent event) {
        String content = messageArea.getText();
        messageArea.clear();
        //messageHistory.getItems().add("You (" + new Date() + "): " + content);
        sendMessage(content);

    }

    @FXML
    private void onPublicChatButtonClick(ActionEvent event) {
        Stage stage = (Stage) sendButton.getScene().getWindow();
        UiController.getInstance().openPage(Page.PUBLIC_CHAT, stage);
    }

    @FXML
    private void onOptionsButtonClick(ActionEvent event) {
        Stage stage = (Stage) sendButton.getScene().getWindow();
        UiController.getInstance().openPage(Page.OPTIONS, stage);
    }

    @FXML
    private void onTicButtonClick(ActionEvent actionEvent) throws IOException {
        int TTTStatus = UiController.getInstance().getTTTStatus();
        if(TTTStatus == -1) {
            sendMessage("This player has invited you to play tictactoe. Click on PLAY TICTACTOE button to start the game!");
        } else if(TTTStatus == 0) {
            Stage stage = (Stage) sendButton.getScene().getWindow();
            UiController.getInstance().openPage(Page.TICTACTOE, stage);
        }
    }

    public void setLabel(String currentUser) {
        chatLabel.setText("Chat with " + currentUser);
    }
}
