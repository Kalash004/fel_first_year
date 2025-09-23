package cz.cvut.fel.pjv.semestral.demo.model;

public class Game {
    private final double pipeSpeed = -.5;
    private final double passiveAcceleration = 0.025; // down
    private final double activeAcceleration = -2;

    private final GameItem background = new GameItem(ImageId.BACKGROUND, 0, 0);
    private final Pipe topPipe = new Pipe(ImageId.TOP_PIPE, 400, -400, pipeSpeed, background.getImageId().getWidth());
    private final Pipe bottomPipe = new Pipe(ImageId.BOTTOM_PIPE, 400, 400, pipeSpeed, background.getImageId().getWidth());
    private final Bird bird = new Bird(ImageId.BIRD, 100, 280, 0, passiveAcceleration, activeAcceleration);

    public void update() {
        topPipe.update();
        bottomPipe.update();
        bird.update();
        checkCollisions();
    }

    public Drawable[] getDrawableObjects() {
        return new Drawable[]{
                new Drawable(background.getImageId(), background.getX(), background.getY()),
                new Drawable(topPipe.getImageId(), topPipe.getX(), topPipe.getY()),
                new Drawable(bottomPipe.getImageId(), bottomPipe.getX(), bottomPipe.getY()),
                new Drawable(bird.getImageId(), bird.getX(), bird.getY()),
        };
    }

    private void checkCollisions() {
        if (bird.getBoundingBox().intersects())
    }

    public void birdJump() {
        bird.jump();
    }
}
