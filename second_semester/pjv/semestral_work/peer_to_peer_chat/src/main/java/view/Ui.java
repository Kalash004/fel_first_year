package view;

import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.logging.Logger;

public class Ui extends Application {
    private static Ui instance = null;

    private static Logger logger = Logger.getLogger(Ui.class.getName()); // TODO: Implement logger

    private final FXMLLoader fxmlLoader = new FXMLLoader(Ui.class.getResource("hello-view.fxml"));
    private final String title;

    private Ui(String title) {
        this.title = title;
    }

    public static Ui getInstance() {
        if (instance == null) {
            instance = new Ui(UiConfigurator.getInstance().getUiWindowName());
        }
        return instance;
    }

    @Override
    public void start(Stage stage) throws IOException {
        Scene scene = new Scene(fxmlLoader.load(), 320, 240);
        stage.setTitle("Hello!");
        stage.setScene(scene);
        stage.show();
    }

    private void setUp(Stage stage) {
        Scene scene = null;
        try {
            scene = new Scene(fxmlLoader.load(), 320, 240);
        } catch (IOException e) {
            logger.log();
            throw new RuntimeException(e);
        }
        stage.setTitle(title);
        stage.setScene(scene);

    }

    public void launchUi() {
        launch();
    }
}