package cz.cvut.fel.pjv.semestral.demo;

import cz.cvut.fel.pjv.semestral.demo.model.ImageId;
import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

import java.util.HashMap;
import java.util.Map;

public class FlappyBird extends Application {
    Map<ImageId, Image> images = new HashMap<ImageId, Image>();
    private final Canvas canvas = new Canvas();

    @Override
    public void start(Stage stage) {
        loadImages();

        double width = images.get(ImageId.BACKGROUND).getWidth();
        double height = images.get(ImageId.BACKGROUND).getHeight();

        canvas.setWidth(width);
        canvas.setHeight(height);
        GraphicsContext gc = canvas.getGraphicsContext2D();

        AnimationTimer timer = new AnimationTimer() {
            @Override
            public void handle(long time) {
                drawBackground(gc);
                drawObject(gc);
            }
        };

        timer.start();

        stage.setMaxHeight(canvas.getHeight());
        stage.setMaxWidth(canvas.getWidth());
        stage.setMinHeight(canvas.getHeight());
        stage.setMinWidth(canvas.getWidth());


        StackPane root = new StackPane(canvas);
        Scene scene = new Scene(root,width,height);
        stage.setScene(scene);
        stage.setTitle("Skibidi sigma michal");
        stage.show();
    }

    private void loadImages() {
        for (ImageId id : ImageId.values()) {
            Image image = new Image(id.getFileName());
            id.setWidth(image.getWidth());
            id.setHeight(image.getHeight());
            images.put(id,image);
        }
    }

    private void drawBackground(GraphicsContext gc) {
        gc.drawImage(images.get(ImageId.BACKGROUND), 0, 0);
    }

    private void drawObject(GraphicsContext gc) {
        gc.drawImage(images.get(ImageId.BIRD), 100, 500);
    }

    public static void main(String[] args) {
        launch();
    }
}