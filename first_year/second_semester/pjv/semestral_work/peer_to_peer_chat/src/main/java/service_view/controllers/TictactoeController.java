package service_view.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;
import javafx.event.ActionEvent;
import service_middle_ware.TictactoeMiddleware;

public class TictactoeController extends MainOptionsController {
    @FXML private Button returnButton;
    @FXML private Label gameWithLabel;
    @FXML private Label statusLabel;
    @FXML private AnchorPane rootPane;
    @FXML private GridPane grid;
    @FXML private Button btn00;
    @FXML private Button btn01;
    @FXML private Button btn02;
    @FXML private Button btn10;
    @FXML private Button btn11;
    @FXML private Button btn12;
    @FXML private Button btn20;
    @FXML private Button btn21;
    @FXML private Button btn22;
    private TictactoeMiddleware middleWare;

    public void initialize() {
    }

    @Override
    public void setDarkMode(boolean darkMode) {
        rootPane.getStyleClass().removeAll("light-mode", "dark-mode");
        if(darkMode) {
            rootPane.getStyleClass().add("dark-mode");
        } else {
            rootPane.getStyleClass().add("light-mode");
        }
    }


    @FXML
    private void onSpaceClick(ActionEvent actionEvent) {
        if(middleWare == null) {
            middleWare = new TictactoeMiddleware();
            middleWare.launchGame(true); // TODO: change hasFT
        }
        Button spaceButton = (Button) actionEvent.getSource();
        spaceButton.setText("X");
        String space = spaceButton.getId();
        System.out.println(space);
        int x = space.charAt(3) - '0';
        int y = space.charAt(4) - '0';
        middleWare.sendBoard(x, y);
    }

    public void sendBoardStatus(String status) {
        statusLabel.setText(status);
    }

    public void onReceiveBoard(String buttonName, String sign) {
        Button spaceButton = (Button) rootPane.lookup("#" + buttonName);
        spaceButton.setText(sign);
        spaceButton.setDisable(true);
    }

    @FXML
    private void onReturnClick(ActionEvent actionEvent) {

    }

}
