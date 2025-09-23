package service_view.controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import service_view.UiController;
import service_view.sort_utils.UserSorter;

import java.io.IOException;

public class MainOptionsController {
    @FXML private AnchorPane rootPane;
    @FXML private ChoiceBox sortChoiceBox;
    @FXML private CheckBox darkThemeSwitch;
    private UserSorter userSorter;

    public void initialize() {
        userSorter = new UserSorter();
        //sortChoiceBox.getItems().add(userSorter.getSortOptions());
    }


    public void setDarkMode(boolean darkMode) {
        rootPane.getStyleClass().removeAll("light-mode", "dark-mode");
        if(darkMode) {
            rootPane.getStyleClass().add("dark-mode");
        } else {
            rootPane.getStyleClass().add("light-mode");
        }
    }

    @FXML
    public void onDarkThemeSwitch(ActionEvent event) {
        UiController.getInstance().setDarkMode(); // true or false
    }

    @FXML
    private void onReturnClick(ActionEvent actionEvent) throws IOException {
        // to do -> return to the previously visited page
        Stage stage = (Stage) darkThemeSwitch.getScene().getWindow();
        UiController.getInstance().openPage(null, stage);
    }
}
