package view.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.CheckBox;
import javafx.scene.layout.BorderPane;

public class MainOptionsController {
    @FXML protected CheckBox darkThemeSwitch;

    @FXML
    public void setSwitch() {
        darkThemeSwitch.setOnAction(event -> {
            if (darkThemeSwitch.isSelected()) {
                //...getStyleClass().removeAll("light-mode");
                //...getStyleClass().add("dark-mode");

            } else {
                //...getStyleClass().removeAll("dark-mode");
                //...getStyleClass().add("light-mode");

            }
        });
    }

}
