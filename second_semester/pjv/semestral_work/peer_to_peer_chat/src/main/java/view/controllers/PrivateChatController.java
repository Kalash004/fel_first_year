package view.controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.control.TextArea;
import javafx.scene.input.MouseEvent;

public class PrivateChatController extends MainOptionsController {
    @FXML private TextArea message;
    @FXML private Button sendButton;
    @FXML private ListView<String> userList;
    @FXML private ListView<String> messageHistory;

    @FXML
    private void handleListClick(MouseEvent event) {
        String selected = userList.getSelectionModel().getSelectedItem();
        /*
        send to backend, show the chat history with a selected active user
         */
    }

    @FXML
    private void onSendButtonClick(ActionEvent event) {
        messageHistory.getItems().add(message.getText());
        /*
        send to backend, send a private message to a selected user
         */
    }
}
