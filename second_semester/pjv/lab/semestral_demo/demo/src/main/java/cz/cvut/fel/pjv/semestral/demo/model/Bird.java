package cz.cvut.fel.pjv.semestral.demo.model;

public class Bird extends GameItem {
    private double speedY;
    private final double passiveAcceleration;


    public Bird(ImageId imageId, double x, double y, double speedY, double passiveAcceleration) {
        super(imageId, x, y);
        this.speedY = speedY;
        this.passiveAcceleration = passiveAcceleration;
    }

    public void update() {
        y += speedY;
        y -= passiveAcceleration;
    }
}
