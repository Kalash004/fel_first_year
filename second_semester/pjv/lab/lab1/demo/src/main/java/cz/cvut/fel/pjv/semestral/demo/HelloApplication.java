package cz.cvut.fel.pjv.semestral.demo;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

public class HelloApplication extends Application {
    private final Image bgimage = new Image("background.png");
    private final Image birdimage = new Image("bird0.png");
    private final Canvas canvas = new Canvas(bgimage.getWidth(), bgimage.getHeight());

    @Override
    public void start(Stage stage) {
        stage.setMaxHeight(canvas.getHeight());
        stage.setMaxWidth(canvas.getWidth());
        stage.setMinHeight(canvas.getHeight());
        stage.setMinWidth(canvas.getWidth());

        GraphicsContext gc = canvas.getGraphicsContext2D();
        drawBackground(gc);
        drawObject(gc);

        StackPane root = new StackPane(canvas);
        Scene scene = new Scene(root,bgimage.getWidth(), bgimage.getHeight());
        stage.setScene(scene);
        stage.setTitle("Skibidi sigma michal");
        stage.show();
    }

    private void drawBackground(GraphicsContext gc) {
        gc.drawImage(bgimage, 0, 0);
    }

    private void drawObject(GraphicsContext gc) {
        gc.drawImage(birdimage, 100, 500);
    }

    public static void main(String[] args) {
        launch();
    }
}