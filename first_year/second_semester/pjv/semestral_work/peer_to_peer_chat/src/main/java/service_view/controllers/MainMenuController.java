package service_view.controllers;

import configs.ApiConfig;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import models.enums.ui_link_utils.Page;
import models.events.SendHelloEvent;
import models.events.StartEvent;
import service_event_handler.EventHandler;
import service_view.UiController;
import service_view.format_utils.RegexUtils;

public class MainMenuController {
    @FXML private AnchorPane rootPane;
    @FXML private TextField usernameField;

    @FXML
    public void initialize() {
    }


    public void setDarkMode(boolean darkMode) {
        rootPane.getStyleClass().removeAll("light-mode", "dark-mode");
        if(darkMode) {
            rootPane.getStyleClass().add("dark-mode");
        } else {
            rootPane.getStyleClass().add("light-mode");
        }
    }

    private void sendHelloMessage(String userName) {
        ApiConfig.setUserName(userName);
        StartEvent startEvent = new StartEvent();
        EventHandler.getInstance().handleEvent(startEvent);
        SendHelloEvent sendHelloEvent = new SendHelloEvent();
        sendHelloEvent.setInput(userName);
        EventHandler.getInstance().handleEvent(sendHelloEvent);
    }

    @FXML
    private void onConnectButtonClick() {
        if(usernameField.getText().isEmpty() || !RegexUtils.isAlphanumeric(usernameField.getText())) {
            usernameField.clear();
            usernameField.setPromptText("Username is not valid!");
        } else {
            sendHelloMessage(usernameField.getText());
            Stage stage = (Stage) rootPane.getScene().getWindow();
            UiController.getInstance().openPage(Page.PUBLIC_CHAT, stage);
            /*
            send to backend, client discovery & connection
             */
        }
    }

    @FXML
    private void onOptionsButtonClick(ActionEvent actionEvent) {
        Stage stage = (Stage) rootPane.getScene().getWindow();
        UiController.getInstance().openPage(Page.OPTIONS, stage);
    }
}
