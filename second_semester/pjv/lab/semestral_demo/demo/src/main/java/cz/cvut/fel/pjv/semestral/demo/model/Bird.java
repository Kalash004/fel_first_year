package cz.cvut.fel.pjv.semestral.demo.model;

public class Bird extends GameItem {
    private double speedY;
    private final double passiveAcceleration;
    private final double activeAcceleration;

    public Bird(ImageId imageId, double x, double y, double speedY, double passiveAcceleration, double activeAcceleration) {
        super(imageId, x, y);
        this.speedY = speedY;
        this.passiveAcceleration = passiveAcceleration;
        this.activeAcceleration = activeAcceleration;
    }

    public void update() {
        speedY += passiveAcceleration;
        if (speedY > passiveAcceleration){
            speedY = passiveAcceleration;
        }
        y += speedY;
    }

    public void jump() {
        this.speedY += activeAcceleration;
    }
}
